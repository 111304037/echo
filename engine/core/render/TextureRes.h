#pragma once

#include "engine/core/resource/Res.h"
#include "engine/core/util/Buffer.h"
#include <engine/core/render/render/Texture.h>

namespace Echo
{
	/**
	 * �����װ
	 */
	class TextureRes: public Res
	{
	public:
		enum MASK
		{
			MASK_NONE = 0,
			MASK_ALPHA = 1,
			MASK_IMAGE = 2,
		};

	private:
		friend class RenderTarget;

	public:
		TextureRes(const String& name, bool isManual);
		virtual ~TextureRes();

		// ��������
		static TextureRes* createTexture(const String& name, Dword usage = Texture::TU_STATIC);

		// �ֶ�����
		static TextureRes* createManual(const String& name, Texture::TexType texType, PixelFormat format, Dword usage, ui32 width, ui32 height, ui32 depth, int num_mips, const Buffer& buff);

		// ������������ͼ
		static TextureRes* createTextureCubeFromFiles(const String& x_posi_name, const String& x_nega_name, const String& y_posi_name, const String& y_nage_name, const String& z_posi_name, const String& z_nega_name, Dword usage = Texture::TU_STATIC);

		// ������Դ
		void prepareLoad();

		// ��ȡ����
		Texture* getTexture() { return m_texture;  }

		// ��ȡ���ظ�ʽ
		PixelFormat getPixelFormat() const{ return m_texture->getPixelFormat(); }

		// ��ȡ���
		ui32 getWidth() const { return m_texture->getWidth(); }

		// ��ȡ�߶�
		ui32 getHeight() const{ return m_texture->getHeight(); }

		// �༭��ʹ��
		virtual bool getData(Byte*& data);

		// ������������
		bool updateSubTex2D(ui32 level, const Rect& rect, void* pData, ui32 size);

		// ���´�������
		bool reCreate2D(PixelFormat pixFmt, Dword usage, ui32 width, ui32 height, ui32 numMipmaps, const Buffer& buff);

		// ���ָ�����������
		void setPixelsByArea(ui32 inPosX,ui32 inPosY, ui32 outPosX, ui32 outPosY ,ui32 width, ui32 height,Texture* pTexture);

		// �����Ƿ���CPUͼƬ����
		void setRetainPreparedData(bool setting);

	protected:
		// ����ߴ�
		virtual size_t	calculateSize() const;

	protected:
		Texture*		m_texture;
	};
	typedef ResRef<TextureRes> TextureResPtr;
}
