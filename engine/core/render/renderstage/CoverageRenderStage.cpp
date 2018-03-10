#include "CoverageRenderStage.h"
#include "engine/core/Scene/scene_manager.h"
#include "engine/core/main/EngineSettings.h"
#include "engine/core/render/render/Material.h"
#include "Engine/core/Render/RenderTargetManager.h"
#include "Engine/core/Render/MaterialInstance.h"

namespace Echo
{
	// ���캯��
	CoverageRenderStage::CoverageRenderStage()
		: RenderStage(RSI_Coverage)
		, m_coverageSamplerState(NULL)
		, m_pMtlCoverage(NULL)
	{
	}

	// ��������
	CoverageRenderStage::~CoverageRenderStage()
	{
		destroy();
	}

	bool CoverageRenderStage::initialize()
	{
		if (!EngineSettingsMgr::instance()->isEnableCoverage() || m_bInitialized)
		{
			return false;
		}
		// ��ʼ���ڸ�ͼ
		ui32 screenWidth = Renderer::instance()->getScreenWidth();
		ui32 screenHeight = Renderer::instance()->getScreenHeight();

		// ����һ��floatֵ������
		RenderTarget::Options option;
		RenderTarget* target = RenderTargetManager::instance()->createRenderTarget(RTI_CoverageMap, 
			screenWidth, screenHeight, PF_RGBA8_UNORM, option);
		if (target)
		{
			SamplerState::SamplerDesc desc;
			desc.addrUMode = SamplerState::AM_CLAMP;
			desc.addrVMode = SamplerState::AM_CLAMP;
			desc.addrWMode = SamplerState::AM_CLAMP;
			m_coverageSamplerState = Renderer::instance()->getSamplerState(desc);
		}
		else
		{
			EchoLogError("RenderTargetManager::createRenderTarget( m_coverageSamplerState ) ... Failed");
			return false;
		}

		// ������������ͼ�Ĳ���
		m_pMtlCoverage = MaterialManager::instance()->createMaterial();
		EchoAssert(m_pMtlCoverage);
		m_pMtlCoverage->loadFromFile("pp_CoverageMap.xml","");

		return true;
	}

	void CoverageRenderStage::destroy()
	{
		MaterialManager::instance()->destroyMaterial(m_pMtlCoverage);
	}

	void CoverageRenderStage::addQueryObject(class QueryObject* pQo)
	{
		m_vecQueryObjects.push_back(pQo);
	}

	// ��Ⱦ
	void CoverageRenderStage::render()
	{
		if (m_bEnable)
		{ 
			EchoAssert(SceneManager::instance());
			EchoAssert(RenderTargetManager::instance());
			ui32 coverageRendertarget;

			coverageRendertarget = RTI_CoverageMap;
			// ����ȾĿ��
			RenderTargetManager::instance()->beginRenderTarget(coverageRendertarget,true,Color(0,0,0,0.5)); 

	//		m_pMtlCoverage->active();
			//for (QueryObjectVector::iterator bit = m_vecQueryObjects.begin(); bit != m_vecQueryObjects.end(); ++bit)
			//{
				// ʹ���Զ��������ȾQueryObject
//				(*bit)->setCustomProgram(m_pMtlCoverage->getShaderProgram());
				//(*bit)->render();
//				(*bit)->setCustomProgram(NULL);
			//}
			m_vecQueryObjects.clear();
		}
	}
}