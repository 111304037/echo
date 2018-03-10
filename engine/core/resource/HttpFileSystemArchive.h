#pragma once

#include <engine/core/resource/ArchiveFactory.h>
#include "engine/modules/net/NetConnectionManager.h"

#ifdef ECHO_USE_LIBEVENT
namespace Echo
{
	/**
	* �����ļ�ϵͳ�浵
	*/
	class HttpFileSystemArchive : public Archive
	{
	public:
		HttpFileSystemArchive(const String& name, const String& archType);
		~HttpFileSystemArchive();

		// �Ƿ��Сд����
		bool isCaseSensitive(void) const;

		// ����
		void load();

		// ж��
		void unload();

		// ���ļ�
		DataStream* open(const String& filename);

		// ��ȡȫ·��
		String location(const String& filename);

		// �г������ļ�
		StringArray list(bool recursive = true, bool dirs = false);

		// ��ȡ�ļ���Ϣ�б�
		FileInfoList* listFileInfo(bool dirs = false);

		// �ж��ļ��Ƿ����
		bool exists(const String& filename);

		// �Ƿ��������ļ�(���ļ����뵽����һ��FileSystemArchive�ж��Ǻ����,����ֱ�ӷ���true)
		virtual bool isCanAddFile(const String& fullPath) { return true; }

		// ����ļ�
		virtual bool addFile(const String& fullPath);

		// �Ƴ��ļ�
		virtual bool removeFile(const char* fileName);

		// �Ƿ��첽
		virtual bool isAsync(const char* fileName) { return true; }

		// �����첽�ص�
		virtual bool addAsyncCompleteCallback(const Echo::String& name, EchoOpenResourceCb callback); 

	public: 
		// ��Դ���سɹ��ص�
		void onOpenFileComplete(const Echo::String& name, void* data, size_t size, NetErrCode::ErrCode err); 

	private:
		typedef map<String, vector<EchoOpenResourceCb>::type >::type ResourceCallbackMapping; 

		String						m_url;			// ��������ַ
		set<String>::type			m_files;		// �����ļ�(�ļ����ƣ��ļ�·��)
		ResourceCallbackMapping		m_callbacks;	// 
	};

	/**
	 * �浵������
	 */
	class HttpFileSystemArchiveFactory : public ArchiveFactory
	{
	public:
		virtual ~HttpFileSystemArchiveFactory() {}

		// ��ȡ����
		const String& getType(void) const;

		// ����ʵ��
		Archive* createInstance(const String& name) { return EchoNew(HttpFileSystemArchive(name, "http")); }

		// ����ʵ��
		void destroyInstance(Archive* arch) { EchoDelete(arch); }
	};
}
#else
namespace Echo
{
	class HttpFileSystemArchiveFactory : public ArchiveFactory
	{
	public:
		virtual ~HttpFileSystemArchiveFactory() {}

		// ��ȡ����
		const String& getType(void) const
		{
			static String name = "http";
			return name;
		}

		virtual Archive* createInstance(const String& name) { return nullptr; }
		virtual void destroyInstance(Archive*) {}
	};
}
#endif
