#pragma once

#include <interface/texture.h>

namespace Echo
{
	class GLESTextureCube: public Texture
	{
		friend class GLES2Renderer;
	public:
		// ������������
		virtual	bool updateSubTex2D(ui32 level, const Rect& rect, void* pData, ui32 size);

	protected:
		GLESTextureCube(const String& name);
		GLESTextureCube(TexType texType, PixelFormat pixFmt, Dword usage, ui32 width, ui32 height, ui32 depth, ui32 numMipmaps, const Buffer& buff, bool bBak = true);
		virtual ~GLESTextureCube();

		virtual bool create2D(PixelFormat pixFmt, Dword usage, ui32 width, ui32 height, ui32 numMipmaps, const Buffer& buff);

		// ������������ͼ
		virtual bool createCube(PixelFormat pixFmt, Dword usage, ui32 width, ui32 height, ui32 numMipmaps, const Buffer& buff);

		// �����ڴ����ݵ�GPU
		virtual bool loadToGPU();

		// ���Դ�ж��
		virtual void unloadFromGPU();

	public:
		GLuint		m_hTexture;
		bool		m_isUploadGPU;
	};
}
