#include "RenderStage.h"

namespace Echo
{
	// ���캯��
	RenderStage::RenderStage(RenderStageID stageID)
		: m_rsID(stageID)
		, m_bEnable(false)
		, m_bInitialized(false)
	{
	}

	// ��������
	RenderStage::~RenderStage()
	{
	}
}