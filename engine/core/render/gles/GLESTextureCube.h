#pragma once

#include <interface/TextureCube.h>

namespace Echo
{
	class GLESTextureCube: public TextureCube
	{
		friend class GLES2Renderer;
	public:
		// ������������
		virtual	bool updateSubTex2D(ui32 level, const Rect& rect, void* pData, ui32 size);

	protected:
		GLESTextureCube(const String& name);
		GLESTextureCube(TexType texType, PixelFormat pixFmt, Dword usage, ui32 width, ui32 height, ui32 depth, ui32 numMipmaps, const Buffer& buff, bool bBak = true);
		virtual ~GLESTextureCube();

		// create cube
		virtual bool createCube(PixelFormat pixFmt, Dword usage, ui32 width, ui32 height, ui32 numMipmaps, const Buffer& buff);

		// ���Դ�ж��
		virtual bool unload();

	public:
		GLuint		m_hTexture;
	};
}
