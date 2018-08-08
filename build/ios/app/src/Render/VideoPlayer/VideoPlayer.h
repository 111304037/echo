#pragma once

#include "Frame/Example.h"

namespace Examples
{
	/**
	 * ��Ƶ����
	 */
	class VideoPlayer : public Example
	{
	public:
		VideoPlayer();
		virtual ~VideoPlayer() {}

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

