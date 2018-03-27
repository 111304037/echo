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
		ImageCodecMgr();
		~ImageCodecMgr();

		// ��ȡʵ��
		static ImageCodecMgr* instance();

		// �滻ʵ��
		static void replaceInstance( ImageCodecMgr* inst);

		// ע�����
		void registerCodec(ImageCodec *pImgCodec);

		// ж�ر���
		void unregisterCodec(ImageFormat imgFmt);

		// ��ȡ����
		ImageCodec* getCodec(ImageFormat imgFmt) const;

	private:	
		CodecMap m_codecMap;		// ����ͼ
	};
}

#endif
