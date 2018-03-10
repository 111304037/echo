#pragma once

#include "engine/core/Util/Singleton.h"
#include <engine/core/Memory/MemManager.h>
#include <engine/core/render/render/RenderState.h>
#include <engine/core/render/render/RenderInput.h>

namespace Echo
{
	// ��Ⱦ�׶�ID
	enum RenderStageID
	{
		RSI_EditorBackground,		// �༭������
		RSI_Coverage,				// �ڱ�ͼ
		RSI_LDRSceneColor,			// �Ͷ�̬��Χ
		RSI_HDRSceneColor,			// �߶�̬��Χ
		RSI_PostProcess,			// ����׶�
		RSI_AfterPostProcess,		// ������ɺ�׶�
		RSI_Reflective,				// ���淴��
		RSI_ShadowMap,				// ��Ⱦ��Ӱͼ
		RSI_WaterRefectMap,			// ˮ�淴��ͼ
		RSI_Glow,
		RSI_Spine,					// 2D��������
		RSI_UI,						// ����
		RSI_EditorFrontground,		// ��������
		RSI_End
	};

	/**
	* ��Ⱦ�׶�
	*/
	class RenderStageManager;
	class RenderStage
	{
		friend class RenderStageManager;
	public:
		RenderStage(RenderStageID stageName);
		virtual ~RenderStage();

		// �����Ƿ����
		virtual void setEnable(bool _enable) { m_bEnable = _enable; }

		// �Ƿ����
		bool getEnable() const { return m_bEnable; }

		// ��ȡID
		const RenderStageID getID() const { return m_rsID; }

		// ��Ⱦ
		virtual void render() = 0;

		// ��ʼ��
		virtual bool initialize() = 0;

		// ����
		virtual void destroy() = 0;

	protected:
		bool			m_bEnable;				// �Ƿ����
		bool			m_bInitialized;			// �Ƿ��ѳ�ʼ��
		RenderStageID	m_rsID;					// ID
	};
}