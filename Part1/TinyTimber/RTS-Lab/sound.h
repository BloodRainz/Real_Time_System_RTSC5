#ifndef SOUND_H
#define SOUND_H

#include "TinyTimber.h"
#include <stdint.h>

#define DAC_Output (*((volatile uint8_t*) 0x4000741C))

#define MIN_VOLUME 1
#define MAX_VOLUME 6

#define VOL_INCR 1
typedef struct {
    Object super;
    uint8_t volume;
    uint8_t prev_sound;
    int period;
    char c;
	char buf[50];
} SoundObject;

#define initSound() { initObject(), 0, 0, 0, 'X', {0} };



int getSound(SoundObject*, int);
int setLevel(SoundObject*, int);
int mute(SoundObject*, int, int);
int setfrequency1Khz(SoundObject*, int);
int setperiod(SoundObject*, int);
void toggle_DAC_output(SoundObject*, int);
int Toggle(SoundObject*, int);

#endif
