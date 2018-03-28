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

		// ִ����Ⱦ(����Ϊ������Ⱦ)
		void render();

		// ��տ���Ⱦ��
		void clear();

		// ��ȡ��Ⱦ������
		ui32 getRenderableCount(){ return m_renderables.size(); }

		// ��ȡ����
		const String& getName() { return m_name; }

		// ����Ϊ����Ⱦ
		void setEnable( bool isEnable) { m_isEnable = isEnable; }

		// �Ƿ����
		bool isEnable() const { return m_isEnable; }

	private:
		// �����Ⱦ��
		void addRenderable(Renderable* pRenderable);

		// ���ݾ�������
		void sortByPos();

	protected:
		String					m_name;
		RenderableList			m_renderables;		
		bool					m_isEnable;
	};
}
