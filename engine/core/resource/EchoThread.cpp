#include "Engine/Core.h"
#include "EchoThread.h"
#include "engine/core/Util/Exception.h"
#include "engine/core/Util/LogManager.h"
#include "Engine/core/Scene/Scene_Manager.h"
#include "engine/core/Util/AssertX.h"
#ifndef ECHO_PLATFORM_WINDOWS
#include <unistd.h>
#endif
namespace Echo
{
	//////////////////////////////////////////////////////////////////////////
	enum EVENT_SCHEDULER
	{
		ES_LOW		= 3,
		ES_NORMAL	= 6,
		ES_HIGHT	= 0x0fffffff
	};
	enum EVENT_LEVEL
	{
		E_NORMAL	= 1,
		E_HIGHT		= 2,

		E_MAX_COUNT	= 3
	};

#ifdef ECHO_PLATFORM_WINDOWS
	class Event
	{
		HANDLE mEvent;

	public:
		Event(void);
		~Event(void);
		void Create();
		void Destroy();
		void Trigger(void);
		void Wait();
	};
#else
	class Event
	{
		inline void LockEventMutex();
		inline void UnlockEventMutex();
		bool bInitialized;
		pthread_mutex_t Mutex;
		pthread_cond_t Condition;

	public:
		Event(void);
		~Event(void);
		void Create();
		void Destroy();
		void Trigger(void);
#ifdef _DEBUG
		bool Wait();
#else 
		void Wait();
#endif
	private:
		unsigned short state;
	};
#endif

#ifdef ECHO_PLATFORM_HTML5
	// html5 doesn't support html5 well.
	Event::Event(void){}
	Event::~Event(void){}
	void Event::Create(){}
	void Event::Destroy(){}
	void Event::Trigger(void){}
	void Event::Wait(){}

#elif defined(ECHO_PLATFORM_WINDOWS)
	/////////////////////////////////////////////////////////////////////////
	//Wiindows Event
	Event::Event(void)
	{
		mEvent = NULL;
	}
	Event::~Event(void)
	{
		Destroy();
	}
	void Event::Create()
	{
		mEvent = CreateEvent(NULL, true, 0, NULL);
	}
	void Event::Destroy()
	{
		if (mEvent != NULL)
		{
			CloseHandle(mEvent);
			mEvent = NULL;	
		}
	}
	void Event::Trigger(void)
	{
		SetEvent(mEvent);
	}
	void Event::Wait()
	{
		WaitForSingleObject(mEvent, INFINITE);
		ResetEvent(mEvent);
	}
#else

	/////////////////////////////////////////////////////////////////////////
	//Iphone Event
	Event::Event(void)
	{
		state = 0;
		bInitialized = false;
	}
	Event::~Event(void)
	{
		Destroy();
	}
	void Event::LockEventMutex()
	{
		pthread_mutex_lock(&Mutex);
	}
	void Event::UnlockEventMutex()
	{
		pthread_mutex_unlock(&Mutex);
	}
	void Event::Create()
	{
		EchoAssert(!bInitialized);

		if (pthread_mutex_init(&Mutex, NULL) == 0)
		{
			if (pthread_cond_init(&Condition, NULL) == 0)
			{
				bInitialized = true;
			}
			else
			{
				pthread_mutex_destroy(&Mutex);
				EchoException("Error: error in Event::Create");
			}
		}
	}
	void Event::Destroy()
	{
		if (bInitialized)
		{
			LockEventMutex();
			pthread_cond_destroy(&Condition);
			UnlockEventMutex();
			pthread_mutex_destroy(&Mutex);
			bInitialized = false;
		}
	}
	void Event::Trigger(void)
	{
		LockEventMutex();
		pthread_cond_signal(&Condition);
		state = 1;
		UnlockEventMutex();
	}

#ifdef _DEBUG
	bool Event::Wait()
	{
		bool ret = false;
		LockEventMutex();
		if(state == 0)
		{
			pthread_cond_wait(&Condition, &Mutex);
			ret = true;
		}
		state = 0;
		UnlockEventMutex();
		return ret;
	}
#else
	void Event::Wait()
	{
		LockEventMutex();
		if(state == 0)
		{
			pthread_cond_wait(&Condition, &Mutex);
		}
		state = 0;
		UnlockEventMutex();
	}
#endif

#endif

	// ���캯��
	ThreadLoopObject::ThreadLoopObject()
		: m_state(TS_ShutingDown)
		, m_hThread(NULL)
	{

	}

	ThreadLoopObject::~ThreadLoopObject()
	{

	}

#ifdef ECHO_PLATFORM_WINDOWS
	// �̺߳���
	unsigned __stdcall ThreadLoopObject::ThreadCallBackProc(void *pvArg)
	{
		(static_cast<ThreadLoopObject *>(pvArg))->processLoop();

		return 0;
	}
#else
	// �̺߳���
	void* ThreadLoopObject::ThreadCallBackProc(void *pvArg)
	{
		(static_cast<ThreadLoopObject *>(pvArg))->processLoop();

		return 0;
	}
#endif

	void ThreadLoopObject::start()
	{
		if (m_state!=TS_ShutingDown)
			return;

		m_threadEvent = EchoNew( Event);
		m_threadEvent->Create();

#ifdef ECHO_PLATFORM_WINDOWS
		m_hThread = (HANDLE)_beginthreadex(
			NULL,			        	// SD
			0,				        	// initial stack size
			ThreadLoopObject::ThreadCallBackProc,			// thread function
			(void*)this, 	// thread argument
			0,				        	// creation option
			NULL						// thread identifier
			);	
		if (!m_hThread)
		{
			EchoException("Error: _beginthreadex fail! Thread::Init");
			//return false;
		}
#elif defined(ECHO_PLATFORM_HTML5)
		// html5 don't support multithread well.
		// do nothing
#else
		int error;
		error = pthread_create((pthread_t*)&m_hThread, NULL, ThreadLoopObject::ThreadCallBackProc, (void*)this);
		if(error != 0)
		{
			EchoException("Error: _beginthreadex fail! Thread::Init");
			//return false;
		}
#endif

		m_state = TS_Running;
		mIsPause= false;

		EchoLogInfo("Thread Pool Startuped");
	}

	// �ر��߳�ѭ��
	void ThreadLoopObject::shutdown()
	{
		if (m_state!=TS_Running)
			return;

		m_state = TS_ShutingDown;

		m_threadEvent->Trigger();

		if (m_hThread)
		{
#ifdef ECHO_PLATFORM_WINDOWS
			WaitForSingleObject(m_hThread, INFINITE);
			CloseHandle(m_hThread);
#endif
			m_hThread = NULL;
		}

		m_threadEvent->Destroy();
		EchoSafeDelete(m_threadEvent, Event);

		EchoLogInfo("Stream thread Ended.");
	}
	//-----------------------------------------------------------------------
	bool ThreadLoopObject::IsRunning()
	{
		return m_state==TS_Running;
	}

	bool ThreadLoopObject::IsPause()
	{
		return mIsPause;
	}

	// ��ͣ�߳�
	void ThreadLoopObject::pause()
	{
		mIsPause = true;
		m_threadEvent->Wait();
	}

	// �ָ��߳�
	void ThreadLoopObject::resume()
	{
		mIsPause = false;
		m_threadEvent->Trigger();
	}

	// ���캯��
	StreamThread::StreamThread()
		: ThreadLoopObject()
		, m_totalTaskCount( 0)
	{
	}

	// ��������
	StreamThread::~StreamThread()
	{
	}

	// �������
	ui32 StreamThread::addTask( StreamThread::Task* pTask)
	{
		EchoAssert( pTask);

		if ( m_state!=TS_Running)
		{
			pTask->process();
			pTask->finished();
			return 0;
		}
		else
		{
			m_totalTaskCount++;
			{
				EE_LOCK_MUTEX( m_mutexTask);
				m_taskQueue.insert( std::make_pair(pTask->GetLevel(), pTask));
			}

			// �����߳�
			resume();

			return m_totalTaskCount;
		}
	}

	// ����ѭ��
	void StreamThread::processLoop()
	{
#ifdef ECHO_PLATFORM_HTML5
		if (!mShuttingDown)
#else
		while (m_state==TS_Running)
#endif
		{
			Task* pNextTask = getNextTask();
			if ( pNextTask)
			{
				pNextTask->process();
				{
					EE_LOCK_MUTEX( m_mutexTaskFinished);
					m_taskFinished.push_back( pNextTask);
				}
			}
			else
			{
				pause();
			}
		}
	}

	// ֪ͨ���������
	void StreamThread::notifyTaskFinished()
	{
		vector<Task*>::type finishedTasks; finishedTasks.reserve( 128);
		{
			EE_LOCK_MUTEX( m_mutexTaskFinished);
			for( TaskArray::iterator it=m_taskFinished.begin(); it!=m_taskFinished.end(); it++)
				finishedTasks.push_back( *it);

			m_taskFinished.clear();
		}
										
		// ����������ɽӿ�
		for( size_t i=0; i<finishedTasks.size(); i++)
		{
			Task* curTask = finishedTasks[i];

			// δ������񷵻���ɶ���
			if( !curTask->finished() && curTask->isRepeatNotifyIfFail())
			{
				EE_LOCK_MUTEX( m_mutexTaskFinished);
				m_taskFinished.push_back( finishedTasks[i]);
			}
			else if( curTask->isAutoDestroy())
			{
				EchoSafeDelete( curTask, Task);
			}
		}
	}

	// �ر�
	void StreamThread::shutdown()
	{
		ThreadLoopObject::shutdown();
	}

	// ��ȡ��һ������
	StreamThread::Task* StreamThread::getNextTask()
	{
		EE_LOCK_MUTEX( m_mutexTask);
		if ( !m_taskQueue.empty())
		{
			TaskMap::iterator beginIt = m_taskQueue.begin();
			Task* pNextTask = beginIt->second;
			m_taskQueue.erase( beginIt);

			return pNextTask;
		}

		return NULL;
	}

	// ��ȡ��������
	int StreamThread::getTaskCount()
	{
		return (int)m_taskQueue.size();
	}

	// �������������
	int StreamThread::getTaskFinishedCount()
	{
		return (int)m_taskFinished.size();
	}
}
