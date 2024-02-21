#include "backgroundTask.h"
#include <stdlib.h>
#include <stdio.h>

const Time loopPeriod = USEC(1300);

void beBusy(Backgroundtask* self, int unused)
{
	for(int i = 0; i < self->background_loop_range; i++) 
		{ 
			// Busy for loop
		}
	SEND(loopPeriod, 0, self, beBusy, unused);

}

int getLoopRange(Backgroundtask* self, int unused)
{
	return self->background_loop_range;
}

int setLoopRange(Backgroundtask* self, int background_loop)
{
	if((background_loop >= MIN_LOOP_RANGE) && (background_loop <= MAX_LOOP_RANGE))
	{
		self->background_loop_range = background_loop;
		return 0;
    }
    else
	{
        return -1;
    }
}



