#pragma once

#include <engine/core/Thread/Threading.h>
#include "ArchiveFactory.h"

namespace Echo
{
	/**
	* 7zipArchive(Sevenzip Sz)
	*/
	class SzArchive : public Archive
	{
	public:
		SzArchive(const String& name, const String& archType);
		~SzArchive();

		// �Ƿ��Сд����
		bool isCaseSensitive(void) const;

		// ����
		void load();

		// ж��
		void unload();

		// ���ļ�
		DataStream* open(const String& filename);

		// �г������ļ�
		StringArray list(bool recursive = true, bool dirs = false);

		// ��ȡ�ļ���Ϣ�б�
		FileInfoList* listFileInfo(bool dirs = false);

		// �ж��ļ��Ƿ����
		bool exists(const String& filename);

		// �Ƿ��첽
		virtual bool isAsync(const char* fileName) { return false; }

		// �����첽�ص�
		virtual bool addAsyncCompleteCallback(const Echo::String& name, EchoOpenResourceCb callback) { return false; }

	public:
		// ��̬��������ѹ��Դ��
		static void extractTo(const String& szip, const String& outPath);

		// ��̬������ѹ����Դ��
		static void updateArchive(const String& szip, const String& inPath);
	private:
		String						m_name;			// ����
		map<String, int>::type		m_files;		// �ļ���������ӳ��
		void*						m_7zipHandle;	// 7zip����
		Echo::Mutex					m_7zipMutexLock;// �߳���
	};

	/**
	* �浵������
	*/
	class SzArchiveFactory : public ArchiveFactory
	{
	public:
		virtual ~SzArchiveFactory() {}

		// ��ȡ����
		const String& getType(void) const;

		// ����ʵ��
		Archive* createInstance(const String& name) { return EchoNew(SzArchive(name, "7zip")); }

		// ����ʵ��
		void destroyInstance(Archive* arch) { EchoSafeDelete(arch, Archive); }
	};
}
