#ifndef WCET_H
#define WCET_H

#include "TinyTimber.h"
#include <stdint.h>

#define RUNS 10000

typedef struct {
	Timer timer;
	Time start;
	Time end;
	int runs;
	Time totalTime;
	long average;
	long maxTime;
} WCET;

#define initWCET() {initTimer(), 0, 0, 0, 0, 0, 0}

long getWCETStartTime(WCET*, int unused);
long getWCETEndTime(WCET*, int unused);
long getWCETTotalTime(WCET*, int unused);

long getWCETMaxTime(WCET*, int unused);
long getWCETAverage(WCET*, int unused);

int getWCETLongRun(WCET*, int unused);

void startRecording(WCET*, int unused);
void stopRecording(WCET*, int unused);


#endif