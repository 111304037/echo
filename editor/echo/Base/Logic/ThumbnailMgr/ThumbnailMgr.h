#pragma once

#include <Engine/Core.h>
#include <engine/core/Util/StringUtil.h>

namespace Studio
{
	/**
	 * ����ͼ������
	 */
	class ThumbnailMgr
	{
	public:
		// ����ͼ����
		enum THUMBNAIL_TYPE
		{
			THUMB_NON,
			THUMB_SCENE,
			THUMB_MODEL,
			THUMB_EFFECT,
			THUMB_ACTOR,
			THUMB_ATT,
			THUMB_NUM,
		};

	public:
		ThumbnailMgr();
		virtual ~ThumbnailMgr();

		// instance
		static ThumbnailMgr* instance();

		// ��������ͼ
		bool saveThumbnail( const Echo::String& fileName, THUMBNAIL_TYPE type = THUMB_NON);

		// ��������ͼ
		static void update();
	};
}
