#pragma once

#include "RenderStage.h"

namespace Echo
{
	/**
	 * ˮ�澵��ͼ��Ⱦ�׶�
	 */
	class WaterRefectRenderStage : public RenderStage
	{
		friend class RenderStageManager;
	public:
		WaterRefectRenderStage();
		virtual ~WaterRefectRenderStage();

		// ��ʼ��
		virtual bool initialize();

		// ִ����Ⱦ
		virtual void render();

		// ����
		virtual void destroy();

		//����rendertarget ֻ���������õ�ʱ���ڴ���,�ڴ���ˮ��ʵ���ʱ�򴴽�
		virtual void createRenderTarget();
	};
}