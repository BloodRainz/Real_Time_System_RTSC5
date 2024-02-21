#ifndef BACKGROUNDTASK_H
#define BACKGROUNDTASK_H

#include "TinyTimber.h"
#include <stdint.h>

#define MIN_LOOP_RANGE 1000
#define MAX_LOOP_RANGE 8000

#define LOOP_INCR 500

typedef struct{
	Object super;
	int background_loop_range;

} Backgroundtask;
//Backgroundtask backgroundtask = {initObject(), 1000};

#define initBackgroundTask() { initObject(), MIN_LOOP_RANGE };

void beBusy(Backgroundtask*, int);
int getLoopRange(Backgroundtask*, int);
int setLoopRange(Backgroundtask*, int);


#endif