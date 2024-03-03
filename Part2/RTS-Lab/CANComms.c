#include "TinyTimber.h"
#include "musicPlayer.h"
#include "CANComms.h"
#include "canTinyTimber.h"
#include "toneGenerator.h"
#include <stdlib.h>
#include <stdio.h>

/////////////////////////////////////////////////////////////////
// 
// Global variables

CANComms canComms = initCANComms();

////////////////////////////////////////////////////////////////
//
// CANState()
// - Defines if the device is in Conductor or Musician mode
// - When set to 0, device is in Conductor mode, meaning
//   it does not receive CAN messages.
// - When set to 1, device is in Musician mode, meaning
//   it receives CAN messages, and acts accordingly.

void setCANState(CANComms* self, int newState)
{
	self->state = !self->state;
}

int getCANState(CANComms* self, int unused)
{
	return self->state;
}

////////////////////////////////////////////////////////
//
// CANReceive method
// - Called in application.c when CAN state is set to 1, 
//   Musician Mode.
// - MsgId definitions are found within CANComms.h
// 
void CANReceive(CANComms* self, CANMsg received_msg)
{
	switch(received_msg.msgId)
	{
		// CAN_Start
		case CAN_START:
		
		break;
		
		case CAN_STOP:
		
		break;
		
		case CAN_VOLUME:
			SYNC(&toneGenerator, setVolume, received_msg.buff);
		break;
		
		case CAN_MUTE:
			SYNC(&toneGenerator, set_user_mute, 0);
		break;
		
		case CAN_TEMPO:
			SYNC(&musicPlay, setTempo, received_msg.buff);
		break;
		
		case CAN_KEY:
			SYNC(&musicPlay, setKey, received_msg.buff);
		break;
		
		case CAN_STATE:
			SYNC(&canComms, setCANState, 0);
		break;
	}
}

/////////////////////////////////////////////////////////////
//void CANTransmit(CANComms* self, CANMsg msg)
//{
//	
//}