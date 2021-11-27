#include "Time.h"

#if defined(_WIN32)
//Windows

#include <Windows.h>
#include <assert.h>

static uint64_t frequency;
static uint64_t frequencyMs;

namespace TimeSys {

	void init(void *data) {
		LARGE_INTEGER freq;
		if(!QueryPerformanceFrequency(&freq)){
			assert(false && "QueryPerformanceFrequency failed!");
		}
		frequency = (uint64_t) freq.QuadPart;
		frequencyMs = frequency / 1000;
	}

	uint64_t getTicks() { 
		LARGE_INTEGER ticks;
		QueryPerformanceCounter(&ticks);
		return (uint64_t) ticks.QuadPart;
	}

	uint64_t getFrequency() { 
		return frequency;
	}

	uint32_t getMillis() {
		LARGE_INTEGER ticks;
		QueryPerformanceCounter(&ticks);
		return (uint64_t)(ticks.QuadPart / frequency);
	}

	void sleepTicks(uint64_t ticks) {
		Sleep(ticks / frequencyMs);
	}

	void sleepMillis(uint32_t millis) {
		Sleep(millis);
	}

} //namespace TimeSys

#elif defined(__MACH__)
//Mac/iOS

#include <CoreServices/CoreServices.h>

namespace TimeSys {

	void init(void *data) {
		//--
	}

	uint64_t getTicks() { 
		return clock_gettime_nsec_np(CLOCK_UPTIME_RAW);
	}

	uint64_t getFrequency() { 
		return 1000000000;
	}

	uint32_t getMillis() {
		return clock_gettime_nsec_np(CLOCK_UPTIME_RAW) / 1000000;
	}

	void sleepTicks(uint64_t ticks) {
		timespec ts;
		ts.tv_sec = ticks / 1000000000;
		ts.tv_nsec = ticks % 1000000000;
		nanosleep(&ts, NULL);
	}

	void sleepMillis(uint32_t millis) {
		sleepTicks((uint64_t)millis * 1000000);
	}

} //namespace TimeSys

#else
//Linux/Android/probably BSD

#include <time.h>
#include <unistd.h>

namespace TimeSys {

	static uint64_t frequency = 0, start = 0;

	#define SEC_TO_NSEC(sec) (sec * 1000000000)
	#define NSEC_TO_USEC(nsec) (nsec / 1000)

	void init(void *data) {
		//calcluate and store clock frequency at init time

		timespec ts;
		clock_getres(CLOCK_MONOTONIC, &ts);

		frequency = 1;
		if(ts.tv_sec != 0)
			frequency += 1.0 / (double) ts.tv_sec;

		if(ts.tv_nsec != 0)
			frequency += 1000000000.0 / (double) ts.tv_nsec;

		start = getTicks();
	}

	/**
		Ticks are nanoseconds in this implementation
	*/
	uint64_t getTicks() { 
		timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);
		return SEC_TO_NSEC(((uint64_t)ts.tv_sec)) + ((uint64_t)ts.tv_nsec) - start;
	}

	uint64_t getFrequency() { 
		return frequency;
	}

	uint32_t getMillis() { 
		uint64_t now_ticks = getTicks();
		uint64_t t_msec = (uint64_t)(((double)now_ticks / (double)frequency) * 1000.0);
		return (uint32_t) t_msec;
	}

	void sleepTicks(uint64_t ticks) {
		usleep((useconds_t) NSEC_TO_USEC(ticks));
	}

	void sleepMillis(uint32_t millis) {
		usleep((useconds_t)(millis * 1000));
	}

} //namespace TimeSys

#endif