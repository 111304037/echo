#pragma once

#include <engine/core/Util/Buffer.h>
#include <engine/core/Math/Rect.h>
#include <engine/core/io/DataStream.h>
#include "PixelFormat.h"

namespace Echo
{
	/**
	 * �����װ
	 */
	class SamplerState;
	class ECHO_EXPORT_RENDER Texture
	{
		friend class Renderer;
		friend class RenderTarget;
		typedef vector<Buffer>::type		TexArray;

	public:
		// ��������
		enum TexType
		{
			TT_1D,
			TT_2D,
			TT_3D,
			TT_CUBE,
		};

		// ��������ͼ��
		enum CubeFace
		{
			CF_Positive_X,
			CF_Negative_X,
			CF_Positive_Y,
			CF_Negative_Y,
			CF_Positive_Z,
			CF_Negative_Z,
			CF_End
		};

		// Usage
		enum TexUsage
		{
			TU_CPU_READ			= 0x00000001,
			TU_CPU_WRITE		= 0x00000002,
			TU_GPU_READ			= 0x00000004,	// texture is used as shader resource	
			TU_GPU_WRITE		= 0x00000008,	// texture is used as depth or render target (depend on pixel format)
			TU_GPU_UNORDERED	= 0x00000010,
			TU_GPU_MIPMAP		= 0x00000020,		
			TU_DEFAULT			= TU_GPU_READ | TU_GPU_WRITE | TU_GPU_MIPMAP,		// default to automatic mipmap generation static textures
			TU_STATIC			= TU_GPU_READ | TU_GPU_MIPMAP,
			TU_DYNAMIC			= TU_GPU_READ | TU_CPU_WRITE | TU_GPU_MIPMAP,
		};

		// ѹ������
		enum CompressType
		{
			CompressType_ETC1 = 0,
			CompressType_ETCA,
			CompressType_PVR,
			CompressType_S3TC,
			CompressType_ATITC,
			CompressType_KTX,
			CompressType_JPGA,
			CompressType_Unknown
		};

		static const ui32	MAX_MINMAPS = 16;

	public:
		// ��ȡ����
		TexType getType() const { return m_texType; }

		// ��ȡ����
		const String& getName() const { return m_name; }

		// ��ȡ���ظ�ʽ
		PixelFormat getPixelFormat() const{ return m_pixFmt; }

		// ��ȡ���
		ui32 getWidth() const { return m_width; }

		// ��ȡ�߶�
		ui32 getHeight() const{ return m_height; }

		// ��ȡ���
		ui32 getDepth() const { return m_depth; }

		// MipMaps
		ui32 getNumMipmaps() const { return m_numMipmaps; }

		// ��ȡ�û�����
		void* getUserData() const { return m_userData; }

		// �����û�����
		void setUserData(void* userData) { m_userData = userData; }

		// ������������
		virtual bool updateSubTex2D(ui32 level, const Rect& rect, void* pData, ui32 size) = 0;

		/// for editor.
		virtual bool getData(Byte*& data);

		// ���ָ�����������
		void setPixelsByArea(ui32 inPosX,ui32 inPosY, ui32 outPosX, ui32 outPosY ,ui32 width, ui32 height,Texture* pTexture);

		// �����Ƿ���CPUͼƬ����
		void setRetainPreparedData(bool setting){ m_isRetainPreparedData = setting;}

		// ���ò���״̬
		void setCurSamplerState(const SamplerState* pSs) { m_samplerState = pSs; }
		
		// ��ȡ����״̬
		const SamplerState* getCurSamplerState() const { return m_samplerState; }

		// ���´�������
		virtual bool reCreate2D(PixelFormat pixFmt, Dword usage, ui32 width, ui32 height, ui32 numMipmaps, const Buffer& buff);

		// ���������ڴ沢���ص�GPU
		bool load(DataStream* stream);

		// ж��
		bool unload();

		// ���ݲ�������2D����
		virtual bool create2D(PixelFormat pixFmt, Dword usage, ui32 width, ui32 height, ui32 numMipmaps, const Buffer& buff);

		// ����ߴ�
		virtual size_t	calculateSize() const;

		// ������Դ�첽���ػص�
		bool onLoadedToMemoryAsync(DataStream* dataStream);

	public:
		bool softDecodeETC2();

		bool decodeFromPVR();
		bool decodeFromKTX();

	public:
		// ���µ��Դ�
		virtual bool loadToGPU() = 0;

		// ж��
		virtual void unloadFromMemory() = 0;

	protected:
		Texture(const String& name);
		Texture(TexType texType, PixelFormat pixFmt, Dword usage, ui32 width, ui32 height, ui32 depth, ui32 numMipmaps, const Buffer& buff, bool bBak = true);
		virtual ~Texture();

		// ������������ͼ
		virtual bool createCube(PixelFormat pixFmt, Dword usage, ui32 width, ui32 height, ui32 numMipmaps, const Buffer& buff)=0;

		// IO
		bool _data_io(DataStream* dataStream);

		// ������������ͼ
		bool _data_io_cubemap_from_files();

		ui32 GetCompressType();
		void SetCompressType(ui32 val) { m_compressType = val; }

		// ����������Դ���ڴ�
		virtual bool loadToMemory(DataStream* stream);

		// ���Դ�ж��
		virtual void unloadFromGPU() = 0;

	protected:
		// ��Դ����
		bool _data_parser();
		bool _parser_pvr();
		bool _parser_etc_aa_ktx();
		bool _parser_dds();
		bool _parser_ktx();
		bool _parser_common();
		bool _parser_jpga();
		bool _data_parser_cubemap_from_files();

		bool _upload_common();
		bool _upload_pvr();

	public:
		void*			m_userData;				// �û�����
		TexType			m_texType;				// ��������
		String			m_name;					// ��ʶ
		PixelFormat		m_pixFmt;				// ���ظ�ʽ
		bool			m_bCompressed;			// �Ƿ�ѹ����ʽ
		ui32			m_compressType;			// ѹ������
		Dword			m_usage;				// ʹ��(��������洢λ��)
		ui32			m_width;				// ��
		ui32			m_height;				// ��
		ui32			m_depth;				// ���(3d����)
		ui32			m_numMipmaps;			// mipmaps
		ui32			m_fileSize;				// �ļ���С
		ui32			m_pixelsSize;			// �������ݴ�С
		Byte*			m_pPreparedData;
		bool			m_isRetainPreparedData; // �Ƿ���CPU��������
		bool			m_isLoadedToMemory;		// �Ƿ��Ѽ��ص��ڴ�
		ui32			m_faceNum;				// ����
		ui32			m_endian;
		ui32			m_bitsPerPixel;
		ui32			m_blockSize;
		ui32			m_xDimension;
		ui32			m_yDimension;
		ui32			m_zDimension;
		ui32			m_headerSize;
		ui32			m_uploadedSize;		// its actually uploaded size. uploadSize + headerSize + identifier size = m_size
		ui32			m_surfaceNum;
		const SamplerState*	m_samplerState;		// ����״̬

public:
		bool			m_bUploadFromFiles;
		String			m_surfaceFilename[CF_End];
	};

	/**
	 * ���������״̬���
	 */
	struct TextureSampler
	{
		Texture*			m_texture;			// ����
		const SamplerState*	m_samplerState;		// ����״̬

		// ���캯��
		TextureSampler()
			: m_texture(NULL), m_samplerState(NULL)
		{}

		// ���캯��
		TextureSampler(Texture* texture, const SamplerState* samplerState);
	};
}
