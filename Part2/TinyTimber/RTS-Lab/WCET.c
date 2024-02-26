#include "TinyTimber.h"
#include "WCET.h"
#include <stdlib.h>
#include <stdio.h>

long getWCETMaxTime(WCET* self, int)
{
	return self->maxTime;
}

long getWCETAverage(WCET* self, int)
{
	return self->average;
}

void startRecording(WCET* self, int)
{
	// Set the starting point to the current baseline
	self->start = CURRENT_OFFSET();
}

void stopRecording(WCET* self, int)
{
	// Set the end point to the current baseline
	self->end = CURRENT_OFFSET();
	
	if(self->runs < RUNS)
	{
		self->runs += 1;
		
		// End points baseline - starting baseline
		Time diff = self->end - self->start;
		
		// Accumulate time differences to get total time
		self->totalTime += diff;
		
		// Getting the worst case execution time
		// Replaces current WCET if new time difference is larger
		if (diff > self->maxTime)
		{
			self->maxTime = diff;
		}
		
		if (self->runs == RUNS)
		{
		self->average = USEC_OF(self->totalTime) / RUNS;
		self->maxTime = USEC_OF(self->maxTime);
		self->runs += 1;
		}
	}
}

////////////////////////////////////////////////////////////////////////
// Trouble shooting with getValues
long getWCETStartTime(WCET* self, int)
{
	return USEC_OF(self->start);
}

long getWCETEndTime(WCET* self, int)
{
	return USEC_OF(self->end);
}

long getWCETTotalTime(WCET* self, int)
{
	return USEC_OF(self->totalTime);
}

int getWCETLongRun(WCET* self, int)
{
	return self->runs;
}