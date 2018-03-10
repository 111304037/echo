#ifndef __FileSystem_H__
#define __FileSystem_H__

#include "engine/core/resource/Archive.h"
#include "engine/core/resource/ArchiveFactory.h"

namespace Echo
{
	/**
	 * �ļ�ϵͳ�浵
	 */
	class FileSystemArchive : public Archive 
	{
	public:
		FileSystemArchive(const String& name, const String& archType );
		~FileSystemArchive();

		bool isCaseSensitive(void) const;

		void load();

		void unload();

		DataStream* open(const String& filename);

		String location(const String& filename);

		StringArray list(bool recursive = true, bool dirs = false);

		FileInfoList* listFileInfo(bool dirs = false);

		bool exists(const String& filename);

		// �Ƿ��������ļ�(���ļ����뵽����һ��FileSystemArchive�ж��Ǻ����,����ֱ�ӷ���true)
		virtual bool isCanAddFile(const String& fullPath) { return true; }

		// ����ļ�
		virtual bool addFile(const String& fullPath);

		// �Ƴ��ļ�
		virtual bool removeFile( const char* fileName);

		// �Ƿ��첽
		virtual bool isAsync(const char* fileName) { return false; }

		// �����첽���ػص�
		virtual bool addAsyncCompleteCallback(const Echo::String& name, EchoOpenResourceCb callback) { return false; }

	private:
		map<String, String>::type		m_files;		// �����ļ�(�ļ����ƣ��ļ�·��)
	};

	class FileSystemArchiveFactory : public ArchiveFactory
	{
	public:
		virtual ~FileSystemArchiveFactory() {}

		const String& getType(void) const;

		Archive* createInstance(const String& name) 
		{
			return EchoNew(FileSystemArchive(name, "filesystem"));
		}

		void destroyInstance(Archive* arch) { EchoSafeDelete(arch, Archive); }
	};
}

#endif
