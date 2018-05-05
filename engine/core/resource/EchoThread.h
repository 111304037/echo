#pragma once

#include "engine/core/Thread/Threading.h"
#include "engine/core/memory/MemAllocDef.h"

namespace Echo
{
	/**
	 * �̶߳���
	 */
	class Event;
	class ThreadLoopObject
	{
	public:
		// �߳�״̬
		enum ThreadState
		{
			TS_Running,		// ������
			TS_ShutingDown,	// �ѹر�
		};

	public:
		ThreadLoopObject();
		virtual ~ThreadLoopObject();

		// ����
		virtual void start();

		// �ر�
		virtual void shutdown();

	public:
		// ��Ծ��
		virtual bool IsRunning();

		// �Ƿ����
		virtual bool IsPause();

		// �̴߳���ѭ��
		virtual void processLoop()=0;
	protected:
		// ��ͣ�߳�
		void pause();

		// �ָ��߳�
		void resume();

#ifdef ECHO_PLATFORM_WINDOWS
		// �̺߳���
		static unsigned __stdcall ThreadCallBackProc(void *pvArg);
#else
		// �̺߳���
		static void* ThreadCallBackProc(void *pvArg);
#endif

	protected:
		void*			m_hThread;					// �߳̾��
		Event*			m_threadEvent;				// �߳��¼�����
		ThreadState		m_state;					// �߳�״̬
		bool			mIsPause;					// �Ƿ��ǹ���״̬
	};

	/**
	 * ��Դ�����߳�
	 */
	class StreamThread : public ThreadLoopObject
	{
	public:
		/** �����Ż��� */
		enum TaskLevel
		{
			TL_High,
			TL_Normal,
			TL_Low
		};

		/** ���� */
		class Task
		{
		public:
			Task(){}
			virtual ~Task(){}

			// ��������
			virtual bool process() = 0;

			// ���������Ӧ
			virtual bool finished() = 0;

			// ���ظ��¼������ȼ���
			virtual TaskLevel GetLevel() { return TL_Normal; }

			// �Ƿ��ظ�֪ͨ
			virtual bool isRepeatNotifyIfFail()=0;

			// �Ƿ��Զ��ͷ�
			virtual bool isAutoDestroy()=0;
		};
		typedef multimap<TaskLevel,Task*>::type TaskMap;
		typedef vector<Task*>::type	TaskArray;

	public:
		virtual ~StreamThread();

		// instance
		static StreamThread* instance();

		// �������
		virtual ui32 addTask( Task* pThreadEvent);

		// �ر�
		virtual void shutdown();

		// ֪ͨ�������
		virtual void notifyTaskFinished();

		// ��ȡ��ǰ��������
		virtual int getTaskCount();

		// ��ȡ�������������
		virtual int getTaskFinishedCount();

		// �̴߳���ѭ��
		virtual void processLoop();

	private:
		StreamThread();

		// ��ȡ����
		Task* getNextTask();

	protected:
		TaskMap			m_taskQueue;				// �������
		TaskArray		m_taskFinished;				// ���������
		EE_MUTEX(		m_mutexTask);				// ������л�����
		EE_MUTEX(		m_mutexTaskFinished);		// ��ɶ��л�����
		ui32			m_totalTaskCount;			// �������
	};
}
