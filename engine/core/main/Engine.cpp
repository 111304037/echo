#include "engine/core/base/class.h"
#include "engine/core/Memory/MemManager.h"
#include "Engine/core/main/Engine.h"
#include "engine/core/Util/PathUtil.h"
#include "engine/core/Util/Exception.h"
#include "engine/core/io/IO.h"
#include "engine/core/log/LogManager.h"
#include "engine/core/render/render/ImageCodecMgr.h"
#include "engine/core/scene/render_node.h"
#include "Engine/core/Scene/NodeTree.h"
#include "engine/core/Util/Timer.h"
#include "engine/core/render/render/Viewport.h"
#include "Engine/core/Render/Material.h"
#include "ProjectSettings.h"
#include "Engine/modules/Audio/FMODStudio/FSAudioManager.h"
#include "engine/core/script/lua/LuaEx.h"
#include "engine/core/script/lua/register_core_to_lua.cxx"
#include "engine/core/script/lua/LuaBinder.h"
#include "engine/core/render/RenderTargetManager.h"
#include "module.h"
#include "engine/core/render/renderstage/RenderStage.h"
#include "engine/core/render/gles/GLES2.h"
#include "engine/core/script/LuaScript.h"
#include "engine/core/render/render/ShaderProgramRes.h"
#include "engine/core/render/TextureCube.h"
#include "engine/core/gizmos/Gizmos.h"

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
		, m_bRendererInited(NULL)
		, m_currentTime(0)
		, m_renderer(NULL)
	{
#ifdef ECHO_PLATFORM_WINDOWS
		// ���Windows VS2013 Thread join����BUG
		_Cnd_do_broadcast_at_thread_exit();
#endif
		MemoryManager::instance();
		Time::instance();
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

		// check root path
		setlocale(LC_ALL, "zh_CN.UTF-8");

		try
		{
			if (!PathUtil::IsFileExist( cfg.m_projectFile))
			{
				EchoLogError("Set root path failed [%s], initialise Echo Engine failed.", cfg.m_projectFile.c_str());
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

		// lua script
		{
			luaex::LuaEx* luaEx = luaex::LuaEx::instance();
			LuaBinder::instance()->init(luaEx->get_state());
			register_core_to_lua();
			registerClassTypes();
		}

		// ������Ŀ�ļ�
		loadProject(cfg.m_projectFile.c_str());
		
		// ��Ƶ������
		FSAudioManager::instance()->init(cfg.m_AudiomaxVoribsCodecs,cfg.m_AudioLoadDecompresse);
		loadAllBankFile();

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
		if (m_projectSettings)
		{
			const ResourcePath& launchScene = m_projectSettings->getLaunchScene();
			if (!launchScene.isEmpty())
			{
				Echo::Node* node = Echo::Node::load(launchScene.getPath());
				node->setParent(NodeTree::instance()->getInvisibleRootNode());
			}
			else
			{
				EchoLogError("Please set Game.LaunchScene before start the game");
			}
		}
	}

	// register all class types
	void Engine::registerClassTypes()
	{
		Class::registerType<Node>();
		Class::registerType<Render>();
		Class::registerType<Res>();
		Class::registerType<ShaderProgramRes>();
		Class::registerType<Material>();
		Class::registerType<LuaScript>();
		Class::registerType<TextureCube>();
		Class::registerType<ProjectSettings>();
		Class::registerType<Gizmos>();

		// register all module class
		Module::registerAllTypes();
	}

	// ������Ŀ,�����ʼ��ʱ���Զ����ã�Ҳ�ɵ�������(ȫ·��)
	void Engine::loadProject(const char* projectFile)
	{
		if (PathUtil::IsFileExist(projectFile))
		{
			m_resPath = PathUtil::GetFileDirPath(projectFile);
			IO::instance()->setResPath(m_resPath);
		
			String resPath;
			if(IO::instance()->covertFullPathToResPath(projectFile, resPath))
				m_projectSettings = ECHO_DOWN_CAST<ProjectSettings*>(Res::get(ResourcePath(resPath)));
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

		Camera* p2DCamera = NodeTree::instance()->get2dCamera();
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

			Camera* p2DCamera = NodeTree::instance()->get2dCamera();
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
		EchoSafeDeleteInstance(IO);
		EchoSafeDeleteInstance(Time);
		
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

		m_currentTime = Time::instance()->getMilliseconds();

		// ��������
		FSAudioManager::instance()->tick(static_cast<ui32>(elapsedTime));
		NodeTree::instance()->update(elapsedTime*0.001f);

		// ��Ⱦ
		render();

		// present to screen
		Renderer::instance()->present();
	}

	// ��Ⱦ����
	bool Engine::render()
	{
		RenderStage::instance()->process();

		return true;
	}
}
