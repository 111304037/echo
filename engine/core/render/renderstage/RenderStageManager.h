#pragma once

#include "RenderStage.h"
#include "ShadowMapRenderStage.h"
#include "PostProcessRenderStage.h"
#include "LDRSceneColorRenderStage.h"

namespace Echo
{
	/**
	* ��Ⱦ�׶ι�����
	*/
	class RenderStageManager
	{
		__DeclareSingleton(RenderStageManager);

	public:
		RenderStageManager();
		~RenderStageManager();

		// ��ʼ��
		bool initialize();

		// ͨ����Ⱦ�׶�ID��ȡ��Ⱦ�׶�
		RenderStage* getRenderStageByID(RenderStageID _rsid);

		// ִ����Ⱦ�׶�
		void processRenderStages();

	protected:
		ShadowMapRenderStage*		m_shadowMapRenderStage;		// ��Ӱͼ��Ⱦ�׶�
		vector<RenderStage*>::type	m_renderStages;				// ��Ⱦ�׶�
	};
}