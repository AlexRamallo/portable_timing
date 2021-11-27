#include "Time.h"
#include <stdio.h>
#include <cstring>

void sleep_sec(int sec);
void sleep_ticks(uint64_t ticks);
void stopwatch_start();
void stopwatch_end();

void test_sleep(){
	uint64_t freq = TimeSys::getFrequency();
	printf("\tClock frequency: %" PRIu64 " ticks per second\n", freq);

	printf("MILLIS:\n");
	stopwatch_start();
	sleep_sec(1);
	sleep_sec(2);
	sleep_sec(3);
	sleep_sec(4);
	sleep_sec(5);
	stopwatch_end();

	printf("TICKS:\n");
	stopwatch_start();
	sleep_ticks(freq * 1);
	sleep_ticks(freq * 2);
	sleep_ticks(freq * 3);
	sleep_ticks(freq * 4);
	sleep_ticks(freq * 5);
	stopwatch_end();
}

void test_loop(){
	uint64_t freq = TimeSys::getFrequency();
	uint32_t start_ms = TimeSys::getMillis();
	uint64_t start_ticks = TimeSys::getTicks();

	uint64_t ct = 0;
	while(1){
		if(++ct >= 100000000){
			ct = 0;
			uint32_t now_ms = TimeSys::getMillis();
			uint64_t now_ticks = TimeSys::getTicks();
			uint32_t elapsed_ms = now_ms - start_ms;
			uint64_t elapsed_ticks = now_ticks - start_ticks;
			uint64_t sec = elapsed_ticks / freq;
			printf("Elapsed ms: %u\tNow ms: %u\tElapsed ticks: %" PRIu64 " (sec: %" PRIu64 ")\n", elapsed_ms, now_ms, elapsed_ticks, sec);
		}
	}
}

int main(int argc, char *argv[]){
	printf("Initialize timer\n");
	TimeSys::init(nullptr);

	if(argc == 2){
		if(!strcmp("loop", argv[1])){
			test_loop();
			return 0;
		}
	}
	test_sleep();
	return 0;
}

void sleep_sec(int sec){
	printf("Sleeping for %u seconds...\n", sec);
	TimeSys::sleepMillis(sec * 1000);
	printf("\tDone!\n");
}

void sleep_ticks(uint64_t ticks){
	printf("Sleeping for %" PRIu64 " ticks...\n", ticks);
	TimeSys::sleepTicks(ticks);
	printf("\tDone!\n");
}

uint32_t sw_start_ms;
uint64_t sw_start_ticks;

void stopwatch_start(){
	sw_start_ms = TimeSys::getMillis();
	sw_start_ticks = TimeSys::getTicks();

	printf("stopwatch_start (ms: %u, ticks: %" PRIu64 ")\n", sw_start_ms, sw_start_ticks);
}

void stopwatch_end(){
	uint32_t end_ms = TimeSys::getMillis();
	uint64_t end_ticks = TimeSys::getTicks();

	uint32_t elapsed_ms = end_ms - sw_start_ms;
	uint64_t elapsed_ticks = end_ticks - sw_start_ticks;

	printf("stopwatch_end (ms: %u, ticks: %" PRIu64 ")\n", end_ms, end_ticks);
	printf("\telapsed ms: %u\telapsed ticks: %" PRIu64 "\n", elapsed_ms, elapsed_ticks);
}