#pragma once

#include "Engine/core/Resource/Resource.h"
#include "engine/core/Util/Buffer.h"
#include <engine/core/render/render/Texture.h>

namespace Echo
{
	/**
	 * �����װ
	 */
	class TextureRes: public Resource
	{
	public:
		enum MASK
		{
			MASK_NONE = 0,
			MASK_ALPHA = 1,
			MASK_IMAGE = 2,
		};

	private:
		friend struct commonTextureUploadFunc;
		friend class RenderTarget;
		friend class TextureResManager;
		friend struct TexturePrepareFunc;
		typedef vector<Buffer>::type TexArray;

	public:
		TextureRes(const String& name, bool isManual);
		virtual ~TextureRes();

		// ��ȡ����
		Texture* getTexture() { return m_texture;  }

		void setTexture(Texture* texture) { m_texture = texture; }

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

		// ׼��ʵ��
		virtual bool prepareImpl( DataStream* stream);

		// ж��ʵ�� 
		virtual void unprepareImpl();

		// ����
		virtual bool loadImpl();

		// ж��
		virtual void unloadImpl();

	protected:
		Texture*		m_texture;
	};
}
