#include "TinyTimber.h"
#include "toneGenerator.h"
#include "brotherJohn.h"
#include <stdlib.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES:

//////////////////////////////////////////////////////////////////////////////////
//
// VOLUME CONTROLS:
//
// Return the current volume of the system
int getVolume(ToneGenObj* self, int unused)
{    
	return self->volume;
}

// Set the volume of the system, if it is within volume constraints.
// If within constraints, return the new volume.
// If not, do not change the volume, and return a -1.

int setVolume(ToneGenObj* self, int volume)
{
    if((volume > MIN_VOLUME - 1) && (volume < MAX_VOLUME+1))
	{
		self->volume = volume;
		return volume;
    }
    else
	{
		self->volume = self->volume; // Line may not be necessary?
		return -1;
	}
}

// Mute the DAC: essentially set the volume to zero, and save
// the previous volume.
// If unmuted, return to the original volume.
int mute(ToneGenObj* self, int unused)
 {
	if(self->volume == 0)
	{
		self->volume = self->prev_volume;
	}
	else
	{
		self->prev_volume = self->volume;
		self->volume = 0;
	}
	return self->volume;
 }
 
/////////////////////////////////////////////////////////////////
//
// TEMPO CONTROLS

// Return the current tempo of the system
int getTempo(ToneGenObj* self, int unused)
{    
	return self->tempo;
}

// Set the volume of the system, if it is within volume constraints.
// If within constraints, return the new volume.
// If not, do not change the volume, and return a -1.

int setTempo(ToneGenObj* self, int tempo)
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
int getKey(ToneGenObj* self, int unused)
{    
	return self->key;
}

// Set the key of the system, if it is within key constraints.
// If within constraints, return the new key.
// If not, do not change the key, and return a -1.

int setKey(ToneGenObj* self, int key)
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

void toggle_DAC_output(ToneGenObj* self, int state)
{
	int i = 0;
	
	int current_note = (self->key + f0_pos + song[i]);
	self->notePeriod = notes[current_note][1];
	self->deadline = ((beats[i] * self->tempo) - 50);
	
	SEND(USEC(self->notePeriod), 0, self, toggle_DAC_output, !state);
	
	startRecording(&self->wcet, 0);
	// Allows for changing of volume
	static int DAC_value;
	if(state == 0){
		DAC_value = 0;
	}
	else
	{
		DAC_value = self->volume;
	}
	DAC_Output = DAC_value;
	
	stopRecording(&self->wcet, 0);
	
}





///////////////////////////////////////////////////////////////////////////////////
//
//
// TROUBLESHOOTING METHODS

long getDeadline(ToneGenObj* self, int unused)
{    
	return self->deadline;
}


