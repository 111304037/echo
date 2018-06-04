#include "engine/core/base/class.h"
#include "engine/core/Memory/MemManager.h"
#include "Engine/core/main/Engine.h"
#include "engine/core/Util/PathUtil.h"
#include "engine/core/Util/Exception.h"
#include "engine/core/io/IO.h"
#include "engine/core/Util/LogManager.h"
#include "engine/core/render/render/ImageCodecMgr.h"
#include "Engine/core/Scene/NodeTree.h"
#include "engine/core/Util/Timer.h"
#include "engine/core/render/render/Viewport.h"
#include "Engine/core/Render/MaterialInst.h"
#include "ProjectSettings.h"
#include "Engine/modules/Audio/FMODStudio/FSAudioManager.h"
#include "EngineTimeController.h"
#include "engine/core/script/lua/LuaEx.h"
#include "engine/core/script/lua/register_core_to_lua.cxx"
#include "engine/core/script/lua/LuaBinder.h"
#include "engine/core/render/RenderTargetManager.h"
#include "module.h"
#include "engine/core/render/renderstage/RenderStage.h"
#include "engine/core/render/gles/GLES2.h"
#include "OpenMPTaskMgr.h"

#ifdef ECHO_PLATFORM_ANDROID
#include <sys/syscall.h>
static unsigned int _GetCurrThreadId()
{
	return (unsigned int)syscall(224); 
}
#else
static unsigned int _GetCurrThreadId()
{
	return 0;
}
#endif

namespace Echo
{
	// ���캯��
	Engine::Engine()
		: m_isInited(false)
		, m_pAssetMgr(NULL)
		, m_bRendererInited(NULL)
		, m_currentTime(0)
		, m_enableBloom(false)
		, m_enableFilterAdditional(false)
		, m_framebufferScale(1.0f)
		, m_renderer(NULL)
		, m_projectFile(NULL)
#ifdef ECHO_PROFILER
		, m_profilerSev( nullptr)
#endif
	{
#ifdef ECHO_PLATFORM_WINDOWS
		// ���Windows VS2013 Thread join����BUG
		_Cnd_do_broadcast_at_thread_exit();
#endif
		MemoryManager::instance();
		Time::instance();
		OpenMPTaskMgr::instance();
	}

	Engine::~Engine()
	{
	}

	// instance
	Engine* Engine::instance()
	{
		static Engine* inst = new Engine();
		return inst;
	}

	// װ����־ϵͳ
	bool Engine::initLogSystem()
	{
		LogManager::instance();

		return true;
	}

	// �����ʼ��
	bool Engine::initialize(const Config& cfg)
	{
		m_cfg = cfg;
		m_pAssetMgr = cfg.pAssetMgr;

		// check root path
		setlocale(LC_ALL, "zh_CN.UTF-8");

		try
		{
			if (!PathUtil::IsFileExist( cfg.projectFile))
			{
				EchoLogError("Set root path failed [%s], initialise Echo Engine failed.", cfg.projectFile.c_str());
				return false;
			}

			ImageCodecMgr::instance();
			IO::instance();
		}
		catch (const Exception &e)
		{
			EchoLogError(e.getMessage().c_str());
			return false;
		}

#if !defined(NO_THEORA_PLAYER)
		// ������Ƶ���ŵ���
		EchoNew( VideoPlay);
#endif
		// ������Ŀ�ļ�
		loadProject(cfg.projectFile.c_str());

		// lua script
		{
			luaex::LuaEx* luaEx = luaex::LuaEx::instance();
			LuaBinder::instance()->init(luaEx->get_state());
			register_core_to_lua();
			registerClassTypes();
		}
		
		// ��Ƶ������
		FSAudioManager::instance()->init(cfg.m_AudiomaxVoribsCodecs,cfg.m_AudioLoadDecompresse);
		loadAllBankFile();

		setEnableFrameProfile(true);

		// init render
		Renderer* renderer = nullptr;
		LoadGLESRenderer(renderer);

		Echo::Renderer::RenderCfg renderCfg;
		renderCfg.enableThreadedRendering = false;
		renderCfg.windowHandle = cfg.m_windowHandle;
		renderCfg.enableThreadedRendering = false;
		initRenderer(renderer, renderCfg);

		Renderer::BGCOLOR = Echo::Color(0.298f, 0.298f, 0.322f);

		if (m_cfg.m_isGame)
		{
			loadLaunchScene();
		}

		m_isInited = true;

		return true;
	}

	void Engine::loadLaunchScene()
	{
		Echo::String launchScene = "Res://main.scene";
		//const ProjectSettings::Setting* setting = m_projectFile->getSetting("Application/Run/LaunchScene");
		//if (setting)
		{
			Echo::Node* node = Echo::Node::load(launchScene);
			node->setParent(NodeTree::instance()->getInvisibleRootNode());
		}
	}

	// register all class types
	void Engine::registerClassTypes()
	{
		Class::registerType<Node>();

		// register all module class
		Module::registerAllTypes();
	}

	// ������Ŀ,�����ʼ��ʱ���Զ����ã�Ҳ�ɵ�������(ȫ·��)
	void Engine::loadProject(const char* projectFile)
	{
		if (PathUtil::IsFileExist(projectFile))
		{
			m_resPath = PathUtil::GetFileDirPath(projectFile);

			EchoSafeDelete(m_projectFile, ProjectSettings);
			m_projectFile = EchoNew(ProjectSettings);
			m_projectFile->load(projectFile);
			m_projectFile->setupResource();

			IO::instance()->setResPath(m_resPath);
		}
		else
		{
			EchoLogError("Not found project file [%s], initialise Echo Engine failed.", projectFile);
		}
	}
	
	void Engine::loadAllBankFile()
	{
		FSAudioManager::instance()->loadAllBankFile();
	}

	// ��ʼ����Ⱦ��
	bool Engine::initRenderer(Renderer* pRenderer, const Renderer::RenderCfg& config)
	{
		EchoLogDebug("Canvas Size : %d x %d", config.screenWidth, config.screenHeight);

		m_renderer = pRenderer;
		EchoAssert(pRenderer);
		if (!pRenderer->initialize(config))
		{
			EchoAssert(false);
			EchoLogError( "Root::initRenderer failed...");
			return false;
		}

		if (!onRendererInited())
			return false;

		EchoLogInfo("Init Renderer success.");

		m_settingsMgr.Apply(m_cfg.engineCfgFile);

		// ��ʼ����ȾĿ�������
		if (!RenderTargetManager::instance()->initialize())
		{
			EchoLogError("RenderTargetManager::initialize Falied !");

			return false;
		}

		EchoLogInfo("Initialize RenderStageManager Success !");

		return true;
	}

	// ����Ϸ����ʱ��������Ҫ���еĴ���
	void Engine::onPlatformSuspend()
	{
		FSAudioManager::instance()->suspendFmodSystem();
	}

	// ����Ϸ�ӹ����лָ�ʱ������Ҫ���еĴ���
	void Engine::onPlatformResume()
	{
		FSAudioManager::instance()->resumeFmodSystem();
	}

	// ��Ⱦ��ʼ��
	bool Engine::onRendererInited()
	{
		if (m_bRendererInited)
			return true;

		if (!NodeTree::instance()->init())
			return false;

		// setup viewport
		Viewport* pViewport = Renderer::instance()->getFrameBuffer()->getViewport();

		Camera* p2DCamera = NodeTree::instance()->get2DCamera();
		pViewport->setViewProjMatrix(p2DCamera->getViewProjMatrix());
		ui32 width = pViewport->getWidth();
		ui32 height = pViewport->getHeight();

		m_bRendererInited = true;

		return true;
	}

	bool Engine::onSize(ui32 width, ui32 height)
	{
		if (m_bRendererInited)
		{
			Renderer::instance()->onSize(width, height);

			Camera* pMainCamera = NodeTree::instance()->get3dCamera();
			pMainCamera->setWidth(Real(width));
			pMainCamera->setHeight(Real(height));
			pMainCamera->update();

			Camera* p2DCamera = NodeTree::instance()->get2DCamera();
			p2DCamera->setWidth(Real(width));
			p2DCamera->setHeight(Real(height));
			p2DCamera->update();

			Renderer::instance()->getFrameBuffer()->getViewport()->setViewProjMatrix(p2DCamera->getViewProjMatrix());
		}

		// ��ȾĿ�����ô�С
		if (RenderTargetManager::instance())
		{
			RenderTargetManager::instance()->onScreensizeChanged(width, height);
		}

		return true;
	}

	// ��Ϸ����
	void Engine::destroy()
	{
		// ����������
		EchoSafeDeleteInstance(NodeTree);

		// ��Ƶ������
		FSAudioManager::instance()->release();

		EchoSafeDeleteInstance(FSAudioManager);
		
		EchoSafeDeleteInstance(ImageCodecMgr);
		EchoSafeDeleteInstance(OpenMPTaskMgr);

		EchoSafeDeleteInstance(IO);
		EchoSafeDeleteInstance(Time);

		// �ⲿģ���ͷ�
		//for (const ExternalMgr& mgr : m_cfg.m_externalMgrs)
		//	mgr.m_release();
		releasePlugins();
		
		EchoLogInfo("Echo Engine has been shutdown.");
		
		EchoSafeDeleteInstance(RenderTargetManager);
		
		// ��Ⱦ��
		if (m_renderer)
		{
			m_renderer->destroy();
			EchoSafeDelete(m_renderer, Renderer);
			EchoLogInfo("Echo Renderer has been shutdown.");
		}

		EchoSafeDeleteInstance(LogManager);
		EchoSafeDelete(m_projectFile, ProjectSettings);
#ifdef ECHO_PROFILER
#endif
		// ����ʱ�������
		EngineTimeController::destroy();
		LuaBinder::destroy();
		luaex::LuaEx::instance()->destroy();
		MemoryManager::destroyInstance();
	}

	const String& Engine::getResPath() const
	{
		return m_resPath;
	}

	const String& Engine::getUserPath() const
	{
		return m_userPath;
	}

	void Engine::setUserPath(const String& strPath)
	{
		m_userPath = strPath;

		IO::instance()->setUserPath(m_userPath);
	}

	void Engine::SetPhoneinformation(int max, int free, String tex)
	{
		Maxmemory = max;
		Freememory = free;
		cputex = tex;
	}

	void* Engine::getAssetManager() const
	{
		return m_pAssetMgr;
	}

	bool Engine::isRendererInited() const
	{
		return m_bRendererInited;
	}

	const ui32& Engine::getCurrentTime() const
	{
		return m_currentTime;
	}

	// ÿ֡����
	void Engine::tick(i32 elapsedTime)
	{
		elapsedTime = Math::Clamp( elapsedTime, 0, 1000);
		m_frameTime = elapsedTime * 0.001f;

#ifdef ECHO_PROFILER
#endif
		m_currentTime = Time::instance()->getMilliseconds();


#if !defined(NO_THEORA_PLAYER)
		// ��Ƶ����
		VideoPlay::Instance()->tick(elapsedTime);
#endif

		// ��������
		auto t0 = EngineTimeController::instance()->getSpeed(EngineTimeController::ET_SOUND);
		auto t1 = EngineTimeController::instance()->getSpeed();
		FSAudioManager::instance()->tick(static_cast<ui32>(elapsedTime * t0 / t1));

		//
		auto t = EngineSettingsMgr::instance()->isSlowDownExclusiveUI() ? m_frameTime : elapsedTime;

		NodeTree::instance()->update(elapsedTime*0.001f);

		// ִ�ж�������
		OpenMPTaskMgr::instance()->execTasks(OpenMPTaskMgr::TT_AnimationUpdate);
		OpenMPTaskMgr::instance()->waitForAnimationUpdateComplete();

		// �ⲿģ�����, Ŀǰֻ�� CatUI
		for (const ExternalMgr& mgr : m_cfg.m_externalMgrs)
		{
			mgr.m_tick(elapsedTime);
		}

		// ��Ⱦ
		render();

		// present to screen
		Renderer::instance()->present();
	}

	void Engine::changeFilterAdditionalMap(const String& mapName)
	{
		if (m_enableFilterAdditional)
			RenderTargetManager::instance()->changeFilterBlendmapName(mapName);
	}

	// ��Ⱦ����
	bool Engine::render()
	{
		// �ⲿģ�����, Ŀǰֻ�� CatUI
		for (const ExternalMgr& mgr : m_cfg.m_externalMgrs)
		{
			mgr.m_render();
		}

		RenderStage::instance()->process();

		return true;
	}

	void Engine::releasePlugins()
	{
		// �ⲿģ���ͷ�
		for (ExternalMgr& mgr : m_cfg.m_externalMgrs)
		{
			if (!mgr.m_isReleased)
			{
				mgr.m_release();
				mgr.m_isReleased = true;
			}
		}
	}
}
