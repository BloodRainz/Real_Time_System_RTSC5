#ifndef SOUND_H
#define SOUND_H

#include "TinyTimber.h"
#include <stdint.h>

typedef struct {
    Object super;
    uint8_t volume;
    uint8_t prev_sound;
    int period;
    char c;
char buf[50];
} SoundObject;

//typedef struct{
   // Object super;
   // int Background_loop_range;

//} Backgroundtask;
//Backgroundtask backgroundtask = {initObject(), 1000};

#define initSound() { initObject(), 0, 0, 0, 'X', {0} };
int getSound(SoundObject*, int);
int setLevel(SoundObject*, int);
int mute(SoundObject*, int, int);
int setfrequency1Khz(SoundObject*, int);
int setperiod(SoundObject*, int);
void toggle_DAC_output(SoundObject*, int);
int Toggle(SoundObject*, int);
//void BackgroundLOOP(Backgroundtask* self, int Background_loop_range);
//void load(Backgroundtask* self, int Background_loop_range);

#define DAC_Output (*((volatile uint8_t*) 0x4000741C))
#endif
