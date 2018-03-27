#pragma once

#include <engine/core/Memory/MemManager.h>

namespace Echo
{
	class Renderable;

	/**
	 * ��Ⱦ����
	 */
	class RenderQueue
	{
		friend class Renderable;
		typedef vector<int>::type RenderableList;

	public:
		RenderQueue(const String& strName);
		~RenderQueue();

		// ��ʼ��Ⱦ
		void beginRender();

		bool hasRenderables(){ return !m_renderables.empty(); }

		void sortByPos();

	private:
		// �����Ⱦ��
		void addRenderable(Renderable* pRenderable);

	public:
		// ִ����Ⱦ(����Ϊ������Ⱦ)
		void renderQueue();

		// ��տ���Ⱦ��
		void clearRenderables();

		// ��ȡ����
		const String& getName() { return m_name; }

		// ����Ϊ����Ⱦ
		void setEnable( bool isEnable) { m_isEnable = isEnable; }

		// �Ƿ����
		bool isEnable() const { return m_isEnable; }

	protected:
		String					m_name;
		RenderableList			m_renderables;		
		bool					m_isEnable;
	};
}
