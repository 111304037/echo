#pragma once

#include <QObject>
#include <QFileSystemWatcher>
#include <engine/core/Memory/MemManager.h>

namespace Studio
{
	/**
	 * �ļ�ϵͳ������
	 */
	class EchoEngine;
	class FileSystemWatcher : public QObject
	{
		Q_OBJECT
		typedef Echo::map<Echo::String, Echo::StringArray>::type FileMapping;
	public:
		FileSystemWatcher();
		~FileSystemWatcher();

		// ��Ӽ���·��(�ݹ�)
		void addPath( const char* path);
		void addIgnoreFileName(const char* ignoreName);
	
	private:
		void updateThumbnail(Echo::String fileExt);

	public: signals:
		// ���ݱ䶯
		void Signal_fileChanged(const QString& file);

	public slots:
		// �ļ��޸�
		void onFileChanged(const QString& file);

		// �ļ����
		void onFileAdd(const Echo::String& file);

		// �ļ�ɾ��
		void onFileDelete(const Echo::String& file);

		// �ļ�Ŀ¼�޸�
		void onDirChanged(const QString& dir);

	public:
		QFileSystemWatcher*		m_fileSystemWatcher;
		FileMapping				m_filesInfo;
		Echo::vector<Echo::String>::type m_ignoreFileNames;
	};
}