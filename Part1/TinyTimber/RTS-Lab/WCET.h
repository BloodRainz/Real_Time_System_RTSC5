#ifndef WCET_H
#define WCET_H

#include "TinyTimber.h"
#include <stdint.h>

#define RUNS 500

typedef struct {
	Timer timer;
	Time start;
	int runs;
	Time totalTime;
	int average;
	int maxTime;
} WCET;

#define initWCET() {initTimer(), 0, RUNS, 0, 0, 0}

int getStart(WCET*, int unused);
int getMaxTime(WCET*, int unused);

long getWCETMaxTime(WCET*, int unused);
long getWCETAverage(WCET*, int unused);
long getWCETTotalTime(WCET*, int unused);

void startRecording(WCET*, int unused);
void stopRecording(WCET*, int unused);


#endif