#include "Application.h"
#include <Foundation/Util/Timer.h>
#include <Engine/Object/Root.h>
#include <GLES2Render/GLES2.h>
#include <Foundation/pfs/pfs2.h>
#include <Foundation/platform/utils/PlatformStringUtil.h>
#include "Render/RenderTask.h"

// ƽ̨ϸ��;
#ifdef LORD_PLATFORM_WINDOWS
	#include "Platform/ApplicationWin32.inl"
#elif defined(LORD_PLATFORM_MAC_IOS)
	#include "Platform/ApplicationiOS.inl"
#elif defined(LORD_PLATFORM_ANDROID)
	#include "Platform/ApplicationAndroid.inl"
#elif defined(LORD_PLATFORM_HTML5)
	#include "Platform/ApplicationHtml5.inl"
#else
	#error "Platform Unknown for EngineExample!"
#endif

#include "VenusPentagramExample.h"
#include "EffectOcean.h"

namespace Examples
{
	// ע��ʾ��ǰ������
	void registerAllExamples();

	// ���캯��
	Application::Application()
		: m_log( NULL)
		, m_curExample( NULL)
		, m_curExampleIdx( -1)
		, m_uiManager( NULL)
		, m_selectWindow(NULL)
		, m_circulateWindow(NULL)
	{
        g_app = this;
	}

	// ��������
	Application::~Application()
	{

	}

	//for android.
	void Application::setAssetPath(const char* path, const char* internal_path)
	{
		m_path = path; 
		m_internalPath = internal_path; 
	}

	// ����ִ��
	void Application::start(LORD::i32 width, LORD::i32 height)
	{
		registerAllExamples();

		m_screenWidth = width;
		m_screenHeight = height;

		// 1.��ʼ������
		initEngine();

		// 2.��ʼ��UI
		initUI();
	}

	// ��ʼ������
	void Application::initEngine()
	{
		GetClientSizeImpl(m_screenWidth, m_screenHeight);

		LordNew(LORD::Root);
		LordRoot->initLogSystem();
		// ���Ĭ����־����
		LORD::LogDefault::LogConfig logConfig;
		logConfig.logName = "lord.log";
		logConfig.logLevel = LORD::Log::LL_DEBUG;
		logConfig.bVSOutput = true;

		LordLogManager->setLogLeve(logConfig.logLevel);
		m_log = LordNew(LORD::LogDefault(logConfig));
		if (m_log)
			LordLogManager->addLog(m_log);

		LORD::Root::RootCfg rootcfg;

#if defined(LORD_PLATFORM_WINDOWS)
		rootcfg.projectFile = LORD::PathUtil::GetCurrentDir() + "/../media/examples.project";
#elif defined(LORD_PLATFORM_MAC_IOS)
		rootcfg.projectFile = LORD::PathUtil::GetCurrentDir() + "/media/examples.project";
#elif defined(LORD_PLATFORM_ANDROID)
		rootcfg.projectFile = m_path + "/media/examples.project";
#elif defined(LORD_PLATFORM_HTML5)
		rootcfg.projectFile = LORD::PathUtil::GetCurrentDir() + "media/examples.project";
#endif

		LordRoot->initialize(rootcfg);

		LORD::Renderer::RenderCfg renderCfg; 

#ifdef LORD_PLATFORM_WINDOWS
		renderCfg.windowHandle = (unsigned int)g_hwnd;
#endif
		renderCfg.screenWidth  = m_screenWidth;
		renderCfg.screenHeight = m_screenHeight;

		// ���Ĭ����Դ·��
#if defined(LORD_PLATFORM_WINDOWS) || defined(LORD_PLATFORM_MAC_IOS)
		LordResourceManager->addArchive(LORD::PathUtil::GetCurrentDir() + "/assets", "filesystem", true);
#elif defined(LORD_PLATFORM_ANDROID)
		LordResourceManager->addArchive(m_path + "/assets", "filesystem", true); 
#endif

		LORD::Renderer* render = NULL;
		LORD::LoadGLESRenderer(render);
		if (render)
		{
			LordRoot->initRenderer( render, renderCfg);
		}
		else
		{
			LordLogError( "Examples Init GLESRender Failed!");
		}

		// ����ɫ
		LORD::Renderer::BGCOLOR = LORD::Color(0.298f, 0.298f, 0.322f);
	}

	// ��ʼ��UI
	void Application::initUI()
	{
 		m_uiManager = LordNew( UIManager);
 		m_uiManager->initialize();
 
 		// ����������
 		m_selectWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout("examplesselect.layout");
 		m_uiManager->getRootWindow()->addChildWindow(m_selectWindow);
 
 		// ����ʾ��Cell
 		int exampleIdx = 0; 
  		CEGUI::ScrollablePane* exPanel = LORD_DOWN_CAST<CEGUI::ScrollablePane*>(m_selectWindow->getChildRecursive("battle/changjing/fubenback/cell"));
 		for (LORD::vector<ExampleFactory*>::type::iterator it = g_exampleFactorys.begin(); it != g_exampleFactorys.end(); it++)
  		{
			ExampleFactory* item = *it;
 			CEGUI::Window* cell = CEGUI::WindowManager::getSingleton().loadWindowLayout("exampleselectcell.layout",  LORD::StringUtil::Format("%d", exampleIdx).c_str());
 	
 			// ��������
 			CEGUI::Window* name = cell->getChildRecursive(LORD::StringUtil::Format("%dexampleselectcell/name", exampleIdx).c_str());
 			name->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(CEGUIFunctor(exampleIdx)));
			name->setText(item->m_name.c_str());
 
 			// ��������
			LORD::String imageset = item->m_imageSet.c_str();
 			CEGUI::Window* thumb = cell->getChildRecursive(LORD::StringUtil::Format("%dexampleselectcell/pic", exampleIdx).c_str());
 			thumb->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(CEGUIFunctor(exampleIdx)));
 			thumb->setProperty("Image", imageset.c_str());
 
 			cell->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(CEGUIFunctor(exampleIdx)));
  			exPanel->addChildWindow(cell);
 
 			int row = static_cast<int>(exampleIdx / 5);
 			int col = static_cast<int>(exampleIdx % 5);
 			CEGUI::UVector2 pos = CEGUI::UVector2(CEGUI::UDim(0, col * 190+1.f), CEGUI::UDim(0, row * 190+1.f));
 			cell->setPosition(pos);
 
 			exampleIdx++;
  		}
 
 		// ����ʾ��ѡ�񴰿�
 		m_circulateWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout("circulate.layout");
 		m_uiManager->getRootWindow()->addChildWindow(m_circulateWindow);
 		m_circulateWindow->setVisible(false);
 
 		// ע���¼�
 		CEGUI::Window* wbClose = m_circulateWindow->getChildRecursive("select/close"); LordAssert(wbClose);
 		wbClose->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(CEGUIFunctor(-1)));
 		CEGUI::Window* wbNext = m_circulateWindow->getChildRecursive("select/next"); LordAssert(wbNext);
 		wbNext->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Application::nextExample, this));
 		CEGUI::Window* wbLast = m_circulateWindow->getChildRecursive("select/last"); LordAssert(wbLast);
 		wbLast->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Application::lastExample, this));
 		CEGUI::Window* wbRestart = m_circulateWindow->getChildRecursive("select/re"); LordAssert(wbRestart);
 		wbRestart->subscribeEvent( CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&Application::restartExample, this));
	}

	// ��ⴰ�ڴ�С�仯
	void Application::checkScreenSize()
	{
		LORD::i32 width  = m_screenWidth;
		LORD::i32 height = m_screenHeight; 

		GetClientSizeImpl(width, height);

		if (width != m_screenWidth || height != m_screenHeight)
		{
			m_screenWidth  = width;
			m_screenHeight = height;

			resize(m_screenWidth, m_screenHeight);
		}
	}

	// �޸Ĵ��ڴ�С
	void Application::resize(int cx, int cy)
	{
		LordRoot->onSize(cx, cy);
		m_uiManager->onSize(cx, cy);

		//InitUICamera(LordRender->getScreenWidth(), LordRender->getScreenHeight());
		//m_guiCamera->setWidthHeight( cx, cy);
	}

	// ����
	void Application::tick( LORD::ui32 elapsedTime)
	{
		// ��ⴰ�ڴ�С�仯
		checkScreenSize();

		// ����ʾ��
		if (m_curExample)
			m_curExample->tick(elapsedTime);

		// ����UI
		m_uiManager->tick( elapsedTime);

		// ����
		if (LordEngineSettings.isInitVRMode() && LordEngineSettings.isUseVRMode()) // �����۸���
			LordSceneManager->getMainCamera()->setVRModeForEye(true);

		LordRoot->frameMove( elapsedTime);

		LordRoot->renderScene();

		// ��ʾ����Ļ
		LordRender->present();
	}

	// ����������ȡʾ������
	ExampleFactory* Application::getFactoryByIdx(int idx)
	{
		idx = LORD::Math::Clamp(idx, 0, (int)g_exampleFactorys.size());
		return g_exampleFactorys[idx];
	}

	// ��һʾ��
	bool Application::nextExample(const CEGUI::EventArgs& args)
	{
		m_curExampleIdx++;
		if (m_curExampleIdx >= g_exampleFactorys.size())
			m_curExampleIdx = 0;

		startExample(m_curExampleIdx);

		return true;
	}

	// ������һʾ��
	bool Application::lastExample(const CEGUI::EventArgs& args)
	{
		m_curExampleIdx--;
		if (m_curExampleIdx >= g_exampleFactorys.size())
			m_curExampleIdx = g_exampleFactorys.size() - 1;

		startExample(m_curExampleIdx);

		return true;
	}

	// ������ǰʾ��
	bool Application::restartExample(const CEGUI::EventArgs& args)
	{
		startExample(m_curExampleIdx);

		return true;
	}

	// ����ʾ��
	void Application::startExample(LORD::ui32 index)
	{
		// ɾ����ǰʾ��
		if (m_curExample)
		{
			m_curExample->destroy();
			LordSafeDelete(m_curExample, Example);
		}

		if (index < g_exampleFactorys.size())
		{
			m_curExampleIdx = index;
			m_curExample = getFactoryByIdx(index)->create();
			m_curExample->initialize();
		}

		// �ر�����ʾ��ѡ�����
		m_selectWindow->setVisible(m_curExample ? false : true);
		m_circulateWindow->setVisible(m_curExample ? true : false);
	}

	// ������Ϣ
	void Application::keyboardProc(LORD::ui32 keyChar, bool isKeyDown)
	{
		keyboardProcImpl(keyChar, isKeyDown); 

		// ���ݰ�����Ϣ��ʾ��
		if (m_curExample)
		{
			m_curExample->keyboardProc( keyChar, isKeyDown);
		}
	}

	void Application::mouseWhellProc( LORD::i32 parma )
	{
		if ( m_curExample )
		{
			m_curExample->mouseWhellProc( parma );
		}
	}

	void Application::mouseProc( LORD::i32 xpos, LORD::i32 ypos )
	{
		if ( m_curExample )
		{
			m_curExample->mouseProc( xpos, ypos );
		}
	}

	// ��������Ϣ
	void Application::mouseLBProc( LORD::i32 xpos, LORD::i32 ypos, bool isUp )
	{
		// ��������ϵ
		float uix = (float)xpos / CEGUI::System::getSingleton().GetAdapter()->GetRateW();
		float uiy = (float)ypos / CEGUI::System::getSingleton().GetAdapter()->GetRateH();

		// UI����
		if ( isUp )
		{
			CEGUI::System& uiSystem = CEGUI::System::getSingleton();
			uiSystem.injectMousePosition(uix, uiy);
			uiSystem.injectMouseButtonUp(CEGUI::LeftButton);
		}
		else
		{
			CEGUI::System& uiSystem = CEGUI::System::getSingleton();
			uiSystem.injectMousePosition(uix, uiy);
			uiSystem.injectMouseButtonDown(CEGUI::LeftButton);
		}
		

		// ʾ��
		if ( m_curExample && !isUp )
		{
			m_curExample->mouseLBProc( xpos, ypos );
		}
	}

	void Application::mouseMoveProc( LORD::i32 xpos, LORD::i32 ypos )
	{
		CEGUI::System::getSingleton().injectMouseMove( float(xpos), float(ypos));

		if ( m_curExample )
		{
			m_curExample->mouseMoveProc( xpos, ypos );
		}
	}

	void Application::end()
	{
		LordRoot->destroy();
	}

	void Application::flushRender()
	{
		LORD::FlushRenderTasks();
	}

	// ���������
	bool CEGUIFunctor::operator ()(const CEGUI::EventArgs& args) const
	{
		g_app->startExample( m_id);

		return true;
	}
}


