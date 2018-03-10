#pragma once

#include "RenderStage.h"

namespace Echo
{
	/**
	 * Spine��Ⱦ�׶�
	 */
	class SpineRenderStage : public RenderStage
	{
		friend class RenderStageManager;

	public:
		virtual bool initialize();
		virtual void render();
		virtual void destroy();

		// �����Ⱦ��Ԫ
		void addRenderable(int renderalbe) { m_renderables.push_back(renderalbe); }

	protected:
		SpineRenderStage();
		virtual ~SpineRenderStage();

	private:
		vector<int>::type	m_renderables;		// 2d����������Ⱦ��Ԫ
	};
}