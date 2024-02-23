#include "TinyTimber.h"
#include "WCET.h"
#include <stdlib.h>
#include <stdio.h>

int getStart(WCET* self, int)
{
	return self->start;
}

int getMaxTime(WCET* self, int)
{
	return self->maxTime;
}

long getWCETAverage(WCET* self, int)
{
	return self->average;
}

long getWCETMaxTime(WCET* self, int)
{
	return self->maxTime;
}

long getWCETTotalTime(WCET* self, int)
{
	return USEC_OF(self->totalTime);
}

void startRecording(WCET* self, int)
{
	self->start = CURRENT_OFFSET();
}

void stopRecording(WCET* self, int)
{
	int i;
	
	Time end = CURRENT_OFFSET();
	
	Time diff = end - self->start;
	
	Time maxTimeHolder;
	
	for (i=0; i < self->runs; i++)
	{
		self->totalTime += diff;
		
		if (diff > maxTimeHolder)
		{
			maxTimeHolder = diff;
		}
	}
	
	if (i == self->runs)
	{
		self->average = USEC_OF(end) / self->runs;
		self->maxTime = USEC_OF(maxTimeHolder);
	}
 
}