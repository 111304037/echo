#include "Render/ImageCodec.h"
#include "Render/ImageCodecMgr.h"
#include <freeimage/FreeImage.h>

#ifdef _WINDOWS_
#undef _WINDOWS_
#endif

namespace Echo
{
	static ImageCodecMgr* g_instance = NULL;

	// ��ȡʵ��
	ImageCodecMgr* ImageCodecMgr::instance()
	{
		EchoAssert(g_instance);
		return g_instance;
	}

	// �滻ʵ��
	void replaceInstance(ImageCodecMgr* inst)
	{
		EchoAssert(!g_instance);
		g_instance = inst;
	}

	// ���캯��
	ImageCodecMgr::ImageCodecMgr()
	{
		EchoAssert(!g_instance);
		g_instance = this;

		// lazy initialization.
		FreeImage_Initialise();

		ImageCodec *pDDSCodec = EchoNew(ImageCodec(IF_DDS));
		ImageCodec *pJPGCodec = EchoNew(ImageCodec(IF_JPG));
		ImageCodec *pPNGCodec = EchoNew(ImageCodec(IF_PNG));
		ImageCodec *pPVRCodec = EchoNew(ImageCodec(IF_PVR));
		ImageCodec *pTGACodec = EchoNew(ImageCodec(IF_TGA));
		ImageCodec *pBMPCodec = EchoNew(ImageCodec(IF_BMP));

		registerCodec(pDDSCodec);
		registerCodec(pJPGCodec);
		registerCodec(pPNGCodec);
		registerCodec(pPVRCodec);
		registerCodec(pTGACodec);
		registerCodec(pBMPCodec);
	}

	ImageCodecMgr::~ImageCodecMgr()
	{
		CodecMap::iterator it = m_codecMap.begin();
		for(; it != m_codecMap.end(); ++it)
		{
			EchoSafeDelete(it->second, ImageCodec);
		}
		m_codecMap.clear();

		FreeImage_DeInitialise();
	}

	void ImageCodecMgr::registerCodec(ImageCodec* pImgCodec)
	{
		EchoAssertX(pImgCodec, "The Image Codec can't be null!");

		if(pImgCodec)
		{
			int idx = (int)pImgCodec->getImageFormat();
			m_codecMap[idx] = pImgCodec;
		}
	}

	void ImageCodecMgr::unregisterCodec(ImageFormat imgFmt)
	{
		CodecMap::iterator it = m_codecMap.find((int)imgFmt);
		if (it == m_codecMap.end())
		{
			EchoAssertX(false, "Couldn't found the codec.");
			return;
		}

		ImageCodec *pImgCodec= it->second;
		m_codecMap.erase(it);

		EchoSafeDelete(pImgCodec, ImageCodec);
	}

	ImageCodec* ImageCodecMgr::getCodec(ImageFormat imgFmt) const
	{
		CodecMap::const_iterator it = m_codecMap.find(imgFmt);
		if (it == m_codecMap.end())
			return NULL;
		else
			return it->second;
	}
}