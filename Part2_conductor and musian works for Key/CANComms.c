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
Can can_0 = initCan(CAN_PORT0, &canComms, CANReceive);
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
	self->Rxenterd = 1;
	switch(received_msg.msgId)
	{
		
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
			
			SYNC(&musicPlay, setKey, self->received_value);
		break;
		
		case CAN_STATE:
			
			SYNC(&canComms, setCANState, 0);
		break;
		
		default:

			
			break;
	}
}

int debug_value (CANComms* self, int unused )
{
	return self->received_value;
}
int debug_value2 (CANComms* self, int unused )
{
	return self->Rxenterd;
}
/////////////////////////////////////////////////////////////
