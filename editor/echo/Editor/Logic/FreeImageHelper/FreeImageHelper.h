#pragma once

#include <engine/core/base/type_def.h>
#include <freeimage/FreeImage.h>
#include <engine/core/util/StringUtil.h>
#include <engine/core/math/color.h>

namespace Echo
{
	class FreeImageHelper
	{
	public:
		struct ImageInfo
		{
			ui32					m_width;		// ��
			ui32					m_height;		// ��
			FREE_IMAGE_COLOR_TYPE	m_colorType;	// ��ʽ����
		};

	public:
		FreeImageHelper();
		~FreeImageHelper();
        
        // instance
        static FreeImageHelper* instance();

		/** ��ȡͼ����Ϣ */
        bool getImageInfo( const char* filePath, ImageInfo& imageInfo);

		// scale image
        bool rescaleImage( const char* iFilePath, const char* oFilePath, float scaleValue=1.f);
        bool rescaleImage( const char* iFilePath, const char* oFilePath, ui32 targetWidth, ui32 targetHeight);

		/** ����Ϊtga��ʽ����ѹ�� */
        bool saveImageToTGA( const char* iFilePath, const char* oFilePath, bool isResale=false, float scaleValue=1.f);

		/** ����Ϊbmp��ʽ����ѹ�� */
        bool saveImageToBmp( const char* iFilePath, const char* oFilePath, bool isResale, ui32 width, ui32 height);

		/** ��ȡalphaͨ����һ�����ļ� */
        bool extracRGBAlphaChannel( const char* srcPath, const char* oRgbFile, const char* oAlphaFile);

		/** ӳ���� */
        int MappingFlagsByFormat( FREE_IMAGE_FORMAT fmt);

		/** ����Ϊbmp��ʽ(��֧��RGB��ʽ) */
        int SaveImageToBmp( BYTE* pixelData, int widht, int height, const char* savePath);

		/** ��ȡͼƬ����buffer */
        bool getImageBits(const char* filePath,unsigned char* bits);

		/** ���ˮӡ */
        bool	addWaterMark(const char* dstFile, const char* srcFile);

		/** ��alphaͨ���ֽ⵽RGB����ӵ�ĩβ */
        bool SaveRGBAtoRGBjpeg(const char* srcFile, const char* dstFile, ImageInfo& imageInfo,float scale);

		/** �ϲ���ͼ����߶����ӣ����tga�� */
        bool mergeIMage(StringArray* srcFile, const char* dstFile);

		/** ���ļ�����ȡ��ɫֵ */
        bool extractColors(const char* srcPath, std::vector<Echo::Color>& colors, int& width, int& height);
	};
}
