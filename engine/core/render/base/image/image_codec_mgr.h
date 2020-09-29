#pragma once

#include "image_codec.h"

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
