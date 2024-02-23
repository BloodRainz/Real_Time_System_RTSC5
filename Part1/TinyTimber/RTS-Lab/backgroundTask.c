#include "backgroundTask.h"
#include <stdlib.h>
#include <stdio.h>

const Time loopPeriod = USEC(1300);

// BeBusy task:
// Creates an empty for loop, which completes background_loop_range iterations.

void beBusy(Backgroundtask* self, int unused)
{
	Time deadline = 0; // deadline variable
	
	// If enable deadline is true, set the 
	// deadline to loopPeriod
	if (self->enableDl == 1)
	{
		deadline = loopPeriod;
	}
	
	// If not enabled, set deadline to 0.
	else
	{
		deadline = 0;
	}
	
	for(int i = 0; i < self->background_loop_range; i++) 
	{ 
		// Busy for loop
	}

	SEND(loopPeriod, deadline, self, beBusy, unused);

}

// Get the loop range value.
int getLoopRange(Backgroundtask* self, int unused)
{
	return self->background_loop_range;
}

// Set the loop range value.
// Ensures that it is within range, as set by the header file.
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

// Shows the status of enableDeadline
int statusTaskDeadline(Backgroundtask* self, int unused)
{
	return self->enableDl;
}

// Flips the enabledeadline status.
int enableTaskDeadline(Backgroundtask* self, int unused)
{
	self->enableDl = !self->enableDl;
	return self->enableDl;
}




