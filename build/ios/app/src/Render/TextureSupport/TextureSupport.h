#pragma once

#include "Frame/Example.h"

namespace Examples
{
	/**
	 * �����ʽ֧���Լ��
	 * �����ļ��У���ʾ����������Դ
	 */
	class TextureSupport : public Example
	{
	public:
		TextureSupport();
		virtual ~TextureSupport() {}

		// ������Ϣ
		virtual const LORD::String getHelp();

		// ��ʼ��
		virtual bool initialize();

		// ����
		virtual void tick(LORD::ui32 elapsedTime);

		// ����
		virtual void destroy();

	private:
	};
}

