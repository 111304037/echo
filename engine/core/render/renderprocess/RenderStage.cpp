#include "render/Renderable.h"
#include "RenderStageItem.h"
#include "engine/core/render/RenderTargetManager.h"

namespace Echo
{
	// ���캯��
	RenderStageItem::RenderStageItem()
	{
	}

	// ��������
	RenderStageItem::~RenderStageItem()
	{
	}

	// ��Ⱦ
	void RenderStageItem::render()
	{
		for (RenderableID id : m_renderables)
		{
			Renderable* renderable = Renderer::instance()->getRenderable( id);
			if( renderable)
				renderable->render();
		}

		m_renderables.clear();
	}

	DefaultRenderStageItemOpaque::DefaultRenderStageItemOpaque()
	{
		setName("Opaque");
	}

	// render
	void DefaultRenderStageItemOpaque::render()
	{
		RenderTargetManager::instance()->beginRenderTarget(RTI_DefaultBackBuffer);

		RenderStageItem::render();
	}

	DefaultRenderStageItemTransparent::DefaultRenderStageItemTransparent()
	{
		setName("Transparent");
	}

	// render
	void DefaultRenderStageItemTransparent::render()
	{
		RenderStageItem::render();

		RenderTargetManager::instance()->endRenderTarget(RTI_DefaultBackBuffer);
	}
}