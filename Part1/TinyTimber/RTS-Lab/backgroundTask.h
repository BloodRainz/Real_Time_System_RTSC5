#ifndef BACKGROUNDTASK_H
#define BACKGROUNDTASK_H

#include "TinyTimber.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "WCET.h"

#define MIN_LOOP_RANGE 1000
#define MAX_LOOP_RANGE 30000

#define START_LOOP_RANGE 13500

#define LOOP_INCR 500

#define TASK_DEADLINE USEC(1300)

typedef struct{
	Object super;
	int background_loop_range;
	int enableDl;
	WCET wcet; // Comment this out, and the define section to remove the WCET object.
} Backgroundtask;

//#define initBackgroundTask() { initObject(), START_LOOP_RANGE, USEC(0)};
#define initBackgroundTask() { initObject(), START_LOOP_RANGE, USEC(0), initWCET()}; // Use this definition if performing WCET

void beBusy(Backgroundtask*, int);
int getLoopRange(Backgroundtask*, int);
int setLoopRange(Backgroundtask*, int);
int statusTaskDeadline(Backgroundtask*, int);
int enableTaskDeadline(Backgroundtask*, int);


#endif