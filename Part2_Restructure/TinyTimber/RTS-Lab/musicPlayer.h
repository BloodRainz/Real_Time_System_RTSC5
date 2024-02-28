#ifndef _MUSICPLAYER_H_
#define _MUSICPLAYER_H_

#include "TinyTimber.h"
#include <stdint.h>

typedef struct {
    Object super;
	int current_key;			// user defined key
	int current_volume;			// Holder for the current volume: will help reduce SYNCS
	int current_tempo;			// Holder for the current tempo: will help reduce SYNCs
	uint8_t tempo;
	int key;
	int i;
} MusicPlayerObj;

#define initMusicPlayer() {initObject(), 0, 2, 0, 120, 0, 0, 0, 0}

// Tempo controls
int getTempo(MusicPlayerObj*, int);
int setTempo(MusicPlayerObj*, int);

// Key controls
int getKey(MusicPlayerObj*, int);
int setKey(MusicPlayerObj*, int);

void nextNote(MusicPlayerObj*, int);



#endif