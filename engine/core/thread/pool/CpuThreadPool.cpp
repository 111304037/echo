#include "engine/core/Util/AssertX.h"
#include "engine/core/log/LogManager.h"
#include "CpuThreadPool.h"
#include "engine/core/Thread/Threading.h"

namespace Echo
{
	// ���캯��
	CpuThreadPool::CpuThreadPool(const CpuThreadPool::Cinfo& info, CpuThreadPool::StartThreadsMode mode)
	{
	//	m_numOfJobsProcessed.assign(0);

	//	// �ź�����ʼ��
	//	for (scl::semaphore& sem : m_semaphores)
	//	{
	//		sem.create();
	//	}
	//		
	//	// ���������߳�
	//	if (mode == STM_OnConstruction)
	//	{
	//		startThreads( info);
	//	}
	}

	// ��������
	CpuThreadPool::~CpuThreadPool()
	{

	}

	// �����߳�,ֻ�е�����ʱ����STM_Manually������²ſ��Ե��ô˺���
	void CpuThreadPool::startThreads(const Cinfo& info)
	{
#ifdef ECHO_PLATFORM_HTML5
		// do nothing
#else

		//EchoAssert(!m_info.m_numThreads && info.m_numThreads>0);

		//// ȷ���߳�����
		//m_info = info;
		//if (m_info.m_numThreads > m_workerThreads.size() || m_info.m_numThreads<1)
		//{
		//	EchoLogWarning( "You requested more or less threads than the CpuThreadPool supports - see MAX_NUM_THREADS");
		//	m_info.m_numThreads = std::max<int>(m_info.m_numThreads, 1);
		//	m_info.m_numThreads = std::min<int>(m_info.m_numThreads, m_workerThreads.size());
		//}

		//// �����߳�
		//for (ui32 i = 0; i < m_info.m_numThreads; i++)
		//{
		//	ThreadData& threadData = m_workerThreads[i];
		//	threadData.m_threadPool = this;
		//	threadData.m_killThread = false;

 	//		// Worker thread IDs start from 1, 0 is reserved for the main thread
 	//		threadData.m_threadId = i + 1;
  //          auto localThread = std::thread(CpuThreadPool::threadMainForwarder, std::ref(threadData));
 	//		threadData.m_thread.swap(localThread);
 	//		threadData.m_buffer.alloc(1024 * 32);
		//}
#endif
	}

	// �߳���ں���
	void CpuThreadPool::threadMainForwarder(CpuThreadPool::ThreadData& threadData)
	{
		threadData.m_threadPool->threadMain(threadData.m_threadId - 1);
	}

	// �����߳�������
	void CpuThreadPool::threadMain(int threadIndex)
	{
	//	ThreadData& threadData = m_workerThreads[threadIndex];

	//	// ��ѭ��
	//	while (true)
	//	{
	//		if (threadData.m_buffer.used()>=sizeof(JobInfo))
	//		{
	//			JobInfo jobInfo;
	//			threadData.m_buffer.peek(&jobInfo, sizeof(JobInfo));
	//			jobInfo.m_job->process();

	//			threadData.m_buffer.drop(sizeof(JobInfo));

	//			// �������߳�
	//			if (m_info.m_isBlocking)
	//				m_semaphores[jobInfo.m_job->getType()].post();
	//		}
	//		else
	//		{
	//			// ��ֹ�߳�
	//			if (threadData.m_killThread)
	//				return;

	//			// �̹߳���
	//			threadData.m_semaphore.wait();
	//		}
	//	}
	}

	// ����������
	void CpuThreadPool::processJobs(CpuThreadPool::Job** jobs, int numOfJobs)
	{
#ifdef ECHO_PLATFORM_HTML5
		for( int i=0; i<numOfJobs; i++)
		{
			jobs[i]->process();
		}
#else
		//for (int i = 0; i < numOfJobs; i++)
		//{
		//	// ��ȡ������߳�����
		//	int taskNum = 65535;
		//	int threadIdx = 0;
		//	ThreadData& threadData = m_workerThreads[threadIdx];

		//	JobInfo jobInfo;
		//	jobInfo.m_job = jobs[i];

		//	const int freelen = threadData.m_buffer.free();
		//	if (freelen > sizeof(JobInfo))
		//	{
		//		threadData.m_buffer.write(&jobInfo, sizeof(JobInfo));
		//		m_numOfJobsProcessed[jobInfo.m_job->getType()]++;

		//		// ���Ѷ�Ӧ�����߳�
		//		threadData.m_semaphore.post();
		//	}
		//	else
		//	{
		//		EchoLogWarning("CpuThreadPool::processJobs. RingBuffer don't have enough memory space.");
		//	}
		//}
#endif
	}

	// �����ȴ��������
	void CpuThreadPool::waitForComplete(int type)
	{
#ifdef ECHO_PLATFORM_HTML5
		// do nothing
#else
		//EchoAssert(m_info.m_isBlocking);

		//// �������߳�
		//if (m_info.m_isBlocking)
		//{
		//	for (int i = 0; i < m_numOfJobsProcessed[type]; i++)
		//		m_semaphores[type].wait();

		//	m_numOfJobsProcessed[type] = 0;
		//}
#endif
	}

	// ��ȡʹ�õ��߳�����
	int CpuThreadPool::getNumThreads() const
	{
		return m_info.m_numThreads;
	}

	// ��ֹ
	void CpuThreadPool::stop()
	{
		//// ִ���˳�
		//for (int i = 0; i < getNumThreads(); i++)
		//{
		//	// ��ȡ�߳�����
		//	ThreadData& threadData = m_workerThreads[i];

		//	// ���Ѷ�Ӧ�����߳�
		//	threadData.m_killThread = true;
		//	threadData.m_semaphore.post();

		//	// �ȴ�����
		//	threadData.m_thread.join();
		//}
	}
}