#pragma once

#include <engine/core/thread/pool/CpuThreadPool.h>
#include "engine/core/base/echo_def.h"

namespace Echo
{
	/**
	 * OpenMPTaskManager 2014-10-27
	 * http://www.openmp.org
	 */
	class OpenMPTaskMgr
	{
	public:
		// Task type
		enum TaskType
		{
			TT_AnimationUpdate = 0,
			TT_EffectSystem,
		};

	public:
		~OpenMPTaskMgr();

		// instance
		static OpenMPTaskMgr* instance();

		// add
		void addTask(TaskType type, CpuThreadPool::Job* task);

		// execs
		void execTasks(TaskType type);

		// wait for finished
		void waitForAnimationUpdateComplete();

		// wait finished
		void waitForEffectSystemUpdateComplete();

	private:
		OpenMPTaskMgr();

	private:
		vector<CpuThreadPool::Job*>::type	m_animationUpdateTasks;				// ������������
		vector<CpuThreadPool::Job*>::type	m_animationUpdateTasksFinished;		// �����������������
		vector<CpuThreadPool::Job*>::type	m_effectSystemUpdateTasks;			// ��Ч��������
		vector<CpuThreadPool::Job*>::type	m_effectSystemUpdateTasksFinished;	// ��Ч��������
		CpuThreadPool*						m_threadPool;						// �̳߳�
	};
}
