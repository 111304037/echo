#pragma once

#include "engine/core/memory/MemAllocDef.h"

namespace Echo
{
	#define ECHO_STREAM_TEMP_SIZE 128

	class DataStream
	{
	public:
		enum AccessMode
		{
			READ = 1, 
			WRITE = 2
		};

	public:
		DataStream(ui16 accessMode = READ) : mSize(0), m_buffer(nullptr), mAccess(accessMode) {}
		DataStream(const String& name, ui16 accessMode = READ) : mName(name), mSize(0), m_buffer(nullptr), mAccess(accessMode) {}
		virtual ~DataStream();

		const String& getName(void) { return mName; }
		ui16 getAccessMode() const { return mAccess; }
		virtual bool isReadable() const { return (mAccess & READ) != 0; }
		virtual bool isWriteable() const { return (mAccess & WRITE) != 0; }
        virtual bool isMemoryDS() const { return false; }
		template<typename T> DataStream& operator>>(T& val);
		virtual size_t read(void* buf, size_t count) = 0;
		virtual size_t write(const void* buf, size_t count);

		virtual size_t readLine(char* buf, size_t maxCount, const String& delim = "\n");

		virtual String getLine( bool trimAfter = true );

		virtual String getAsString(void);

		virtual size_t skipLine(const String& delim = "\n");

		virtual void skip(long count) = 0;

		virtual void seek(size_t pos, int origin=SEEK_SET) = 0;

		virtual size_t tell(void) const = 0;

		virtual bool eof(void) const = 0;

		size_t size(void) const { return mSize; }

		// ��ȡ������
		virtual const void* getBuffer();

		virtual void close(void) = 0;

        void readAll( void* buffer );

	protected:
		String	mName;
		size_t	mSize;
		char*	m_buffer;			// ����
		ui16	mAccess;
	};

	/**
	 * �ڴ���
	 */
	class MemoryDataStream : public DataStream
	{		
	public:
		MemoryDataStream(void* pMem, size_t size, bool freeOnClose = false, bool readOnly = false);

		MemoryDataStream(const String& name, void* pMem, size_t size, 
			bool freeOnClose = false, bool readOnly = false);

		MemoryDataStream(DataStream& sourceStream, 
			bool freeOnClose = true, bool readOnly = false);

		MemoryDataStream(DataStream* sourceStream, 
			bool freeOnClose = true, bool readOnly = false);

		MemoryDataStream(const String& name, DataStream& sourceStream, 
			bool freeOnClose = true, bool readOnly = false);

		MemoryDataStream(const String& name, DataStream* sourceStream, 
			bool freeOnClose = true, bool readOnly = false);

		MemoryDataStream(size_t size, bool freeOnClose = true, bool readOnly = false);

		MemoryDataStream(const String& name, size_t size, 
			bool freeOnClose = true, bool readOnly = false);

		~MemoryDataStream();

		// ��ȡ������
		virtual const void* getBuffer() { return mData; }

		ui8* getPtr(void) { return mData; }

		ui8* getCurrentPtr(void) { return mPos; }

		size_t read(void* buf, size_t count);

		size_t write(const void* buf, size_t count);

		size_t readLine(char* buf, size_t maxCount, const String& delim = "\n");

		size_t skipLine(const String& delim = "\n");

		void skip(long count);

		void seek(size_t pos, int origin = SEEK_SET);

		size_t tell(void) const;

		bool eof(void) const;

		void close(void);

		void setFreeOnClose(bool free) { mFreeOnClose = free; }

        virtual bool isMemoryDS() const { return true; }

	protected:
		ui8*	mData;
		ui8*	mPos;
		ui8*	mEnd;
		bool	mFreeOnClose;
	};

	class FileStreamDataStream : public DataStream
	{
	public:

		FileStreamDataStream(std::ifstream* s, 
			bool freeOnClose = true);

		FileStreamDataStream(std::fstream* s, 
			bool freeOnClose = true);

		FileStreamDataStream(const String& name, 
			std::ifstream* s, 
			bool freeOnClose = true);

		FileStreamDataStream(const String& name, 
			std::fstream* s, 
			bool freeOnClose = true);

		FileStreamDataStream(const String& name, 
			std::ifstream* s, 
			size_t size, 
			bool freeOnClose = true);

		FileStreamDataStream(const String& name, 
			std::fstream* s, 
			size_t size, 
			bool freeOnClose = true);

		~FileStreamDataStream();

		size_t read(void* buf, size_t count);

		size_t write(const void* buf, size_t count);

		size_t readLine(char* buf, size_t maxCount, const String& delim = "\n");

		void skip(long count);

		void seek(size_t pos, int origin = SEEK_SET);

		size_t tell(void) const;

		bool eof(void) const;

		void close(void);

	protected:
		void determineAccess();

	protected:
		std::istream*	mpInStream;
		std::ifstream*	mpFStreamRO;
		std::fstream*	mpFStream;
		bool			mFreeOnClose;	
	};

	/**
	 * �ļ����
	 */
	class FileHandleDataStream : public DataStream
	{
	protected:
		FILE* mFileHandle;
	public:
		FileHandleDataStream(const String& name, ui16 accessMode = READ);
		~FileHandleDataStream();

		size_t read(void* buf, size_t count);

		size_t write(const void* buf, size_t count);

		void skip(long count);

		void seek(size_t pos, int origin = SEEK_SET);

		size_t tell(void) const;

		bool eof(void) const;

		void close(void);

		// �Ƿ�ʧ��
		bool fail() { return mFileHandle ? false : true; }
	};

	/**
	 * �ڴ��ȡ��(��ȡ�ļ����ڴ�,�Զ��ͷ�)
	 */ 
	class MemoryReader
	{
	public:
		MemoryReader( const String& file);
		MemoryReader(const char* data, ui32 size);
		~MemoryReader();

		// ��ȡ����ָ��
		template <typename T> T getData() { return reinterpret_cast<T>(m_data); }

		// ��ȡ��С
		ui32 getSize() { return m_size; }

	private:
		char*	m_data;		// ����
		ui32	m_size;		// ���ݴ�С
	};

	/**
	* �ڴ��ȡ��(��ȡ�ļ����ڴ�,�ڴ����)
	*/
	class MemoryReaderAlign
	{
	public:
		MemoryReaderAlign(const String& file, int align=0);
		~MemoryReaderAlign();

		// ��ȡ����ָ��
		template <typename T> T getData() { return static_cast<T>(m_dataAlign); }

		// ��ȡ��С
		ui32 getSize() { return m_size; }

	private:
		char*	m_dataAlign;// ����
		char*	m_data;		// ����
		ui32	m_size;		// ���ݴ�С
	};
}
