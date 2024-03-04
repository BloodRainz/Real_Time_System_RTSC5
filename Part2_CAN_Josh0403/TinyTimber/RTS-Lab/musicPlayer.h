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
	Time deadline;
	int i;
	int notePeriod;
	int startStop;
}musicPlayerObj;

#define initmusicPlayer() { initObject(), 120, 0, 0, -1, 0, 0};

extern musicPlayerObj musicPlay;
// Tempo controls
int getTempo(musicPlayerObj*, int);
int setTempo(musicPlayerObj*, int);

// Key controls
int getKey(musicPlayerObj*, int);
int setKey(musicPlayerObj*, int);

int startStopPlayer(musicPlayerObj*, int);


int getPlayerStatus(musicPlayerObj*, int);

void nextNote(musicPlayerObj*, int);

int getI(musicPlayerObj*, int);

long getDeadline(musicPlayerObj*, int);

void silence(musicPlayerObj*, int);

#endif