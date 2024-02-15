
#include "sound.h"

#include "TinyTimber.h"
#include "sciTinyTimber.h"
#include "canTinyTimber.h"
#include <stdlib.h>
#include <stdio.h>


Sound sound = initSound();

Backgroundtask backgroundtask = initBackgroundtask();

int getSound(Sound* self, int volume)
{
    return self->volume;
}

int setLevel(Sound* self, int volume){
    if((volume > 1) && (volume < 20)){
        self->volume = volume;
        return volume;
    }
    else
	{
        return 0;
    }
}

int setfrequency1Khz(Sound* self, int peri){
    self->peri = 500;
    return 0;
}

int mute(Sound* self, int volume, int prev_volume)
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
	return 0;
 }

 
 void toggle_DAC_output(Sound* self, int state)
 {
    static int DAC_value;
          if(state == 0){
              DAC_value = 0;
          }else{
              DAC_value = self->volume;
          }
          DAC_Output = DAC_value;
        AFTER(USEC(self->peri), self, toggle_DAC_output, !state);
 }

