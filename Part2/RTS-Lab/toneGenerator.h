#ifndef TONEGEN_H
#define TONEGEN_H

#include "TinyTimber.h"
#include <stdint.h>
//#include "WCET.h"

#define DAC_Output (*((volatile uint8_t*) 0x4000741C))

// Volume constants
#define MIN_VOLUME 1
#define MAX_VOLUME 20

#define VOL_INCR 1



#define USEC_MINUTE 60000000
#define MSEC_MINUTE 60000
#define SILENCE_TIME 50

#define RUNS 20000

typedef struct {
    Object super;
    uint8_t volume;
    uint8_t prev_volume;
	uint8_t tempo;
	int key;
    int notePeriod;
	Time deadline;
	int i;
	Timer timer;
	Time start;
	Time end;
	int runs;
	Time totalTime;
	long average;
	long maxTime;
} ToneGenObj;

#define initToneGen() { initObject(), 2, 0,120, 0, 0, 0, 0, initTimer(), 0, 0, 0, 0, 0, 0}

// Volume controls
int getVolume(ToneGenObj*, int);
int setVolume(ToneGenObj*, int);
int mute(ToneGenObj*, int);

void updateTempo(ToneGenObj*, int newtempo);
void updateKey(ToneGenObj*, int newkey); 

void toggle_DAC_output(ToneGenObj*, int);

void startToneTiming(ToneGenObj*, int);
void timingControls(ToneGenObj*, int);
void stopToneTiming(ToneGenObj*, int);

int setNote(ToneGenObj*, int);
// TROUBLESHOOT METHODS
long getDeadline(ToneGenObj*, int);
long getStart(ToneGenObj*, int);
long getEnd(ToneGenObj*, int);
long getTotalTime(ToneGenObj*, int);

long getWCETStartTime(ToneGenObj*, int unused);
long getWCETEndTime(ToneGenObj*, int unused);
long getWCETTotalTime(ToneGenObj*, int unused);

long getWCETMaxTime(ToneGenObj*, int unused);
long getWCETAverage(ToneGenObj*, int unused);

int getWCETLongRun(ToneGenObj*, int unused);

void startRecording(ToneGenObj*, int unused);
void stopRecording(ToneGenObj*, int unused);

void nextNote(ToneGenObj*, int);

int getI(ToneGenObj*, int);

#endif