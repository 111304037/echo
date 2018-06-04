#include "7zipArchive.h"
#include <engine/core/Util/LogManager.h>
#include <engine/core/Util/PathUtil.h>
#include <engine/core/main/Engine.h>
#include <ThirdParty/7zip/7z.h>
#include <ThirdParty/7zip/7zFile.h>
#include <ThirdParty/7zip/7zCrc.h>
#include <ThirdParty/7zip/7zAlloc.h>

namespace Echo
{
	// 7zip����
	struct SzHandler
	{
		CSzArEx			db;
		CFileInStream	archiveStream;
		CLookToRead		lookStream;
		ISzAlloc		allocImp;
		ISzAlloc		allocTempImp;
	};


	void *EchoSzAlloc(void *p, size_t size)
	{
		if (size == 0)
			return 0;
		return EchoMalloc(size);
	}

	void EchoSzFree(void *p, void *address)
	{
		EchoSafeFree(address);
	}

	void *EchoSzAllocTemp(void *p, size_t size)
	{
		if (size == 0)
			return 0;
		return EchoMalloc(size);
	}

	void EchoSzFreeTemp(void *p, void *address)
	{
		EchoSafeFree(address);
	}

	// ���캯��
	SzArchive::SzArchive(const String& name, const String& archType)
		: Archive(name, archType)
	{
		m_name = Engine::instance()->getResPath() + name;
		m_7zipHandle = EchoNew(SzHandler);
	}

	//  �Ƿ��Сд����
	bool SzArchive::isCaseSensitive(void) const
	{
		return true;
	}

	// ��������
	SzArchive::~SzArchive()
	{
		unload();
	}

	// ����
	void SzArchive::load()
	{
		Echo::MutexLock lock(m_7zipMutexLock);

		SzHandler* sz = (SzHandler*)m_7zipHandle;

		// ������
		sz->allocImp.Alloc = SzAlloc;
		sz->allocImp.Free = SzFree;

		sz->allocTempImp.Alloc = SzAllocTemp;
		sz->allocTempImp.Free = SzFreeTemp;

		InFile_Open(&sz->archiveStream.file, m_name.c_str());

		FileInStream_CreateVTable(&sz->archiveStream);
		LookToRead_CreateVTable(&sz->lookStream, False);

		sz->lookStream.realStream = &sz->archiveStream.s;
		LookToRead_Init(&sz->lookStream);

		CrcGenerateTable();

		SzArEx_Init(&sz->db);
		if (SZ_OK == SzArEx_Open(&sz->db, &sz->lookStream.s, &sz->allocImp, &sz->allocTempImp))
		{
			for (UInt32 i = 0; i < sz->db.NumFiles; i++)
			{
				if (!SzArEx_IsDir(&sz->db, i))
				{
					UInt16 tempName[1024];
					SzArEx_GetFileNameUtf16(&sz->db, i, tempName);

					String fileName;// = PathUtil::GetPureFilename(ws2s((wchar_t*)tempName), true);
					StringUtil::LowerCase(fileName);
					if (m_files.find(fileName) == m_files.end())
						m_files[fileName] = i;
					else
						EchoLogError("file [%s] is not unique", fileName.c_str());
				}
			}
		}
	}

	// ж��
	void SzArchive::unload()
	{
		Echo::MutexLock lock(m_7zipMutexLock);

		m_files.clear();

		if (m_7zipHandle)
		{
			SzHandler* sz = (SzHandler*)m_7zipHandle;
			SzArEx_Free(&sz->db, &sz->allocImp);
			File_Close(&sz->archiveStream.file);
			EchoSafeDelete(sz, SzHandler);
			m_7zipHandle = nullptr;
		}
	}

	// ����Դ
	DataStream* SzArchive::open(const String& filename)
	{
		Echo::MutexLock lock(m_7zipMutexLock);

		auto it = m_files.find(filename);
		if (it != m_files.end())
		{
			SzHandler* sz = (SzHandler*)m_7zipHandle;
			int     fileIdx = it->second;
			UInt32  blockIndex = 0xFFFFFFFF;
			Byte*	outBuffer = nullptr;
			size_t	outBufferSize = 0;
			size_t  offset = 0;
			size_t  outSizeProcessed = 0;
			if (SZ_OK == SzArEx_Extract(&sz->db, &sz->lookStream.s, fileIdx, &blockIndex, &outBuffer, &outBufferSize, &offset, &outSizeProcessed, &sz->allocImp, &sz->allocTempImp))
			{
				size_t processedSize = outSizeProcessed;
				char* data = (char*)EchoMalloc(processedSize + 1);
				std::memcpy(data, outBuffer + offset, processedSize);
				data[processedSize] = '\0';
				IAlloc_Free(&sz->allocImp, outBuffer);

				DataStream* dataStream = EchoNew(MemoryDataStream(filename, data, processedSize, true, false));
				return dataStream;
			}
		}

		return nullptr;
	}

	// �г������ļ�
	StringArray SzArchive::list(bool recursive, bool dirs)
	{
 		StringArray strs; 
		strs.reserve(m_files.size());
 		for (auto it = m_files.begin(); it != m_files.end(); it++)
 		{
 			strs.push_back(it->first);
 		}

		return strs;
	}

	// ö�������ļ���Ϣ
	FileInfoList* SzArchive::listFileInfo(bool dirs)
	{
		return nullptr;
	}

	// �ж��ļ��Ƿ����
	bool SzArchive::exists(const String& filename)
	{
		auto it = m_files.find(filename);
		if (it != m_files.end())
			return true;

		return false;
	}

	// ��̬��������ѹ��Դ��
	void SzArchive::extractTo(const String& szip, const String& outPath)
	{
#ifdef ECHO_PLATFORM_WINDOWS
		SzHandler* sz = (SzHandler*)EchoNew(SzHandler);

		// ������
		sz->allocImp.Alloc = SzAlloc;
		sz->allocImp.Free = SzFree;

		sz->allocTempImp.Alloc = SzAllocTemp;
		sz->allocTempImp.Free = SzFreeTemp;

		InFile_Open(&sz->archiveStream.file, szip.c_str());

		FileInStream_CreateVTable(&sz->archiveStream);
		LookToRead_CreateVTable(&sz->lookStream, False);

		sz->lookStream.realStream = &sz->archiveStream.s;
		LookToRead_Init(&sz->lookStream);

		CrcGenerateTable();

		SzArEx_Init(&sz->db);
		if (SZ_OK == SzArEx_Open(&sz->db, &sz->lookStream.s, &sz->allocImp, &sz->allocTempImp))
		{
			for (UInt32 i = 0; i < sz->db.NumFiles; i++)
			{
				if (!SzArEx_IsDir(&sz->db, i))
				{
					UInt16 tempName[1024];
					SzArEx_GetFileNameUtf16(&sz->db, i, tempName);
					Echo::String fileName = StringUtil::WCS2MBS((wchar_t*)tempName);

					int     fileIdx = i;
					UInt32  blockIndex = 0xFFFFFFFF;
					Byte*	outBuffer = nullptr;
					size_t	outBufferSize = 0;
					size_t  offset = 0;
					size_t  outSizeProcessed = 0;
					if (SZ_OK == SzArEx_Extract(&sz->db, &sz->lookStream.s, fileIdx, &blockIndex, &outBuffer, &outBufferSize, &offset, &outSizeProcessed, &sz->allocImp, &sz->allocTempImp))
					{
						size_t processedSize = outSizeProcessed;
						char* data = (char*)EchoMalloc(processedSize + 1);
						std::memcpy(data, outBuffer + offset, processedSize);
						data[processedSize] = '\0';
						IAlloc_Free(&sz->allocImp, outBuffer);

						// create dir if not exist
						String savePath = outPath + fileName;
						String saveDir = PathUtil::GetFileDirPath(savePath);
						if (!PathUtil::IsFileExist(saveDir))
							PathUtil::CreateDir(saveDir);

						// save extracted data
						FILE* fileHandle = fopen( savePath.c_str(), "wb");
						if (fileHandle)
						{
							fwrite(data, processedSize, 1, fileHandle);

							fflush(fileHandle);
							fclose(fileHandle);
						}

						EchoSafeFree(data);
					}
				}
			}
		}

		SzArEx_Free(&sz->db, &sz->allocImp);
		File_Close(&sz->archiveStream.file);
		EchoSafeDelete(sz, SzHandler);
#endif
	}

	void SzArchive::updateArchive(const String& szip, const String& inPath)
	{
#ifdef ECHO_PLATFORM_WINDOWS
#endif
	}
}