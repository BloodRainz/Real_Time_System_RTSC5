#ifndef TONEGEN_H
#define TONEGEN_H

#include "TinyTimber.h"
#include <stdint.h>
#include "WCET.h"

#define DAC_Output (*((volatile uint8_t*) 0x4000741C))

// Volume constants
#define MIN_VOLUME 1
#define MAX_VOLUME 20

#define VOL_INCR 1

// Tempo constants
#define MIN_TEMPO 60
#define MAX_TEMPO 240

#define TEMPO_INCR 10

// Key constants
#define MAX_KEY 5
#define MIN_KEY -5

#define KEY_INCR 1

#define SILENCE_TIME 50

typedef struct {
    Object super;
    uint8_t volume;
    uint8_t prev_volume;
	uint8_t tempo;
	int key;
    int notePeriod;
	int deadline;
	int runs;
	WCET wcet;
} ToneGenObj;

#define initToneGen() { initObject(), 0, 0, 120, 0, 0, 0, 0, initWCET()}

// Volume controls
int getVolume(ToneGenObj*, int);
int setVolume(ToneGenObj*, int);
int mute(ToneGenObj*, int);

// Tempo controls
int getTempo(ToneGenObj*, int);
int setTempo(ToneGenObj*, int);

// Key controls
int getKey(ToneGenObj*, int);
int setKey(ToneGenObj*, int);



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
#endif