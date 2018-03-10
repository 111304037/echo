#ifndef __THUMBNAIL_MANAGER_H__
#define	__THUMBNAIL_MANAGER_H__

#include <Engine/Core.h>
#include <engine/core/Util/StringUtil.h>

namespace Studio
{
	/**
	 * ����ͼ������
	 */
	class ThumbnailMgr : public Echo::Singleton<ThumbnailMgr>
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

		// ��������ͼ
		bool saveThumbnail( const Echo::String& fileName, THUMBNAIL_TYPE type = THUMB_NON);

		// ��������ͼ
		static void update();
	};
}

#endif

