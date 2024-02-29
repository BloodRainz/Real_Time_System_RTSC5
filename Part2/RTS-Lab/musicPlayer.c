#include "TinyTimber.h"
#include "musicPlayer.h"
#include <stdlib.h>
#include <stdio.h>





/////////////////////////////////////////////////////////////////
//
// TEMPO CONTROLS

// Return the current tempo of the system
int getTempo(musicPlayerObj* self, int unused)
{    
	return self->tempo;
}

// Set the volume of the system, if it is within volume constraints.
// If within constraints, return the new volume.
// If not, do not change the volume, and return a -1.

int setTempo(musicPlayerObj* self, int tempo)
{
    if((tempo > MIN_TEMPO - 1) && (tempo < MAX_TEMPO+1))
	{
		self->tempo = tempo;
		return tempo;
    }
    else
	{
		self->tempo = self->tempo; // Line may not be necessary
		return -1;
	}
}

////////////////////////////////////////////////////////////////////////
//
// KEY CONTROLS
//
//

// Return the current key of the system
int getKey(musicPlayerObj* self, int unused)
{    
	return self->key;
}

// Set the key of the system, if it is within key constraints.
// If within constraints, return the new key.
// If not, do not change the key, and return a -1.

int setKey(musicPlayerObj* self, int key)
{
    if((key > MIN_KEY-1) && (key < MAX_KEY+1))
	{
		self->key = key;
		return key;
    }
    else
	{
		self->key = self->key; // Line may not be necessary
		return MAX_KEY+1;
	}
}