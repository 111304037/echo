#pragma once

#include "RenderStage.h"

namespace Echo
{
	/**
	* LDRRenderStage
	*/
	class QueryObject;
	class LDRSceneColorRenderStage : public RenderStage
	{
		friend class RenderStageManager;
		typedef vector<QueryObject*>::type QueryObjectVector;
	public:
		LDRSceneColorRenderStage();
		virtual ~LDRSceneColorRenderStage();

		// ��ʼ��
		virtual bool initialize();

		// ִ����Ⱦ
		virtual void render();

		// ����
		virtual void destroy();
	};
}