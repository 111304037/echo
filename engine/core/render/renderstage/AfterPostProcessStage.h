#pragma once

#include "RenderStage.h"

namespace Echo
{
	/**
	* ������ɽ׶�
	*/
	class AfterPostProcessStage : public RenderStage
	{
		friend class RenderStageManager;
	public:
		AfterPostProcessStage();
		virtual ~AfterPostProcessStage();

		// ��ʼ��
		virtual bool initialize();

		// ��Ⱦ
		virtual void render();

		// ����
		virtual void destroy();
	};
}