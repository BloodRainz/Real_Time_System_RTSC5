#include "TinyTimber.h"
#include "sound.h"
#include <stdlib.h>
#include <stdio.h>

// Return the current volume of the system
int getSound(SoundObject* self, int unused)
{    
	return self->volume;
}

// Set the volume of the system, if it is within volume constraints.
// If within constraints, return the new volume.
// If not, do not change the volume, and return a -1.

int setLevel(SoundObject* self, int volume)
{
    if((volume > 0) && (volume < MAX_VOLUME+1))
	{
		self->volume = volume;
		return volume;
    }
    else
	{
		self->volume = self->volume;
		return -1;
	}
}

// Set the 1kHz frequency
int setfrequency1Khz(SoundObject* self, int unused)
{
	self->notePeriod = 500;
	return 0;
}

// Mute the DAC: essentially set the volume to zero, and save
// the previous volume.
// If unmuted, return to the original volume.
int mute(SoundObject* self, int volume, int prev_sound)
 {
	if(self-> volume == 0){
	self-> volume = self -> prev_sound;
	}
	else{
		self->prev_sound = self->volume;
		self->volume = 0;
	}
	return 0;
 }

void toggle_DAC_output(SoundObject* self, int state)
{
	Time deadline = 0; // Values used to change the deadline value
	
	// If enableDl is true, set a deadline
	if (self->enableDl == 1)
	{
		deadline = USEC(100);
	}
	// If enableDl is false, remove deadline
	else
	{
		deadline = 0;
	}
	
	SEND(USEC(self->notePeriod), deadline, self, toggle_DAC_output, !state);
	
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
	
}

// Gets the current status of the sound deadline bit.
int statusSoundDeadline(SoundObject* self, int unused)
{
	return self->enableDl;
}


// Flips the 'enable deadline' status
int enableSoundDeadline(SoundObject* self, int unused)
{
	self->enableDl = !self->enableDl;
	return self->enableDl;
}
