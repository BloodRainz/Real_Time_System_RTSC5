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


typedef struct {
    Object super;
    uint8_t volume;
    uint8_t prev_volume;
	uint8_t tempo;
	int key;
    int notePeriod;
	Time deadline;
} ToneGenObj;

#define initToneGen() { initObject(), 2, 0, 120, 0, 0, 0}

extern ToneGenObj toneGenerator;
// Volume controls
int getVolume(ToneGenObj*, int);
int setVolume(ToneGenObj*, int);

int application_mute(ToneGenObj*, int unused);
int mute(ToneGenObj*, int);

void updateTempo(ToneGenObj*, int newtempo);
void updateKey(ToneGenObj*, int newkey); 

void toggle_DAC_output(ToneGenObj*, int);

void updateDeadline(ToneGenObj*, Time newDeadline);
void updateNotePeriod(ToneGenObj*, int newNotePeriod);

int getKeyTG(ToneGenObj*, int newKey);
int getTempoTG(ToneGenObj*, int newTempo);

int getVolumeTG(ToneGenObj*, int unusued);
int getPrevVolumeTG(ToneGenObj*, int unusued);



#endif