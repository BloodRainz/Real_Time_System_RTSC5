#ifndef CANCOMMS_H
#define CANCOMMS_H

#define CAN_START 1
#define CAN_STOP 2
#define CAN_VOLUME 3
#define CAN_MUTE 4
#define CAN_TEMPO 5 
#define CAN_KEY 6
#define CAN_STATE 7


#include "TinyTimber.h"
#include "canTinyTimber.h"

typedef struct {
    Object super;
	int state;
} CANComms;

#define initCANComms() {initObject(), 0}

extern CANComms canComms; 

int getCANState(CANComms*, int);

void CANReceive(CANComms*, CANMsg received_msg);

void CANTransmit(CANComms*, CANMsg msg);

#endif