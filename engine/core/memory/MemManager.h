#pragma once

#include "MemAllocDef.h"

namespace Echo
{
	/**
	 * �ڴ������(����Ψһ)
	 */
	class MemoryManager
	{
	public:
		// ��ȡʵ��
		static MemoryManager* instance();

		// �滻ʵ��
		static void replaceInstance(MemoryManager* inst);

		// ����ʵ��
		static void destroyInstance();

		// �ͷ��ڴ�й©��־
		static void outputMemLeakInfo();

	private:
		MemoryManager();
		~MemoryManager() {}

	private:
		void*	m_malloc;			// ������
	};
}