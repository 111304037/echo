#pragma once

#include "RenderQueue.h"

namespace Echo
{
	class RenderQueueGroup
	{
	public:
		RenderQueueGroup() {}
		~RenderQueueGroup() {}

		// get instance
		static RenderQueueGroup* instance();

		// add render queue
		RenderQueue* addRenderQueue(const String& name);

		// ���ݲ���ģ�������Ⱦ����
		RenderQueue* addRenderQueue(const String& materialTemplate, const char* stage, const String& macros, bool isAlphaTest);

		// ������Ⱦ�������ƻ�ȡ��Ⱦ����
		RenderQueue* getRenderQueue(const String& strQueueName) const;

		// ����������ȡ��Ⱦ����
		RenderQueue* getRenderQueueByIndex(ui32 nIndex) const;

		// �������ƻ�ȡ��������
		ui8 getRenderQueueIndex(const String& strQueueName) const;

		// ��ȡ��������
		size_t getRenderQueueCount() { return m_renderQueues.size(); }

		// ��Ⱦ����ִ��(����endQueue)
		void execRenderQueue(const String& startQueue, const String& endQueue, bool includeEndQueue = false);

		// �����Ⱦ����(����endQueue)
		void clearRenderQueue(const String& startQueue, const String& endQueue, bool includeEndQueue = false);

		// clear
		void clear();

		// render
		void render();

	private:
		vector<RenderQueue*>::type  m_renderQueues;
	};
}