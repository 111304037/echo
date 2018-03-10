#pragma once

#include "RenderStage.h"
#include "engine/core/render/render/Viewport.h"

namespace Echo
{
	/**
	* ��Ӱͼ��Ⱦ�׶�
	*/
	class ShadowMapRenderStage : public RenderStage
	{
		friend class RenderStageManager;
		typedef vector<int>::type RenderableList;
	public:
		ShadowMapRenderStage();
		virtual ~ShadowMapRenderStage();

		// ��ʼ��
		virtual bool initialize();

		// ��Ⱦ
		virtual void render();

		// ����
		virtual void destroy();

		// ������ӰŨ�ȣ�0 - 1 ��ǳ���
		void setShadowShade(float t){ m_shadowShade = t; }
		float* getShadowShadePtr(){ return &m_shadowShade; }

		// ������Ӱͼ�ߴ磬2�Ĵ���

		float getShadowMapSize(){ return static_cast<float>(m_shadowMapSize); }
		void setShadowMapSize(ui32 size);

		void enableBlurShadow(bool isEnable);
		bool isEnableBlurShadow() const;

	protected:
		const SamplerState*		m_shadowMapSamplerState;		// ��Ӱͼ����״̬

		ui32 m_shadowMapSize;

		float m_shadowShade;

		bool m_init;

		RenderInput::VertexElementList m_verElementLists;	// �����ʽ
		class GPUBuffer* m_pScreenAlignedQuadVB;
		class GPUBuffer* m_pScreenAlignedQuadIB;

		bool m_bNeedBlur;

		bool m_isShadowCameraEnable;

		// materal
		class Material* m_pMtlShadowMapVBlurPass;
		class Material* m_pMtlShadowMapHBlurPass;

		// renderable
		class Renderable* m_pRaShadowMapVBlurPass;
		class Renderable* m_pRaShadowMapHBlurPass;

		Viewport	m_renderViewPort;
	};
}