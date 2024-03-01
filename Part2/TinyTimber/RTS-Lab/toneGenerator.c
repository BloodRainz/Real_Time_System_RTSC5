#include "TinyTimber.h"
#include "toneGenerator.h"

#include <stdlib.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES:
ToneGenObj toneGenerator = initToneGen();
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
 
 
void updateKey(ToneGenObj* self, int newkey)
{
	self->key = newkey;
	
}

void updateTempo(ToneGenObj* self, int newtempo)
{
	self->tempo = newtempo;
	
}

void updateNotePeriod(ToneGenObj* self, int newNotePeriod)
{
	self->notePeriod = newNotePeriod;
}

void updateDeadline(ToneGenObj* self, Time newDeadline)
{
	self->deadline = newDeadline;
}

void toggle_DAC_output(ToneGenObj* self, int state)
{

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

	SEND(USEC(self->notePeriod), 0, self, toggle_DAC_output, !state);
}

////////////////////////////////////////////////////////////////////////
// Trouble shooting with getValues

long getWCETEndTime(ToneGenObj* self, int unused)
{
	return USEC_OF(self->end);
}

long getWCETTotalTime(ToneGenObj* self, int unused)
{
	return USEC_OF(self->totalTime);
}

long getWCETMaxTime(ToneGenObj* self, int unused)
{
	return self->maxTime;
}

int getKeyTG(ToneGenObj* self, int newkey)
{
	return self->key;
	
}

int getTempoTG(ToneGenObj* self, int newTempo)
{
	return self->tempo;
	
}