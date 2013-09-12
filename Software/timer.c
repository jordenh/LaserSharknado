#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "sys/alt_timestamp.h"
#include "sys/alt_alarm.h"
#include "system.h"
#include "timer.h"

// Test code from lab
void timer_test(void) {
	int freq;
	int cycles;
	float duration;
	int ticks_start;
	int ticks_end;
	int ticks_per_s;
	int ticks_duration;
	int timer_period;
	int status;
	int done;

	printf("Timers\n");
	printf(" Sys Clock Timer\n");
	ticks_per_s = alt_ticks_per_second();
	printf("Tick Freq: %d\n", ticks_per_s);
	printf(" Recording starting ticks\n");
	ticks_start = alt_nticks();
	printf(" Sleeping for 5 seconds\n");
	usleep(5000000);
	printf(" Recording ending ticks\n");
	ticks_end = alt_nticks();
	ticks_duration = ticks_end -ticks_start;
	duration = (float) ticks_duration / (float) ticks_per_s;
	printf(" The program slept for %d ticks (%f seconds)\n\n", ticks_duration,
	duration);
	printf(" Timestamp Timer\n");
	freq = alt_timestamp_freq();
	printf(" CPU Freq: %d\n", freq);
	printf(" Resetting Timestamp timer\n");
	alt_timestamp_start();
	printf(" ...Timing the print of this statement...\n");
	cycles = alt_timestamp();
	duration = (float) cycles / (float) freq;
	printf(" It took %d cycles (%f seconds) to print the statement\n\n",
	cycles, duration);
	printf(" Hardware-Only Timer\n");
	printf(" Setting timer period to 5 seconds.\n");
	timer_period = 5 * 50000000;
	IOWR_16DIRECT(MY_HW_ONLY_TIMER_BASE, 8, timer_period & 0xFFFF);
	IOWR_16DIRECT(MY_HW_ONLY_TIMER_BASE,12, timer_period >> 16);
	printf(" Stopping Timer\n");
	status = IORD_16DIRECT(MY_HW_ONLY_TIMER_BASE, 0);
	if(status & 0x2) {
		IOWR_16DIRECT(MY_HW_ONLY_TIMER_BASE, 4, 1 << 3);
	}

	printf(" Starting Timer\n");
	IOWR_16DIRECT(MY_HW_ONLY_TIMER_BASE, 4, 1 << 2);
	printf("  Waiting for timer to expire...\n");
	done = 0;
	while(! done) {
		status = IORD_16DIRECT(MY_HW_ONLY_TIMER_BASE, 0);
		done = status & 0x1;
	}
	printf(" 5 seconds timer is done\n");
}

