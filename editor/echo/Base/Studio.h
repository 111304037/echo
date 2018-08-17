#pragma once

#include <string>
#include <engine/core/log/LogOutput.h>
#include "ProjectMgr.h"
#include "ConfigMgr.h"
#include "IInputController.h"
#include "Logic/ThumbnailMgr/ThumbnailMgr.h"
#include <QWidget>
#include <QDockWidget>

namespace Studio
{
	// �ļ�֧��
	struct AssetSupportInfo
	{
		std::string		m_icon;			// ͼ��
		std::string		m_tabName;		// Tab����
		std::string		m_name;			// ��Դ����
		std::string		m_ext;			// ��Դ����׺

		AssetSupportInfo()
		{}

		AssetSupportInfo( const std::string& tab, const std::string& ext)
			: m_tabName(tab)
			, m_ext(ext)
		{}

		// ��������� "=="
		bool operator == (const AssetSupportInfo& src) const
		{
			return (m_tabName == src.m_tabName) && (m_ext == src.m_ext);
		}
	};
	typedef std::vector<AssetSupportInfo> AssertsSupportInfoArray;

	class MainWindow;
	class ProjectWnd;
	class RenderWindow;
	class LogPanel;
	class AStudio
	{
	public:
		~AStudio();

		// ������ģʽ��ʼ��
		AStudio(const char* inputProject);

		// instance
		static AStudio* instance();

		// ����
		void Start();

		// �ر�
		void Close();

		// ���ó�����·��
		void setAppPath( const char* appPath);

		// ��ȡ������·��
		const char* getAppPath() const { return m_appPath.c_str(); }

		// ��ȡ���ù�����
		ConfigMgr* getConfigMgr() { return m_projectCfg; }

		// ��ȡ��־����
		LogPanel* getLogPanel() { return m_logPanel; }

		// ��ȡ��Ⱦ����
		virtual QWidget* getRenderWindow();

		// ��������ͼ
		virtual bool saveThumbnail( const Echo::String& fileName, int type = 0 );

		// �����ļ�����ȡ����ͼȫ·��
		Echo::String getThumbnailPath( const Echo::String& filePath,bool needOldExt = true);
		
		// �������������
		virtual void resetCamera(float diroffset = 0);

		// ������Ⱦ���ڿ�����
		virtual void setRenderWindowController( IRWInputController* controller);

		virtual IRWInputController* getRenderWindowController();

	public:
		// ��Դ�Ƿ�ɱ�ɾ��
		virtual bool isResCanbeDeleted( const char* res);

		// ɾ����Դ
		virtual bool deleteResource(const char* res);

	public:
		// ��ȡ������
		QWidget* getMainWindow();
		ProjectWnd* getProjectWindow();
		QDockWidget* getPropertyPanel();

		// ����Ŀ�ļ�
		void OpenProject( const char* fileName);

	private:
		AStudio();

		// ��ʼ����־ϵͳ
		bool initLogSystem();

		// ����ͼ�Ƿ����
		bool isThumbnailExists(const Echo::String& name);

		bool replaceTraverseAllWidget(QWidget* parent, QWidget* from, QWidget* to);

		void loadAllRecentProjects();

	private:
		Echo::String					m_appPath;			// ������ӪĿ¼
		MainWindow*						m_mainWindow;		// ������
		ProjectWnd*						m_projectWindow;
		RenderWindow*					m_renderWindow;		// ��Ⱦ����
		Echo::LogDefault*				m_log;				// ��־ʵ��
		LogPanel*						m_logPanel;			// ��־���
		ConfigMgr*						m_projectCfg;		// �����ļ�

		std::vector<AssetSupportInfo>	m_assetSupportInfos;// ��Դ�༭�����Ϣ
	};
}

#define Studio_ProjectMgr Studio::AStudio::Instance()->getProjectMgr()
