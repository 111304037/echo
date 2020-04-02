#pragma once

#include <engine/core/main/GameSettings.h>

namespace Studio
{
	/**
	 * ProjectManager 2014-7-7
	 */
	class ProjectMgr
	{
	public:
		//ѹ���������ƽ̨
		enum PLATFORM
		{
			IOS		= 0, 
			ANDROID	= 1,
			WINDOWS = 2
		};

	public:
		ProjectMgr();
		~ProjectMgr();

		// new
		void newProject( const char* projectName);

		// ����Ŀ�ļ�(ȫ·��)
		void OpenProject(const char* fullPathName);

		// ��ȡ�����ļ�
		Echo::map<Echo::String, Echo::String>::type& GetAllFiles() { return m_files; }

		// ��ȡȫ·��
		Echo::String GetFullPath( const Echo::String& name);

		// �ж��ļ��Ƿ����
		bool IsFileExist( const char* fileName);

		// ����ļ�
		void AddFileToProject( const char* fullPath);

		void RemoveFileToProject(const char* fileName);

		// ��Դ���
		void CheckProject();

	private:
		// ���������Դ
		void checkTextures();

	private:
		Echo::map<Echo::String, Echo::String>::type		m_files;			 // �����ļ�
	};
}
