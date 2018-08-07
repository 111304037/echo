#pragma once

#include <engine/core/io/DataStream.h>
#include "engine/core/thread/Threading.h"

#define MAX_FILE_NODE_NUM 20000
#define MIN_FILE_NODE_NUM 50			

#define MAX_MEMORY_CACHE 1024*1024

namespace Echo
{
	/**
	 * ��Դ��
	 */
	class ResourcePack
	{
	public:
		// �ļ�ʹ��״̬
		enum FILE_USING_TYPE
		{
			FILE_NOT_USING = 0,
			FILE_IN_USING,
			FILE_DELETE
		};

		// ѹ������
		enum FILE_COMPRESS_TYPE
		{
			CT_NONE = 0,		// ��ѹ��
			CT_ZLIB				// ZLIBѹ��
		};

		// �ļ��ڵ�
		struct FileNode
		{
			char			m_sFileName[64];
			ui32			m_nOffset;
			ui32			m_nFileRealSize;
			ui32			m_nFileCompressSize;
			ui8				m_nCompressType;
			ui8				m_nUseFlag;

			FileNode()
			{
				memset(m_sFileName, 0, sizeof(m_sFileName));
				m_nUseFlag = FILE_NOT_USING;
				m_nOffset = 0;
				m_nFileRealSize = 0;
				m_nFileCompressSize = 0;
				m_nCompressType = 0;
			}
		};
		typedef map<String, int>::type FileNodeMap;

	public:
		ResourcePack();
		~ResourcePack();

		// ���ļ�
		bool openFile(const char* lpszArchiveFile, ui16 access = DataStream::READ);

		// �ر��ļ�����дͷ
		bool closeFile();
		
		// ��ȡ�汾��
		const char* getVersion();

		// ��ȡ�ļ�
		const unsigned char* readFile(const char* lpszFileName, unsigned int& nOutLength) ;

		void ReleaseExtraMemory();

		bool CopyFileList(const FileNode* pFileNode, int nFileNodeCount);

		bool CopyFileCompressData(const char* lpszArchiveFile, unsigned long nOffset);

		const FileNode* GetFileList(int& nFileListCount);

		const char* getError();

		// �ļ��Ƿ����
		bool isFileExist(const char* lpszFileName);

		// �����ļ�
		bool createFile(const char *lpszArchiveFile, int nFileNodeNum = 512);
		bool addFile(const char *lpszFilePath, const char *lpszFileName, int nCompressType);
		bool addFile(const char *lpszFileName, const FileNode* pFileNode, const char *pData, int nSize);
		bool DelFile(const char *lpszFileName);
		bool SetVersion(const char* szVersion);
		bool SetArchivePath( const char* apszArchivePath );
		bool GetFileCompressData( const char* aszFileName, char* aszOutPut );
	
	private:
		int GetEmptyFileNode();
		bool RebuildPacket();
	
	public:
		bool CompressData(FileNode& fileNode, const char* filePath, unsigned int& nCompressSize);
		int	Zlib_Compress(unsigned char* dest,unsigned int* destLen,const unsigned char* source,unsigned int sourceLen);

	private:
		int GetFileNodeByName(const char* lpszFileName);

		int GetFileNodeByName_fast(const char* lpszFileName);

		void ResetMember();

		void FreeMemory();

		bool UnCompressData(const FileNode& fileNode, unsigned int& nRealSize);

		int Zlib_UnCompress(unsigned char* dest,unsigned int* destLen,const unsigned char* source,unsigned int sourceLen);
	
	private:
		String				m_error;						// ������Ϣ
		unsigned char*		m_RealFileBuffer;				// �ļ�����(��ѹ)
		ui32				m_RealFileBufferSize;			// �ļ������С(��ѹ)
		unsigned char*		m_CompressBuffer;				// �ļ�����(ѹ��)
		ui32				m_CompressBufferSize;			// �ļ������С

		unsigned char*		m_UnCompressBuffer;				
		ui32				m_UnCompressBufferSize;		
		unsigned char*		m_OutBuffer;					
		ui32				m_OutBufferSize;

	private:
		FileHandleDataStream*	m_fileHandle;					// �ļ����
		ui32					m_nFileListOffset;				// m_pFileNode����ƫ����	
		char					m_sFileVersion[64];				// ����ļ��汾��
		FileNode*				m_pFileNode;					// �ļ��������
		i32						m_nFilesListNum;				// �ļ�����
		FileNodeMap				m_FileNodeMap;					// �ļ�Ŀ¼(�����ļ�����ѯ)
		String					m_sArchiveFileName;				// �ļ��洢·��
		bool					m_bArchiveFileChange;
		String					m_sArchiveFilePath;				// ��ǰ�浵�ļ�·��

		EE_MUTEX(m_UnpressLock);
	};
}
