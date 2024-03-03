#ifndef TONEGEN_H
#define TONEGEN_H

#include "TinyTimber.h"
#include <stdint.h>
//#include "WCET.h"

#define DAC_Output (*((volatile uint8_t*) 0x4000741C))

// Volume constants
#define MIN_VOLUME 1
#define MAX_VOLUME 5

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
	int u_mute;
} ToneGenObj;

#define initToneGen() { initObject(), 2, 2, 120, 0, 0, 0}

extern ToneGenObj toneGenerator;
// Volume controls
int getVolume(ToneGenObj*, int);
int setVolume(ToneGenObj*, int);
void set_user_mute(ToneGenObj*, int user_mute);
int getUserMute(ToneGenObj*, int);
int mute(ToneGenObj*, int unused);
int unmute(ToneGenObj*, int unused);


void updateTempo(ToneGenObj*, int newtempo);
void updateKey(ToneGenObj*, int newkey); 

void toggle_DAC_output(ToneGenObj*, int);

void startToneTiming(ToneGenObj*, int);
void timingControls(ToneGenObj*, int);
void stopToneTiming(ToneGenObj*, int);

int setNote(ToneGenObj*, int);
// TROUBLESHOOT METHODS
long getTotalTime(ToneGenObj*, int);

void updateNotePeriod(ToneGenObj*, int newNotePeriod);

int getKeyTG(ToneGenObj*, int newKey);
int getTempoTG(ToneGenObj*, int newTempo);


int get_volume_debug(ToneGenObj*, int unused);
int get_prev_volume_debug(ToneGenObj* self, int unused);

#endif