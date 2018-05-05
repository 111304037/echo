#ifndef __ECHO_IMAGECODECMGR_H__
#define __ECHO_IMAGECODECMGR_H__

#include "RenderDef.h"
#include "ImageCodec.h"

namespace Echo
{
	/**
	 * ������������
	 */
	class ImageCodecMgr
	{
		typedef map<int, ImageCodec*>::type CodecMap;
	public:
		~ImageCodecMgr();

		// ��ȡʵ��
		static ImageCodecMgr* instance();

		// ע�����
		void registerCodec(ImageCodec *pImgCodec);

		// ж�ر���
		void unregisterCodec(ImageFormat imgFmt);

		// ��ȡ����
		ImageCodec* getCodec(ImageFormat imgFmt) const;

	private:
		ImageCodecMgr();

	private:	
		CodecMap m_codecMap;		// ����ͼ
	};
}

#endif
