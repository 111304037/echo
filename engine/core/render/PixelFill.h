#ifndef __ECHO_PIXELFILL_H__
#define __ECHO_PIXELFILL_H__

#include <engine/core/Math/EchoMath.h>
#include "engine/core/render/render/RenderDef.h"
#include "Engine/core/Render/TextureRes.h"

namespace Echo
{
	/************************************************************************/
	/* ��ͼ������䣬ͨ��һ�������ļ�����                                   */
	/************************************************************************/
	class PixeFill
	{
	public:
		// Ԫ�ؽṹ��
		struct PixeFillElement
		{
			String	id;
			Vector4	inArea;
			Vector2 outPoint;
			String	inTexture;

			PixeFillElement()
				: id(StringUtil::BLANK)
				, inArea(0.f,0.f,0.f,0.f)
				, outPoint(Vector2::ZERO)
				, inTexture(StringUtil::BLANK)
			{

			}
		};

		// map����
		typedef map<String,PixeFillElement*>::type	PixeFillElementMap;
		typedef PixeFillElementMap::iterator		PixeFillElementMapIt;

		typedef map<String,TextureRes*>::type		TextureMap;
		typedef TextureMap::iterator				TextureMapIt;

	public:
		PixeFill(TextureRes* baseTexture);
		~PixeFill();

	public:
		// �����ļ��ģɣ�
		bool LoadCFGFile(const String& cfgFile);
		void SaveCFGFile(const String& fileName);

		// ͨ��ID���
		void PixeFillByID(const String& id);

	protected:
		// ������ͼ
		void loadTexture(const String& fileName);
		void freeTextures();

	private:
		PixeFillElementMap mElementMap;	  // Ԫ��MAP
		TextureMap         mTextureMap;	  // ��ͼMAP
		TextureRes*        mpBaseTexture; // ��ͼ
	};
}

#endif