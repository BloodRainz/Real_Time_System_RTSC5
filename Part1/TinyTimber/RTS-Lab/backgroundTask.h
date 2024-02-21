#ifndef BACKGROUNDTASK_H
#define BACKGROUNDTASK_H

#include "TinyTimber.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define MIN_LOOP_RANGE 1000
#define MAX_LOOP_RANGE 8000

#define LOOP_INCR 500

#define TASK_DEADLINE USEC(1300)

typedef struct{
	Object super;
	int background_loop_range;
	int enableDl;

} Backgroundtask;
//Backgroundtask backgroundtask = {initObject(), 1000};

#define initBackgroundTask() { initObject(), MIN_LOOP_RANGE, USEC(0)};

void beBusy(Backgroundtask*, int);
int getLoopRange(Backgroundtask*, int);
int setLoopRange(Backgroundtask*, int);
int statusTaskDeadline(Backgroundtask*, int);
int enableTaskDeadline(Backgroundtask*, int);


#endif