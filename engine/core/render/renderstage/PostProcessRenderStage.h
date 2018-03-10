#pragma once

#include "RenderStage.h"
#include "engine/core/render/render/Material.h"
#include "engine/core/render/render/RenderInput.h"

namespace Echo
{
	/**
	 * ����׶�
	 */
	class PostImageEffect;
	class PostProcessRenderStage : public RenderStage
	{
		friend class RenderStageManager;

	public:
		PostProcessRenderStage();
		virtual ~PostProcessRenderStage();

		virtual bool initialize();
		virtual void render();
		virtual void destroy();

		bool isEffectEnable(const String& name) const;
		bool isEnabled() const;
		void setImageEffectEnable(const String& name, bool enable);
		bool getImageEffectEnable(const String& name) const;
		void setImageEffectSourceRT(const ui32 rtId);
		void setImageEffectParameter(const String& name, const String& uniform, const Vector4& value);
		void setLogicCalcParameter(const String& name, const String& uniform, const String& value);
		Vector4 getImageEffectParameter(const String& name, const String& uniform);
		String getLogicCalcParameter(const String& name, const String& uniform);

		i32 getFinalOutput(){ return m_finalOutput; }

		PostImageEffect* getPostImageEffect(const String& name);

		StringArray getAllEffect() const;

		void setIsForUI(bool settting){ m_isForUI = settting; }

		void renderImp();

	private:
		RenderInput::VertexElementList	m_verElementLists;		// �����ʽ
		GPUBuffer*						m_screenAlignedQuadVB;	// ���㻺��
		GPUBuffer*						m_screenAlignedQuadIB;	// ��������
		vector<PostImageEffect*>::type	m_postEffects;

		i32							    m_finalOutput;			// 

		// vr Mode
	protected:
		RenderInput::VertexElementList	m_verElementLists_vr;		// �����ʽ
		GPUBuffer*						m_EyeGlassVB[2];			// ���㻺��(��������)
		GPUBuffer*						m_EyeGlassIB[2];			// ��������(��������)
		Material*						m_material;
		Renderable*						m_renderable[2];
		RenderInput*					m_renderInput[2];
		Vector2							m_eyeToSourceUVScale;
		Vector2							m_eyeToSourceUVOffset;
		Matrix4							m_normalizeMat;
		float							m_eyesOffset;
		bool							m_isForUI;
	};
}  // namespace Echo
