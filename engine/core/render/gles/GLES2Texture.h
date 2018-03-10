#ifndef __ECHO_GLES2TEXTURE_H__
#define __ECHO_GLES2TEXTURE_H__

#include <Render/Texture.h>

namespace Echo
{
	class GLES2TextureGPUProxy;
	
	class ECHO_GLES2RENDER_API GLES2Texture: public Texture
	{
		friend class GLES2Renderer;
	public:
		// ������������
		virtual	bool updateSubTex2D(ui32 level, const Rect& rect, void* pData, ui32 size);

	protected:
		GLES2Texture(const String& name);
		GLES2Texture(TexType texType, PixelFormat pixFmt, Dword usage, ui32 width, ui32 height, ui32 depth, ui32 numMipmaps, const Buffer& buff, bool bBak = true);
		virtual ~GLES2Texture();

		virtual bool create2D(PixelFormat pixFmt, Dword usage, ui32 width, ui32 height, ui32 numMipmaps, const Buffer& buff);

		// ������������ͼ
		virtual bool createCube(PixelFormat pixFmt, Dword usage, ui32 width, ui32 height, ui32 numMipmaps, const Buffer& buff);

		// �����ڴ����ݵ�GPU
		virtual bool loadToGPU();

		virtual void unloadFromMemory();

		// ���Դ�ж��
		virtual void unloadFromGPU();

		bool _upload_dds();
		bool _upload_pvr(ui8* pPreparedData, i32* oTextureSize=NULL);
		bool _upload_ktx();
		bool _upload_etc_aa_ktx();

	public:
#ifdef ECHO_RENDER_THREAD
		GLES2TextureGPUProxy* m_gpu_proxy;
#else
		GLuint				m_hTexture;
#endif

	private:
		bool	m_isUploadGPU;
	};
}

#endif
