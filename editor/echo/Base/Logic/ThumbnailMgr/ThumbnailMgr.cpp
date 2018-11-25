#include "ThumbnailMgr.h"
#include "FreeImageHelper.h"
#include <engine/core/render/interface/RenderTargetManager.h>
#include <engine/core/util/StringUtil.h>
#include "Studio.h"

#ifdef ECHO_PLATFORM_WINDOWS
#include <shellapi.h>
#endif

namespace Studio
{
	ThumbnailMgr::ThumbnailMgr()
	{}

	ThumbnailMgr::~ThumbnailMgr()
	{}

	ThumbnailMgr* ThumbnailMgr::instance()
	{
		static ThumbnailMgr* inst = new ThumbnailMgr;
		return inst;
	}

	bool ThumbnailMgr::saveThumbnail( const Echo::String& fileName, THUMBNAIL_TYPE type /* = THUMB_NON */ )
	{
		Echo::String WaterMarkType[ThumbnailMgr::THUMB_NUM] = 
		{ 
			"", 
			AStudio::instance()->getAppPath() + Echo::String( "assets/watermark/scene.bmp"), 
			AStudio::instance()->getAppPath() + Echo::String( "assets/watermark/att.bmp"),														
		};

		Echo::RenderTarget* defauldBackTarget = Echo::RenderTargetManager::instance()->getRenderTargetByID( Echo::RTI_DefaultBackBuffer);
		if ( defauldBackTarget)
		{
			Echo::String appPath   = AStudio::instance()->getAppPath();
			Echo::String ofileName = Echo::StringUtil::Format( "%sCache/thumbnail/%s.bmp", appPath.c_str(), fileName.c_str() ) ;
			defauldBackTarget->saveTo( ofileName.c_str() );

			// ���ˮӡ
			Echo::FreeImageHelper::addWaterMark( ofileName.c_str(), WaterMarkType[type].c_str() );

			return true;
		}

		return false;
	}

	void ThumbnailMgr::update()
	{
		//Echo::String project = EchoRoot->getConfig().projectFile;

		//��������ͼ
		//ShellExecute(0, "open", "Thumbnail.exe", project.c_str(), "", SW_HIDE);
	}
}
