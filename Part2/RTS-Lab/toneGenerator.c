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
	// Safeguard to prevent muted volume printing
	if(self->volume == 0)
	{
		return self->prev_volume;
	}
	else
	{
		return self->volume;
	}
}

// Set the volume of the system, if it is within volume constraints.
// If within constraints, return the new volume.
// If not, do not change the volume, and return a -1.

int setVolume(ToneGenObj* self, int volume)
{
	// Safeguard to ensure that setting the volume does not 
	// cause the mute function to inverse
	if(self->volume == 0)
	{
		self->prev_volume = volume;
		return volume;
	}
	else
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

void set_user_mute(ToneGenObj* self, int user_mute)
{
	self->u_mute = user_mute;
}

int getUserMute(ToneGenObj* self, int unusued)
{
	return self->u_mute;
}


int mute(ToneGenObj* self, int user_mute)
 {
	 if((user_mute == 1) && (self->volume != 0))
	 {
		self->prev_volume = self->volume;
		self->volume = 0;
	 }
	 else
	 {
		 self->volume =0;
	 }
	return self->volume;
 }
 
 
 int unmute(ToneGenObj* self, int unused)
 {
	 if(self->u_mute == 0)
	 {
		self->volume = self->prev_volume;
	
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

int getKeyTG(ToneGenObj* self, int newkey)
{
	return self->key;
	
}

int getTempoTG(ToneGenObj* self, int newTempo)
{
	return self->tempo;
	
}

int get_volume_debug(ToneGenObj* self, int unused)
{
	return self->volume;
}

int get_prev_volume_debug(ToneGenObj* self, int unused)
{
	return self->prev_volume;
}