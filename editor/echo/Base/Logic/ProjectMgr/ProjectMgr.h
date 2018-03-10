#pragma once

#include <Engine/core/main/ProjectFile.h>

namespace Studio
{
	/**
	 * ��Ŀ�ļ������� 2014-7-7
	 */
	class ProjectMgr
	{
		typedef std::map<std::string, Echo::ProjectFile::TextureCompressType>  CompressMap;
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

		// �½���Ŀ�ļ�
		void NewProject( const char* projectName);

		// ����Ŀ�ļ�(ȫ·��)
		void OpenProject(const char* fullPathName);

		// ������Ŀ�ļ�
		void SaveProject();

		// ������Ŀ�ļ�
		bool LoadProject(const char* fileName);

		// ��ȡ��Ŀ�ļ�
		Echo::ProjectFile& GetProjectFile() { return m_projectFile; }

		// ��ȡ�����ļ�
		Echo::map<Echo::String, Echo::String>::type& GetAllFiles() { return m_files; }

		// ��ȡ��·��
		const Echo::String& GetRootPath() const { return m_projectFile.getPath(); }

		// ��ȡȫ·��
		Echo::String GetFullPath( const Echo::String& name);

		// �ж��ļ��Ƿ����
		bool IsFileExist( const char* fileName);

		// ����ļ�
		void AddFileToProject( const char* fullPath);

		void RemoveFileToProject(const char* fileName);

		// ��ȡ��Ŀ�ļ�·������
		const Echo::String& GetProjectFilePathName() { return m_projectFile.getPathName(); }

		//��¼ѹ����ʽ�仯���ļ�
		void addCompressChangeTextrue(PLATFORM platform, std::string name, Echo::ProjectFile::TextureCompressType ctype);

		// ����ͼƬ���ֻ�ȡѹ����ʽ��Ϣ
		Echo::ProjectFile::TextureCompressItem* GetTextureCompressItem(const Echo::String& name);

		// ��Դ���
		void CheckProject();

	private:
		// ���������Դ
		void checkTextures();

	private:
		Echo::ProjectFile							m_projectFile;		 // ��Ŀ������
		Echo::map<Echo::String, Echo::String>::type	m_files;			 // �����ļ�
		Echo::ProjectFile::TextureCPIVec*			m_pTextureCompreses; // ����ѹ����Ϣ��
	};
}