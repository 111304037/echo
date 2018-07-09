#include "Studio.h"
#include "MainWindow.h"
#include "ProjectWnd.h"
#include "RenderWindow.h"
#include "LogPanel.h"
#include "ConfigMgr.h"
#include <QtUiTools/QUiLoader>
#include <QFile>
#include <QLibrary>
#include <QMetaMethod>
#include <QSplitter>
#include "QPropertyModel.h"
#include "TimelinePanel.h"
#include <shellapi.h>
#include <engine/core/util/HashGenerator.h>
#include <engine/core/util/TimeProfiler.h>
#include <engine/core/util/PathUtil.h>
#include "EchoEngine.h"


namespace Studio
{
	// ���캯��
	AStudio::AStudio()
		: m_logPanel(nullptr)
		, m_mainWindow(nullptr)
	{
		m_renderWindow = NULL;
		m_projectCfg = EchoNew( ConfigMgr);

		m_log = NULL;
	}

	AStudio::AStudio(const char* inputProject)
	{
		m_renderWindow = NULL;
		m_projectCfg = NULL;
		m_log = NULL;

		initLogSystem();
	}

	// ��������
	AStudio::~AStudio()
	{
		Echo::Engine::instance()->destroy();
		EchoSafeDelete(m_logPanel, LogPanel);

		EchoSafeDelete(m_projectWindow, ProjectWnd);
		EchoSafeDelete(m_renderWindow, RenderWindow);
		EchoSafeDelete(m_projectCfg, ConfigMgr);
		EchoSafeDelete(m_mainWindow, MainWindow);

		// ��ӡ�ڴ�й©��־
		Echo::MemoryManager::outputMemLeakInfo();
	}

	// instance
	AStudio* AStudio::instance()
	{
		static AStudio* inst = new AStudio;
		return inst;
	}

	// ��ʼ����־ϵͳ
	bool AStudio::initLogSystem()
	{
		Echo::Engine::instance();
		Echo::Engine::instance()->initLogSystem();

		// ���Ĭ����־����
		Echo::LogDefault::LogConfig logConfig;
		logConfig.logName = "echo.log";
		logConfig.logLevel = Echo::Log::LL_INVALID;
		logConfig.path = "./cache/";
		logConfig.logFilename = "echo.log";
		logConfig.bFileOutput = true;

		Echo::LogManager::instance()->setLogLeve(logConfig.logLevel);

		m_log = EchoNew(Echo::LogDefault(logConfig));
		if ( m_log )
			Echo::LogManager::instance()->addLog(m_log);

		return true;
	}

	// ����
	void AStudio::Start()
	{
		// ��ʼ��־ϵͳ
		initLogSystem();

		// ����������
		m_mainWindow = EchoNew( MainWindow);
		m_projectWindow = EchoNew( ProjectWnd);

		loadAllRecentProjects();

		// ������־���
		m_logPanel = EchoNew(LogPanel( m_mainWindow));
		Echo::LogManager::instance()->addLog(m_logPanel);
	}

	// �ر�
	void AStudio::Close()
	{

	}

	void AStudio::loadAllRecentProjects()
	{
		Echo::list<Echo::String>::type recentProjects;
		m_projectCfg->getAllRecentProject(recentProjects);

		Echo::list<Echo::String>::iterator iter = recentProjects.begin();
		for ( ; iter != recentProjects.end(); ++iter )
		{
			m_projectWindow->addRecentProject((*iter).c_str());
		}
	}

	// �ж�����ͼ�Ƿ����
	bool AStudio::isThumbnailExists(const Echo::String& name)
	{
		Echo::String appPath = AStudio::instance()->getAppPath();
		Echo::String fileFullName = Echo::StringUtil::Format("%sCache/thumbnail/%s.bmp", appPath, name);

		return Echo::PathUtil::IsFileExist(fileFullName);
	}

	bool AStudio::replaceTraverseAllWidget(QWidget* parent, QWidget* from, QWidget* to)
	{
		if ( parent && parent->layout() )
		{
			auto replaced = parent->layout()->replaceWidget(from, to);
			if ( replaced )
			{
				delete replaced;
				return true;
			}
			auto widgets = parent->findChildren<QWidget*>();
			if ( !widgets.size() )
			{
				return false;
			}
			for ( int i = 0; i < widgets.size(); ++i )
			{
				if ( replaceTraverseAllWidget(widgets[i], from, to) )
				{
					return true;
				}
			}
		}
		return false;
	}

	// ��ȡ��Ⱦ����
	QWidget* AStudio::getRenderWindow()
	{
		// �½���Ⱦ����
		if ( !m_renderWindow )
		{
			// �½�EchoEngine
			TIME_PROFILE
			(
				EchoEngine::instance();
				ThumbnailMgr::instance();
			)

			// �½���Ⱦ����
			TIME_PROFILE
			(
				m_renderWindow = EchoNew(RenderWindow);
				m_renderWindow->BeginRender();
			)
		}

		return m_renderWindow;
	}

	// ������Ⱦ���ڿ�����
	void AStudio::setRenderWindowController(IRWInputController* controller)
	{
		RenderWindow* renderWindow = qobject_cast<RenderWindow*>(m_renderWindow);
		if ( renderWindow )
		{
			renderWindow->setInputController(controller);
		}
	}

	IRWInputController* AStudio::getRenderWindowController()
	{
		RenderWindow* renderWindow = qobject_cast<RenderWindow*>(m_renderWindow);
		if (renderWindow)
		{
			return renderWindow->getInputController();
		}
		else
			return NULL;
	}

	// ��ȡ������
	QWidget* AStudio::getMainWindow()
	{
		return m_mainWindow;
	}

	ProjectWnd* AStudio::getProjectWindow()
	{
		return m_projectWindow;
	}

	// ����Ŀ�ļ�
	void AStudio::OpenProject(const char* fileName)
	{
		// remember it
		m_projectCfg->addRecentProject(fileName);

		// ��ʼ����Ⱦ����
		TIME_PROFILE(EchoEngine::SetProject(fileName);)

		// ֪ͨ������
		TIME_PROFILE
		(
			m_mainWindow->onOpenProject();
			m_mainWindow->setWindowTitle(fileName);
		)

		// ���ʱ��ռ�Ƚ��
		TIME_PROFILE_OUTPUT
	}

	// ���ó�����·��
	void AStudio::setAppPath(const char* appPath)
	{
		m_appPath = appPath;
		Echo::PathUtil::FormatPath(m_appPath, true);
	}

	// ɾ����Դ
	bool AStudio::deleteResource(const char* res)
	{

		return false;
	}

	// ��Դ�Ƿ�ɱ�ɾ��
	bool AStudio::isResCanbeDeleted(const char* res)
	{
		return true;
	}

	// ��������ͼ
	bool AStudio::saveThumbnail(const Echo::String& fileName, int type /* = 0 */)
	{
		bool success = ThumbnailMgr::instance()->saveThumbnail(fileName, ThumbnailMgr::THUMBNAIL_TYPE(type));
		if ( success )
		{
			QString itemName;
			itemName = fileName.c_str();
		}
		return success;
	}

	// �����ļ�����ȡ����ͼȫ·��
	Echo::String AStudio::getThumbnailPath(const Echo::String& filePath, bool needOldExt)
	{
		// ���˵���׺��������bmp
		Echo::String fileName = Echo::PathUtil::GetPureFilename(filePath, needOldExt);
		Echo::String appPath = Echo::PathUtil::GetCurrentDir();

		unsigned int projectHash = Echo::BKDRHash(Echo::Engine::instance()->getConfig().m_projectFile.c_str());
		Echo::String thumbnailPath = Echo::StringUtil::Format("%s/Cache/project_%d/thumbnail/%s.bmp", appPath.c_str(), projectHash, fileName.c_str());

		return thumbnailPath;
	}

	// ���������
	void AStudio::resetCamera(float diroffset)
	{
		auto* renderWindow = static_cast<RenderWindow*>(getRenderWindow());
		renderWindow->getInputController()->onInitCameraSettings(diroffset);
	}
}