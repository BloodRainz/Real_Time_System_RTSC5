#ifndef SOUND_H
#define SOUND_H

#include "TinyTimber.h"
#include <stdint.h>

#include "WCET.h"

#define DAC_Output (*((volatile uint8_t*) 0x4000741C))

#define MIN_VOLUME 1
#define MAX_VOLUME 20

#define VOL_INCR 1

#define SOUND_DEADLINE USEC(100)

typedef struct {
    Object super;
    uint8_t volume;
    uint8_t prev_sound;
    int notePeriod;
	int enableDl;
	
//	WCET soundWCET;
	
} SoundObject;

#define initSound() { initObject(), 0, 0, 500, 0};

//#define initSound() { initObject(), 0, 0, 500, 0, initWCET()};



int getSound(SoundObject*, int);
int setLevel(SoundObject*, int);
int mute(SoundObject*, int, int);
int setfrequency1Khz(SoundObject*, int);
int setperiod(SoundObject*, int);
void toggle_DAC_output(SoundObject*, int);

int statusSoundDeadline(SoundObject*, int);
int enableSoundDeadline(SoundObject*, int);

#endif
