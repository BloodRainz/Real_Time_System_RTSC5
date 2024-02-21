#include "TinyTimber.h"
#include "sound.h"
#include <stdlib.h>
#include <stdio.h>

int getSound(SoundObject* self, int unused)
{    
	return self->volume;
}

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

int setfrequency1Khz(SoundObject* self, int unused)
{
	self->notePeriod = USEC(500);
	return 0;
}

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
	Time deadline = 0;
	
	// Enabling and disabling of DAC values
	if (self->enableDl == 1)
	{
		deadline = USEC(100);
	}
	else
	{
		deadline = 0;
	}
	
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
	
	SEND(USEC(self->notePeriod), deadline, self, toggle_DAC_output, !state);
}

int statusSoundDeadline(SoundObject* self, int unused)
{
	return self->enableDl;
}


// Maybe an easier way to implement, like with a not statement 
// to flip the bit
int enableSoundDeadline(SoundObject* self, int unused)
{
	self->enableDl = !self->enableDl;
	return self->enableDl;
}
