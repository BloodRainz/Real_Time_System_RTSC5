#include "TinyTimber.h"
#include "toneGenerator.h"
#include "brotherJohn.h"
#include <stdlib.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES:
int i = 0;
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

void nextNote(ToneGenObj* self, int i)
{
	// Note controls
	int current_note = (self->key + f0_pos + song[i]);
	self->notePeriod = notes[current_note][1];
	
		// Tempo controls
	
	// Converting bpm to microseconds
	// NOTES:
	// - Beats are doubled, to ensure that fractions aren't present
	//   i.e. half beat = 1, single beat = 2, double beat = 4.
	// - Tempo is user set: for example, 120 bpm
	// - (Beat * tempo) / 2 converts to the correct beat, and accounts
	//   for beats being double.
	// - 60s / 120bpm = 0.5s period of a sound, however can't have fractions.
	//   To prevent this, USEC_MINUTE is the amount of microseconds in
	//   a minute, 60,000,000.
	// - 60,000,000us / 120bpm = 500,000us
	
	int temp_tempo = ((beats[i] * self->tempo) / 2); 
	int temp_tempo_us = (MSEC_MINUTE / temp_tempo) - 50;
	
	self->deadline = MSEC(temp_tempo_us);
	
	// This maths is very funky: so need to think of a better way to perform this
}

void toggle_DAC_output(ToneGenObj* self, int state)
{
	
	self->totalTime = T_SAMPLE(&self->timer);
	if(self->totalTime < self->deadline)
	{
		SEND(USEC(self->notePeriod), self->deadline, self, toggle_DAC_output, !state);
	}
	else
	{
		if(i < SONG_LENGTH)
		{
			i += 1;
		}
		else
		{
			i = 0;
		}
		// Should include processing of new values in this location to
		// prevent processing happening where it matters
		if(self->totalTime < (self->deadline + MSEC(SILENCE_TIME)))
		{
			BEFORE(MSEC(25), self, nextNote, i);
			BEFORE(MSEC(50), self, toggle_DAC_output,0);

			T_RESET(&self->timer);
			self->maxTime = 0;
		}
	}
	
	//startRecording(self, 0);
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
	
	//stopRecording(self, 0);
	
}


///////////////////////////////////////////////////////////////////////////////////
// TIMING

void startRecording(ToneGenObj* self, int)
{
	// Set the starting point to the current baseline
	self->start = CURRENT_OFFSET();
}

void stopRecording(ToneGenObj* self, int)
{
	// Set the end point to the current baseline
	self->end = CURRENT_OFFSET();
}

////////////////////////////////////////////////////////////////////////
// Trouble shooting with getValues
long getWCETStartTime(ToneGenObj* self, int)
{
	return USEC_OF(self->start);
}

long getWCETEndTime(ToneGenObj* self, int)
{
	return USEC_OF(self->end);
}

long getWCETTotalTime(ToneGenObj* self, int)
{
	return USEC_OF(self->totalTime);
}

long getWCETMaxTime(ToneGenObj* self, int)
{
	return self->maxTime;
}

long getDeadline(ToneGenObj* self, int unused)
{    
	return USEC_OF(self->deadline);
}



