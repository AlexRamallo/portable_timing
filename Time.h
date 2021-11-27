#ifndef __TIMESYS__H__
#define __TIMESYS__H__
#include <inttypes.h>

namespace TimeSys {

	void init(void *data);

	uint64_t getTicks();

	/**
		Returns the number of ticks per second.

		Example:
			auto start = getTicks();
			slow_func();
			auto elapsedTicks = getTicks() - start;
			auto elapsedSeconds = elapsedTicks / getFrequency();
	*/
	uint64_t getFrequency();

	uint32_t getMillis();

	/**
		Sleeps for the specified number of ticks (or the
		smallest possible sleep interval)
	*/
	void sleepTicks(uint64_t ticks);
	void sleepMillis(uint32_t millis);
};
#endif