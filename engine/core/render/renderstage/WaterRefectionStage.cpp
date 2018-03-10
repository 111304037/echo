#include "WaterRefectionStage.h"
#include "engine/core/Render/RenderTargetManager.h"
#include <engine/core/Util/LogManager.h>
#include "Engine/core/main/EngineSettings.h"
#include "Engine/core/Scene/Scene_Manager.h"

namespace Echo
{
	// ���캯��
	WaterRefectRenderStage::WaterRefectRenderStage()
		:RenderStage(RSI_WaterRefectMap)
	{

	}

	// ��������
	WaterRefectRenderStage::~WaterRefectRenderStage()
	{
		destroy();
	}

	// ��ʼ��
	bool WaterRefectRenderStage::initialize()
	{
		createRenderTarget();
		return true;
	}

	// ��Ⱦ����ͼ
	void WaterRefectRenderStage::render()
	{
		// �߼�Ʒ���о���
		if (getEnable() && EngineSettingsMgr::instance()->getWaterQuality() >= WQ_High)
		{
			RenderTargetManager::instance()->beginRenderTarget(RTI_WaterRefectMap);
			//SceneManager::instance()->renderWaterRefectScene(true);
			RenderTargetManager::instance()->endRenderTarget(RTI_WaterRefectMap);
		}
	}

	// ����
	void WaterRefectRenderStage::destroy()
	{

	}

	// ������ȾĿ��
	void WaterRefectRenderStage::createRenderTarget()
	{
		ui32 screenWidth = static_cast<ui32>(Renderer::instance()->getScreenWidth() * 0.5f);
		ui32 screenHeight = static_cast<ui32>(Renderer::instance()->getScreenHeight() * 0.5f);

		RenderTarget::Options option;
		option.depth = true;
		auto rt = RenderTargetManager::instance()->createRenderTarget(RTI_WaterRefectMap, screenWidth, screenHeight, PF_RGBA8_UNORM, option);
		if (rt == nullptr)
		{
			EchoLogError("RenderTargetManager::createRenderTarget waterRefectMap failed");
		}
		rt->setScaleFactor(1.f);
		rt->setResolutionRelative(false);
	}
}