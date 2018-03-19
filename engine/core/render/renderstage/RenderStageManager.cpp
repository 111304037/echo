#include "RenderStageManager.h"
#include "PostProcessRenderStage.h"
#include "AfterPostProcessStage.h"
#include "LDRSceneColorRenderStage.h"
#include "UIRenderStage.h"
#include "CoverageRenderStage.h"
#include "WaterRefectionStage.h"
#include "GlowRenderStage.h"
#include "SpineRenderStage.h"
#include "Engine/core/main/EngineSettings.h"
#include "Engine/core/main/Root.h"

namespace Echo
{
	__ImplementSingleton(RenderStageManager);

	// ���캯��
	RenderStageManager::RenderStageManager()
	{
		__ConstructSingleton;

		// ��Ӱͼ��Ⱦ�׶�
		//m_shadowMapRenderStage = EchoNew(ShadowMapRenderStage);
		//m_shadowMapRenderStage->setEnable(EngineSettingsMgr::instance()->isActorCastShadow());
		//m_renderStages.push_back(m_shadowMapRenderStage);

		// ��Ⱦ������Ƭ�׶�
		RenderStage* pRs = NULL;
		//pRs = EchoNew(CoverageRenderStage);
		//pRs->setEnable(Root::instance()->getSettingsMgr().isEnableCoverage());
		//m_renderStages.push_back(pRs);

		//��Ⱦˮ��
		//pRs = EchoNew(WaterRefectRenderStage);
		//pRs->setEnable(false);
		//m_renderStages.push_back(pRs);

		// ��Ⱦ�����׶�
		pRs = EchoNew(LDRSceneColorRenderStage);
		pRs->setEnable(true);
		m_renderStages.push_back(pRs);

		// ����׶�
		//pRs = EchoNew(PostProcessRenderStage);
		//pRs->setEnable(true);
		//m_renderStages.push_back(pRs);

		//pRs = EchoNew(AfterPostProcessStage);
		//pRs->setEnable(true);
		//m_renderStages.push_back(pRs);

		//pRs = EchoNew(GlowRenderStage);
		//pRs->setEnable(EngineSettingsMgr::instance()->isEnableGlow());
		//m_renderStages.push_back(pRs);

		// 2D���������׶�
		//pRs = EchoNew(SpineRenderStage);
		//pRs->setEnable(true);
		//m_renderStages.push_back(pRs);

		//pRs = EchoNew(UIRenderStage);
		//pRs->setEnable(true);
		//m_renderStages.push_back(pRs);
	}

	// ��������
	RenderStageManager::~RenderStageManager()
	{
		for (vector<RenderStage*>::type::iterator bit = m_renderStages.begin(); bit != m_renderStages.end();)
		{
			EchoSafeDelete(*bit, RenderStage);
			bit = m_renderStages.erase(bit);
		}

		__DestructSingleton;
	}

	bool RenderStageManager::initialize()
	{
		vector<RenderStage*>::type::iterator bit = m_renderStages.begin();
		for (; bit != m_renderStages.end(); ++bit)
		{
			(*bit)->initialize();
		}
		return true;
	}

	// ����������Ⱦ�׶�
	void RenderStageManager::processRenderStages()
	{
		// ����������Ⱦ�׶�
		for (size_t i = 0; i < m_renderStages.size(); i++)
		{
			m_renderStages[i]->render();
		}
	}

	// ͨ����Ⱦ�׶�ID��ȡ��Ⱦ�׶�
	RenderStage* RenderStageManager::getRenderStageByID(RenderStageID _rsid)
	{
		// ���Ա���
		for (vector<RenderStage*>::type::iterator bit = m_renderStages.begin(); bit != m_renderStages.end(); ++bit)
		{
			if ((*bit)->getID() == _rsid)
				return *bit;
		}
		return NULL;
	}
}