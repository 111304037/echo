#pragma once

#include <thread>
#include <engine/core/util/Array.hpp>

namespace Echo
{
	/**
	 * ����CPU���̳߳�ʵ��
	 */
	class CpuThreadPool
	{
	public:
		// ������Ϣ
		struct Cinfo
		{
			ui32		m_numThreads;		// �����߳�����
			bool		m_isBlocking;		// ִ�й���ʱ�Ƿ��������߳�

			// ���캯��
			Cinfo()
			{
				m_numThreads = 0;
				m_isBlocking = true;
			}
		};

		// �̳߳ؿ�ʼ״̬
		enum StartThreadsMode
		{
			STM_OnConstruction,		// �������������
			STM_Manually,			// �ֶ�����startThreads������
		};

		/**
		* Job Base type for all jobs
		*/
		class Job
		{
		public:
			Job(){}
			virtual ~Job(){}

			// ��������
			virtual bool process() = 0;

			// ������ɺ����(���߳�)
			virtual bool onFinished() { return true; }

			// ��ȡ��������
			virtual int getType() = 0;
		};


		// ������Ϣ
		struct JobInfo
		{
			Job*	m_job;
		};

		// �����߳�״̬
		struct ThreadData
		{
			CpuThreadPool*			m_threadPool;		// ��Ӧ���̳߳�
			int						m_threadId;			// �߳�Id (0-N)
			std::thread				m_thread;			// �߳�
			//scl::semaphore			m_semaphore;		// �ź�����������ͣ�����߳�
			//scl::ring_buffer		m_buffer;			// ����
			bool					m_killThread;		// ���λ�����̱߳�Ҫ��ر�ʱ����Ϊtrue

			// ���캯��
			ThreadData()
				: m_threadPool( NULL)
			{
				//m_semaphore.create();
			}
		};

	public:
		CpuThreadPool(const Cinfo& info, StartThreadsMode mode=STM_OnConstruction);
		virtual ~CpuThreadPool();

		// �����߳�,ֻ�е�����ʱ����STM_Manually������²ſ��Ե��ô˺���
		void startThreads(const Cinfo& info);

		// ����������(������)
		void processJobs(Job** jobs, int numOfJobs);

		// �����ȴ��������(Ҫ�����߳�Ϊ����ģʽ)
		void waitForComplete( int type);

		// ��ȡʹ�õ��߳�����
		int getNumThreads() const;

		// ��ֹ
		void stop();

	protected:
		// �߳���ں���
		static void threadMainForwarder(ThreadData& threadData);

		// �����߳�������
		void threadMain(int threadIndex);

	private:
		Cinfo						m_info;								// ��ǰ�߳�����
		array<ThreadData, 32>		m_workerThreads;					// �����߳�����
		//array<scl::semaphore, 24>	m_semaphores;						// �ź������������̵߳ȴ�
		//array<int, 24>				m_numOfJobsProcessed;				// �����̴߳����������������Ӧ��������
	};
}
