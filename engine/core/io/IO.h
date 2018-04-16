#pragma once

#include <functional>
#include "engine/core/Thread/Threading.h"
#include "DataStream.h"
#include "archive/Archive.h"
#include "engine/core/util/Singleton.h"
#include "FileSystem.h"

namespace Echo
{
	/**
	 * ��Դ������ 
	 */
	class IO
	{
		__DeclareSingleton(IO);

	public:
		EE_AUTO_MUTEX

		typedef list<Archive*>::type				 ArchiveList;
		typedef map<String, Archive*>::type			 FileArchiveMapping;
	public:
		IO();
		virtual ~IO();

		// ����������Դ·��
		void setResPath(const String& resPath);

		// �����û���Դ·��
		void setUserPath(const String& userPath);

		// ����Դ
		DataStream* open(const String& resourceName);

		// ����
		void reset();

		// �����Դ�浵
		void addArchive(const String& name, const String& locType, bool isOverWrite = false);

		// �Ƴ���Դ�浵
		void removeArchive(const String& name);

		// �첽����Դ
		void openResourceAsync(const String& resourceName, EchoOpenResourceCb callback);

		// �Ƿ�Ϊ�첽��Դ
		bool isAsync( const char* resouceName);

		// �ж���Դ�Ƿ����
		bool isResourceExists(const String& filename);

		// ��ȡ�ļ�ȫ·��(��Ӧ�ڴ浵����)
		String getFullPath(const String& filename);

		// �������ȡ������Դ�浵
		const ArchiveList& getArchiveList() { return m_archives; }

		// ��ȡָ����׺���������ļ�(.bmp|.tga)
		void listFilesWithExt( StringArray& oFiles, const char* extWithDot);
		void foreach_archive_unsafe(std::function<bool (const Echo::String&, const int)> callback);
		int getArchiveCount() const { return m_resourceIndexCaseSensitive.size(); }

		// ��ȡָ��Ŀ¼��ָ����׺����Դ�浵
		void listFileWithPathAndExt(StringArray& oFiles, const char* path, const char* extWithDot);

		// ����ļ�
		void addFile(const String& archiveType, const String& fullPath);

		// �Ƴ��ļ�
		void removeFile( const String& fileName);

		// ��ȡ�ļ����ڵĴ浵
		Archive* getArchiveByFileName( const char* fileName);

		// ���ݴ浵���ƻ�ȡ�浵
		Archive* getArchiveByName( const char* archiveName);

	private:
		// ��ӵ�����
		void addToIndex(const String& filename, Archive* arch, bool isOverWrite=false);

		// �Ƴ�������
		void removeFromIndex( const String& filename);

		// ��ȡ�ļ����ڵĴ浵
		Archive* FindFileArchive(const String& filename);

	protected:
		FileSystem*					m_resFileSystem;					// ������Դ����("Res://")
		FileSystem*					m_userFileSystem;					// �û���Դ����("User://")
		ArchiveList					m_archives;							// �浵�б�
		FileArchiveMapping			m_resourceIndexCaseSensitive;		// �ļ�����浵ӳ���ϵ
	};
}
