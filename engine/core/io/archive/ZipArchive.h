#pragma once

#include <engine/core/io/stream/MemoryDataStream.h>
#include "Archive.h"
#include "ResourcePack.h"

namespace Echo
{
	class ZipArchive : public Archive 
	{
	public:
		ZipArchive(const String& strName, const String& strArchType);
		virtual ~ZipArchive();

		// �Ƿ��Сд����
		bool isCaseSensitive(void) const;

		// ����
		void load();

		// ж��
		void unload();

		// ���ļ�
		DataStream* open(const String& strFilename);

		// ���������ļ�
		StringArray list(bool bRecursive = true, bool bDirs = false);

		// �г��ļ���Ϣ
		FileInfoList* listFileInfo(bool bDirs = false);

		// �Ƿ��Ѵ���
		bool exists(const String& strFilename);

		// �Ƿ��첽
		virtual bool isAsync(const char* fileName) { return false; }

		// �����첽���ػص�
		virtual bool addAsyncCompleteCallback(const Echo::String& name, EchoOpenResourceCb callback) { return false; }

	protected:
		ResourcePack		m_resourcePack;		// ��Դ��
		bool				mLoaded;			// ����״̬���Ƿ��Ѽ��سɹ�
		EE_MUTEX			(m_mutex)
	};

	/**
	 * ��Դ��������
	 */
	class ResourcePackDataStream : public MemoryDataStream
	{
	public:
		ResourcePackDataStream(const String& strName, void* pvMem, size_t uSize, bool bFreeOnClose = false, bool bReadOnly = false)
			:	MemoryDataStream(strName, pvMem, uSize, bFreeOnClose, bReadOnly) {}
	};
}