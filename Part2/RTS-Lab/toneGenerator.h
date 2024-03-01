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

#define PULSE_DL 100

#define RUNS 20000

typedef struct {
    Object super;
    uint8_t volume;
    uint8_t prev_volume;
	uint8_t tempo;
	int key;
    int notePeriod;
	Time deadline;

	Timer timer;
	Time start;
	Time end;
	int runs;
	Time totalTime;
	long average;
	long maxTime;
	int u_mute;
} ToneGenObj;

#define initToneGen() { initObject(), 2, 0,120, 0, 0,0,  initTimer(), 0, 0, 0, 0, 0, 0}

extern ToneGenObj toneGenerator;
// Volume controls
int getVolume(ToneGenObj*, int);
int setVolume(ToneGenObj*, int);
void set_user_mute(ToneGenObj*, int user_mute);
int mute(ToneGenObj*, int user_mute);

void updateTempo(ToneGenObj*, int newtempo);
void updateKey(ToneGenObj*, int newkey); 

void toggle_DAC_output(ToneGenObj*, int);

void startToneTiming(ToneGenObj*, int);
void timingControls(ToneGenObj*, int);
void stopToneTiming(ToneGenObj*, int);

int setNote(ToneGenObj*, int);
// TROUBLESHOOT METHODS
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

void updateDeadline(ToneGenObj*, Time newDeadline);
void updateNotePeriod(ToneGenObj*, int newNotePeriod);

int getKeyTG(ToneGenObj*, int newKey);
int getTempoTG(ToneGenObj*, int newTempo);



#endif