#pragma once

#include "RenderStage.h"
#include <engine/core/render/render/Renderable.h>

namespace Echo
{
	/**
	 * UI��Ⱦ�׶�
	 */

	class UIRenderStage : public RenderStage
	{
		friend class RenderStageManager;

	public:
		virtual bool	initialize					();
		virtual void	render						();
		virtual void	destroy						();

		// catui
		void			addCatUIManualRenderable	(IManualRenderable* renderable);
		void			addCatUIRenderable			(IManualRenderable* renderable, const int renderLevel);
		void			addCatUIEffectRenderable	(IManualRenderable* renderable, const int renderLevel);

	protected:
		UIRenderStage();
		virtual ~UIRenderStage();

		void			_postRenderClear			();
		void			_renderCatui				();
		void			_renderCEGUI				();
		void			_renderCatuiTop				();
		void			_renderCatui3DModel			();

	private:
		vector<IManualRenderable*>::type			m_catuiManualRenderables;	// CatUI�ֶ�����Ⱦ����
		vector<IManualRenderable*>::type			m_catuiRenderables;			// CatUI����Ⱦ����
		vector<IManualRenderable*>::type			m_catuiTopRenderables;		// λ�ڶ����CatUI����Ⱦ������CEGUI����
		vector<IManualRenderable*>::type			m_autoDeletes;			
	};
}