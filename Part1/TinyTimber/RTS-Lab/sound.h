#ifndef _SOUND_
#define _SOUND_

#include "TinyTimber.h"

#include <stdlib.h>
#include <stdio.h>

#define DAC_Output (*((volatile uint8_t*) 0x4000741C))

typedef struct {
	Object super;
	char buf[50];
	uint8_t  volume;
	uint8_t  prev_volume;
	int peri;
	char c;
} Sound;

#define initSound() { initObject(), {0}, 0, 0, 0, 'X'};

extern Sound sound;

typedef struct{
    Object super;
    int Background_loop_range;
} Backgroundtask;

#define initBackgroundtask() {initObject(), 0};

extern Backgroundtask backgroundtask;

int getSound(Sound*, int);
int setLevel(Sound*, int);
int mute(Sound*, int, int);
int setfrequency1Khz(Sound*, int);
int setperiod(Sound*, int);
void toggle_DAC_output(Sound*, int);
int Toggle(Sound*, int);
void BackgroundLOOP(Backgroundtask*, int);
void load(Backgroundtask*, int);

#endif
