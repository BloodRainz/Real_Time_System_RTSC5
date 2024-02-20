#include "TinyTimber.h"
#include "canTinyTimber.h"
#include "sound.h"
#include <stdlib.h>
#include <stdio.h>

#define DAC_Output (*((volatile uint8_t*) 0x4000741C))
//SoundObject sound = initSound();
int getSound(SoundObject* self, int volume){
    
    return self-> volume;
}

int setLevel(SoundObject* self, int volume){
    if((volume >= 0) && (volume <= 20)){
        self->volume = volume;
        return 0;
    }
    else{
        
        return -1;
    }
}

int setfrequency1Khz(SoundObject* self, int period){
    self-> period = USEC(500);
    return 0;
}

 int mute(SoundObject* self, int volume, int prev_sound){
     
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
    static int DAC_value;
          if(state == 0){
              DAC_value = 0;
          }else{
              DAC_value = self->volume;
          }
          DAC_Output = DAC_value;
        AFTER(USEC(self->period), self, toggle_DAC_output, !state);
 }

//void BackgroundLOOP(Backgroundtask* self, int Background_loop_range);
//void BackgroundLOOP(Backgroundtask* self, int low){
//  if(low < 0){
  //    self->Background_loop_range = 0;
 // }  else{
 //     self->Background_loop_range = low;
 // }
 // for(int i = 0; i < self -> Background_loop_range; i++){
       
  //  }
//}