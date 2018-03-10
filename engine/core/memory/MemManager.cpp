#include "MemManager.h"
#include "engine/core/Util/AssertX.h"
#include "MemDefaultAlloc.h"
#include "MemBinnedAlloc.h"

namespace Echo
{
	static MemoryManager* g_inst = nullptr;

	// ���캯��
	MemoryManager::MemoryManager()
		: m_malloc(nullptr) 
	{
	}

	// ��ȡʵ��
	MemoryManager* MemoryManager::instance()
	{
		if (!g_inst)
		{
			g_inst = new MemoryManager;

#if ECHO_MEMORY_ALLOCATOR == ECHO_MEMORY_ALLOCATOR_BINNED
			g_inst->m_malloc = Echo::MallocBinnedMgr::CreateInstance();
#endif
		}

		return g_inst;
	}

	// �滻ʵ��
	void MemoryManager::replaceInstance(MemoryManager* inst)
	{
		EchoAssert(!g_inst);
		g_inst = inst;
		g_inst->m_malloc = inst->m_malloc;

#if ECHO_MEMORY_ALLOCATOR == ECHO_MEMORY_ALLOCATOR_BINNED
		Echo::MallocBinnedMgr::ReplaceInstance((MallocInterface*)(inst->m_malloc));
#endif
	}

	// ����ʵ��
	void MemoryManager::destroyInstance()
	{
		if (g_inst)
		{
			delete g_inst;
			g_inst = nullptr;
		}
	}

	// �ͷ��ڴ�й©��־
	void MemoryManager::outputMemLeakInfo()
	{
#if ECHO_MEMORY_ALLOCATOR == ECHO_MEMORY_ALLOCATOR_BINNED
	#ifdef ECHO_DEBUG
		Echo::MallocBinnedMgr::ReleaseInstance();
		Echo::release_memx();
	#endif
#endif
	}
}