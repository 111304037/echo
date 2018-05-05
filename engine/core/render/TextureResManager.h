#pragma once

#include <engine/core/render/render/Texture.h>
#include <Engine/core/Resource/ResourceManager.h>
#include "Engine/core/Render/TextureRes.h"

namespace Echo
{
	/**
	 * ���������
	 */
	class TextureRes;
	class TextureResManager : public ResourceManager
	{
	public:
		virtual ~TextureResManager();

		// instance
		static TextureResManager* instance();

		// ��������
		virtual TextureRes* createTexture(const String& name, Dword usage = Texture::TU_STATIC);
		
		// �ֶ�����
		virtual TextureRes* createManual(const String& name, Texture::TexType texType, PixelFormat format, Dword usage, ui32 width, ui32 height, ui32 depth, int num_mips, const Buffer& buff);
		
		// ������������ͼ
		virtual TextureRes* createTextureCubeFromFiles( const String& x_posi_name,const String& x_nega_name,const String& y_posi_name, const String& y_nage_name,const String& z_posi_name,const String& z_nega_name,Dword usage = Texture::TU_STATIC);
	
		// ��������ʵ��
		Resource* createImpl(const String& name, bool isManual);

	private:
		TextureResManager();
	};
}