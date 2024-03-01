#ifndef MUSICPLAY_H
#define MUSICPLAY_H

#include "TinyTimber.h"
#include <stdint.h>

// Tempo constants
#define MIN_TEMPO 60
#define MAX_TEMPO 240

#define TEMPO_INCR 10

// Key constants
#define MAX_KEY 5
#define MIN_KEY -5

#define KEY_INCR 1


typedef struct {
	Object super;
	uint8_t tempo;
	int key;
}musicPlayerObj;

#define initmusicPlayer() { initObject(), 120, 0};

// Tempo controls
int getTempo(musicPlayerObj*, int);
int setTempo(musicPlayerObj*, int);

// Key controls
int getKey(musicPlayerObj*, int);
int setKey(musicPlayerObj*, int);





#endif