#include "render/Texture.h"
#include "render/Renderer.h"
#include "engine/core/Util/LogManager.h"
#include "engine/core/Util/Exception.h"
#include "TextureResManager.h"
#include "render/PixelFormat.h"
#include "engine/core/Math/EchoMathFunction.h"

namespace Echo
{
	// ���캯��
	TextureRes::TextureRes(const String& name, bool isManual)
		: Resource(name, RT_Texture, isManual)
	{
		m_texture = Renderer::instance()->createTexture(name);
		m_texture->setUserData(this);
	}

	// ��������
	TextureRes::~TextureRes()
	{
		if (m_texture)
		{
			Renderer::instance()->releaseTexture(m_texture);
		}
	}

	// ���������С
	size_t TextureRes::calculateSize() const
	{
		// need repaird
		return  m_texture->calculateSize();
	}

	// ������������
	bool TextureRes::updateSubTex2D(ui32 level, const Rect& rect, void* pData, ui32 size)
	{
		return m_texture->updateSubTex2D(level, rect, pData, size);
	}

	// ��ȡ��������
	bool TextureRes::getData(Byte*& data)
	{
		return m_texture->getData(data);
	}

	// �ؽ�����
	bool TextureRes::reCreate2D(PixelFormat pixFmt, Dword usage, ui32 width, ui32 height, ui32 numMipmaps, const Buffer& buff)
	{
		m_texture->reCreate2D(pixFmt, usage, width, height, numMipmaps, buff);

		return true;
	}

	bool TextureRes::prepareImpl( DataStream* stream)
	{
		return m_texture->load( stream);
	}

	bool TextureRes::loadImpl()
	{
		return m_texture->loadToGPU();
	}

	// �����Ƿ���CPUͼƬ����
	void TextureRes::setRetainPreparedData(bool setting)
	{
		m_texture->setRetainPreparedData(setting);
	}

	void TextureRes::unprepareImpl()
	{	
		//m_texture->unloadFromMemory();
	}

	// ж��
	void TextureRes::unloadImpl()
	{	
		m_texture->unload();
	}

	void TextureRes::setPixelsByArea(ui32 inBeginX, ui32 inBeginY, ui32 inEndX, ui32 inEndY, ui32 outPosX, ui32 outPosY, Texture* pTexture)
	{
		m_texture->setPixelsByArea(inBeginX, inBeginY, inEndX, inEndY, outPosX, outPosY, pTexture);
	}
}

