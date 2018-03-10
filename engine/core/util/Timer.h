#pragma once

#include "engine/core/Memory/MemManager.h"
#include "Singleton.h"

#ifdef ECHO_PLATFORM_WINDOWS
	#include <windows.h>
	#include <time.h>
#elif defined(ECHO_PLATFORM_MAC_IOS) || defined(ECHO_PLATFORM_MAC_OSX)
	#include <mach/mach_time.h>
#endif

namespace Echo
{
	class ECHO_EXPORT_FOUNDATION Time: public Singleton<Time>
	{	
	public:
		static Time* instance();
		static void replaceInstance(Time* instance);
		static void destroyInstance();

	public:
		// ����
		void reset();

		/** Returns milliseconds since initialisation or last reset */
		unsigned long getMilliseconds();

		/** Returns microseconds since initialisation or last reset */
		unsigned long getMicroseconds();

		/** Returns milliseconds since initialisation or last reset, only CPU time measured */	
		unsigned long getMillisecondsCPU();

		/** Returns microseconds since initialisation or last reset, only CPU time measured */	
		unsigned long getMicrosecondsCPU();

	private:
		Time();
		~Time();

	protected:
		clock_t mZeroClock;

#ifdef ECHO_PLATFORM_WINDOWS
		Echo::ui32 mStartTick;
		LONGLONG mLastTime;
		LARGE_INTEGER mStartTime;
		LARGE_INTEGER mFrequency;

		DWORD_PTR mTimerMask;

#elif defined(ECHO_PLATFORM_MAC_IOS) || defined(ECHO_PLATFORM_MAC_OSX)
        
        uint64_t m_IosStartTime;
        mach_timebase_info_data_t m_TimeBaseInfo;
        
#elif defined(ECHO_PLATFORM_ANDROID) || defined(ECHO_PLATFORM_HTML5)
		struct timespec m_AndroidStartTime;
#else 
		struct timeval	m_startTime;
#endif
	};
}
