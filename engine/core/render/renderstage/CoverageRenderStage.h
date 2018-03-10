#pragma once

#include "RenderStage.h"
#include "engine/core/render/render/Material.h"

namespace Echo
{
	/**
	* LDRRenderStage
	*/
	class QueryObject;
	class CoverageRenderStage : public RenderStage
	{
		friend class RenderStageManager;
		typedef vector<QueryObject*>::type QueryObjectVector;
	public:
		CoverageRenderStage();
		virtual ~CoverageRenderStage();

		// ��ʼ��
		virtual bool initialize();

		// ִ����Ⱦ
		virtual void render();

		// ����
		virtual void destroy();

		virtual Echo::ShaderProgram* getProgram(){ return m_pMtlCoverage->getShaderProgram(); }

		// ��Ӳ�ѯ����
		void addQueryObject(class QueryObject* pQo);

		// �����ѯ����
		void clearQueryObjects() { m_vecQueryObjects.clear(); }

	protected:
		QueryObjectVector m_vecQueryObjects;

	private:
		const SamplerState* m_coverageSamplerState;

		class Material* m_pMtlCoverage;
	};
}