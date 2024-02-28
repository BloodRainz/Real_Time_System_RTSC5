#include "TinyTimber.h"
#include "toneGenerator.h"
#include "brotherJohn.h"
#include <stdlib.h>
#include <stdio.h>

#include "musicPlayer.h"
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

void startTotalTime(ToneGenObj* self, int unused)
{
	self->totalTime = T_SAMPLE(&self->timer);
}
// Note controls
int getNotePeriod(ToneGenObj* self, int i)
{
	int current_note = (self->key + f0_pos + song[self->i]);
	return notes[current_note][1];
}

long setDeadline(ToneGenObj* self, int i)
{
	int temp_tempo = ((beats[self->i] * self->tempo) / 2); 
	int temp_tempo_us = (MSEC_MINUTE / temp_tempo);
	int temp_tempo_minus50 = temp_tempo_us - 50;
	
	return temp_tempo_minus50;
}

void toggle_DAC_output(ToneGenObj* self, int state)
{

	self->deadline = MSEC(SYNC(&self, setDeadline, self->i));
	self->notePeriod = MSEC(SYNC(&self, getNotePeriod, self->i));

	startTotalTime(self, 0); 
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
	
	if(self->totalTime < self->deadline)
	{
		SEND(USEC(self->notePeriod), USEC(100), self, toggle_DAC_output, !state);
	}
	else
	{

		// Should include processing of new values in this location to
		// prevent processing happening where it matters

		if(self->totalTime < (self->deadline + MSEC(SILENCE_TIME)))
		{
			if(self->i < SONG_LENGTH)
			{
				self->i += 1;
			}
			else
			{
				self->i = 0;
			}
			SEND(USEC(self->notePeriod), MSEC(50), self, toggle_DAC_output, 0);
			T_RESET(&self->timer);
			self->maxTime = 0;
		}
	}
	
}

////////////////////////////////////////////////////////////////////////
// Trouble shooting with getValues

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

int getI(ToneGenObj* self, int unusued)
{
	return self->i;
}



