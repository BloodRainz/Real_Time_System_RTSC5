/*
 * Part 2 Section 2: Music generator
 * 
 * RTS-C5:	Emil Johansson (emiuo@chalmers.se) 
 * 			Kavineshver Sivaraman Kathiresan (kavkat@chalmers.se) 
 * 			Joshua Geraghty (joshuag@chalmers.se)
 * 
 * Verified by Lab TA
 * 
 * Current bugs:
 * 
 * 1. toneGenerator.c skips the first iteration of Brother John upon
 *    the first loop of the program. 
 *    - Seems like its bypassing the if statements upon initialisation
 *    - Possibly change the if-else statements within toneGenerator.c
 *    - Initialiasng i to -1 works as a 'bandage' solution: but doesn't really fix problem
 * 2. Deadline arithmetic doesn't seem to calculate correctly:
 *    - Double check the maths, and possible change the order of operations
 *    - Turn it to microseconds to get a higher resolution?
 *    - SOLVED: Looking at it again, I think its correct?
 * 3. Find a better way to understand the timing requirements of the system.
 *    - Derive WCET of player at extreme conditions
 */
 
#include "TinyTimber.h"
#include "sciTinyTimber.h"
#include "canTinyTimber.h"
#include <stdlib.h>
#include <stdio.h>

#include "toneGenerator.h"
#include "musicPlayer.h"

#include "CANComms.h"

typedef struct {
    Object super;
    int count;
    char c;
	char buffer[50];	// buffer to store the user input
	int current_key;			// user defined key
	int current_volume;			// Holder for the current volume: will help reduce SYNCS
	int current_tempo;			// Holder for the current tempo: will help reduce SYNCs
	int user_mute;
	int CANMode;				// Makes the device aware of the CAN modes: 0 = Conductor, 1 = Musician
	
	
	
	CANMsg transmit_msg[CAN_BUFSIZE];		// CAN Message to transmit
} App;

App app = { initObject(), 0, 'X', {0}, 0, 0, 0 ,0, 0};

//ToneGenObj toneGenerator = initToneGen();
//musicPlayerObj musicPlay = initmusicPlayer();

void reader(App*, int);
void receiver(App*, int);

Serial sci0 = initSerial(SCI_PORT0, &app, reader);

Can can0 = initCan(CAN_PORT0, &app, receiver);

void receiver(App *self, int unused) {
    CANMsg msg;
    CAN_RECEIVE(&can0, &msg);
	//CAN_RECEIVE(&can_0, &msg2);
    SCI_WRITE(&sci0, "Can msg received: ");
    SCI_WRITE(&sci0, msg.buff);
	
	int temp = (int)(msg.msgId);

	SCI_WRITE(&sci0, "\n Can msgId: ");
	SCI_WRITE(&sci0, temp);


	
	// If set to musician, send the message to CANComms CANReceive
	if(self->CANMode == 1)
	{
		SYNC(&canComms, CANReceive, &msg);
	}
	
	if(msg.msgId == CAN_KEY)
	{
		int check_value,check2;
		
		SCI_WRITE(&sci0, "\n received CAN KEY \n");
		check_value = SYNC(&canComms, debug_value, 0);
		SCI_WRITE(&sci0, check_value);
		SCI_WRITE(&sci0, "\n check \n");
		check2 = SYNC(&canComms, debug_value2, 0);
		SCI_WRITE(&sci0, check2);
		SCI_WRITE(&sci0, "\n NOT atoi \n");
		SCI_WRITE(&sci0, msg.buff);
		if (atoi(msg.buff) == 2)
		{
			SCI_WRITE(&sci0, "2 out");
		}
		SCI_WRITE(&sci0, atoi(msg.buff));
	}
}

void reader(App *self, int c) {

	// Create a char to print text and value together
	SCI_WRITE(&sci0, "Rcv: \'");
	SCI_WRITECHAR(&sci0, c);
	SCI_WRITE(&sci0, "\'\n");
	
	// Buffer for writing
	char write_buf[200];
	char transmit_buf[8];
	int key_length=1;
	int volume_length=2;
	switch(c)
	{
		// Case where valid decimal integers are typed.
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '-':
			// Add valid value into buffer
			
			self->transmit_msg->buff[self->count] = c;
			self->buffer[self->count++] = c;
			break;
			
		//////////////////////////////////////////////////////////////////////////////
		//   
		// KEY CONTROLS:
		// 
		// - Instead of printing the current key and following notes
		//	 assign the current key to the music player
		case 'K':
			
			// Delimiter
			self->buffer[self->count] = '\0';
			//self->transmit_msg->buff[self->count] = '\0';
			// Set the length of the CAN Message to transmit
			self->transmit_msg->length = self->count;
			key_length = self->count;
			// Set the message Id for the CAN Bus
			self->transmit_msg->msgId = CAN_KEY;
			
			// Revert buffer count to zero

			self->count = 0;
            
			// Take in the user defined volume
			self->current_key = atoi(self->buffer);
			
			
			//CAN transmit
			snprintf(transmit_buf,key_length+1,"%d", self->current_key);
			
			for(int i=0; i<key_length+1; i++)
			{
				self->transmit_msg->buff[i] = transmit_buf[i];
			
			}
 
			
			
			// Checks the current CANMode:
			// If in Conductor mode, allow for normal operation
			if(self->CANMode == 0)
			{
				// If SYNC setlevel is successful, print new volume
				if(SYNC(&musicPlay, setKey, self->current_key) == self->current_key)
				{
					snprintf(write_buf, 200, "New key: %d \n", SYNC(&musicPlay, getKey, NULL));
					SCI_WRITE(&sci0, write_buf);
					//break;
				}
			
				// If user defined volume is out of range
				else
				{
					SCI_WRITE(&sci0, "ERROR: Key out of range. \n");
				
					snprintf(write_buf, 200, "Please select a value between %d and %d \n", MIN_KEY, MAX_KEY); 
					SCI_WRITE(&sci0, write_buf);
				
					snprintf(write_buf, 200, "Current key is: %d", SYNC(&musicPlay, getKey, NULL));
					SCI_WRITE(&sci0, write_buf);
					//break;
				}
			}
			
			CAN_SEND(&can0, &self->transmit_msg);
			
			break;
		
		// Increase the key by one step.
		
		// Notes: this method was being very fussy: would prefer to remove incr_volume values
		//		  and create the method like the V method, but wouldn't work.
		//			- Josh, 21.02.2024
		case 'H':
		
			// Clear the buffer: prevent preloading values.
			self->buffer[self->count] = '\0';
			self->count = 0;
		
			self->current_key = SYNC(&musicPlay, getKey, NULL);
			
			
			if(self->CANMode ==0)
			{
				int incr_key = self->current_key + KEY_INCR;
			
				// Checks if new key is outside of range
				if(incr_key < MAX_KEY+1)
				{
					self->current_key = SYNC(&musicPlay, setKey, incr_key);
					snprintf(write_buf, 200, "New key: %d \n", self->current_key);
					SCI_WRITE(&sci0, write_buf);
					//can transmit
					snprintf(transmit_buf,key_length,"%d", self->current_key);			
					for(int i=0; i<key_length; i++)
					{
						self->transmit_msg->buff[i] = transmit_buf[i];
				
					}					
					
				}
			
				// If new key is out of range, give out to the user
				else
				{
					SCI_WRITE(&sci0, "ERROR: Key out of range. \n"); 
					snprintf(write_buf, 200, "Please select a value between %d and %d \n", MIN_KEY, MAX_KEY); 
					SCI_WRITE(&sci0, write_buf);
					
					snprintf(write_buf, 200, "Current key: %d \n", self->current_key);
					SCI_WRITE(&sci0, write_buf);
					break;
				}
				
			}
			CAN_SEND(&can0, &self->transmit_msg);
		break; 
 
		// Decrease the key by one step.
		
		// Notes: this method was being very fussy: would prefer to remove decr_volume values
		//		  and create the method like the V method, but wouldn't work.
		//			- Josh, 21.02.2024
		case 'G':
		
			// Clear the buffer: prevent preloading values.
			self->buffer[self->count] = '\0';
			self->count = 0;
		
			// Get the current volume of the system
			self->current_key = SYNC(&musicPlay, getKey, NULL);
			
			if(self->CANMode == 0)
			{
			
				// Decrease the current key by an incremental value (found in toneGenerator.c)
				int decr_key = self->current_key - KEY_INCR;
				
				// Check that the decreasement of key is valid.
				if(decr_key > MIN_KEY-1)
				{
					self->current_key = SYNC(&musicPlay, setKey, decr_key);
					snprintf(write_buf, 200, "New key: %d \n", self->current_key);
					SCI_WRITE(&sci0, write_buf);
					snprintf(transmit_buf,key_length,"%d", self->current_key);			
					for(int i=0; i<key_length; i++)
					{
						self->transmit_msg->buff[i] = transmit_buf[i];
					}
					

				}
				
				// Give out to the user if key is out of range
				else
				{
					SCI_WRITE(&sci0, "ERROR: Key out of range. \n"); 
					snprintf(write_buf, 200, "Please select a value between %d and %d \n", MIN_KEY, MAX_KEY); 
					SCI_WRITE(&sci0, write_buf);
					
					snprintf(write_buf, 200, "Current key: %d \n", self->current_key);
					SCI_WRITE(&sci0, write_buf);

				}
				
			}
			CAN_SEND(&can0, &self->transmit_msg);
		break;
			

		////////////////////////////////////////////////////////////////////////////////////////
		// Volume controls
		//
		// User defined volume
		case 'V':

			// Delimiter
			self->buffer[self->count] = '\0';
			
			self->transmit_msg->length = self->count;
			volume_length = self->count;
			// Set the message Id for the CAN Bus
			self->transmit_msg->msgId = CAN_VOLUME;
			
			self->count = 0;
            
			// Take in the user defined volume
			int newVolume = atoi(self->buffer);
			
			snprintf(transmit_buf,key_length,"%d", newVolume);			
			for(int i=0; i<volume_length; i++)
			{
				self->transmit_msg->buff[i] = transmit_buf[i];
			}
			            
			if(self->CANMode == 0)
			{
			// If no volume is selected, print current volume.
				if (newVolume == 0)
				{
					SCI_WRITE(&sci0, "No volume entered.\n");
					snprintf(write_buf, 200, "Current volume: %d\n", SYNC(&toneGenerator, getVolume, NULL));
					SCI_WRITE(&sci0, write_buf);

				}
		
				// If SYNC setlevel is successful, print new volume
				if((newVolume < MAX_VOLUME+1) && (newVolume > MIN_VOLUME-1))
				{
					snprintf(write_buf, 200, "New volume: %d \n", SYNC(&toneGenerator, setVolume, newVolume));
					SCI_WRITE(&sci0, write_buf);
				}
				
				// If user defined volume is out of range
				else
				{
					SCI_WRITE(&sci0, "ERROR: Volume out of range. \n"); 
					snprintf(write_buf, 200, "Please select a value between %d and %d \n", MIN_VOLUME, MAX_VOLUME); 
					SCI_WRITE(&sci0, write_buf);
					
					snprintf(write_buf, 200, "Current volume is: %d \n", SYNC(&toneGenerator, getVolume, NULL));
					SCI_WRITE(&sci0, write_buf);
				}
				
			}
			CAN_SEND(&can0, &self->transmit_msg);
		break;
		
		// Mute functionality
		case 'M':
			// Clear the buffer: prevent preloading values.
			self->buffer[self->count] = '\0';
			self->count = 0;
			
			self->user_mute = !(self->user_mute);
			
			// Fixed comment from Jan, about MUTED always being printed
			SYNC(&toneGenerator, set_user_mute, self->user_mute);
			
			if(self->user_mute == 1)
			{
				if(SYNC(&toneGenerator, mute, 1) == 0)
				{
					SCI_WRITE(&sci0, "Volume muted \n");
				}
			}
			else if (self->user_mute == 0)
			{
				if(SYNC(&toneGenerator, unmute, NULL) > 0)
				{
					SCI_WRITE(&sci0, "Volume unmuted \n");
					
				}
			}
			break;
			
		// Increase the volume by one step.
		
		// Notes: this method was being very fussy: would prefer to remove incr_volume values
		//		  and create the method like the V method, but wouldn't work.
		//			- Josh, 21.02.2024
		case 'X':
		
			// Clear the buffer: prevent preloading values.
			self->buffer[self->count] = '\0';
			self->count = 0;
						
			self->current_volume = SYNC(&toneGenerator, getVolume, NULL);
			
			if(self->CANMode == 0)
			{
				int incr_volume = self->current_volume + VOL_INCR;
				
				// Checks if new volume is outside of range
				if(incr_volume < MAX_VOLUME+1)
				{
					self->current_volume = SYNC(&toneGenerator, setVolume, incr_volume);
					snprintf(write_buf, 200, "New volume: %d \n", self->current_volume);
					SCI_WRITE(&sci0, write_buf);
					break;
				}
				
				// If new volume is out of range, give out to the user
				else
				{
					SCI_WRITE(&sci0, "ERROR: Volume out of range \n"); 
					snprintf(write_buf, 200, "Please select a value between %d and %d \n", MIN_VOLUME, MAX_VOLUME); 
					SCI_WRITE(&sci0, write_buf);
					
					snprintf(write_buf, 200, "Current volume: %d \n", self->current_volume);
					SCI_WRITE(&sci0, write_buf);
					break;
				}
				
			}
		break; 
 
		// Decrease the volume by one step.
		
		// Notes: this method was being very fussy: would prefer to remove decr_volume values
		//		  and create the method like the V method, but wouldn't work.
		//			- Josh, 21.02.2024
		case 'Z':
		
			// Clear the buffer: prevent preloading values.
			self->buffer[self->count] = '\0';
			self->count = 0;
		
			// Get the current volume of the system
			self->current_volume = SYNC(&toneGenerator, getVolume, NULL);
			
			// Decrease the current volume by an incremental value (found in toneGenerator.c)
			int decr_volume = self->current_volume - VOL_INCR;
			
			// Check that the decreasement of volume is valid.
			if(decr_volume > MIN_VOLUME-1)
			{
				self->current_volume = SYNC(&toneGenerator, setVolume, decr_volume);
				snprintf(write_buf, 200, "New volume: %d \n", self->current_volume);
				SCI_WRITE(&sci0, write_buf);
				break;
			}
			
			// Give out to the user if volume is out of range
			else
			{
				SCI_WRITE(&sci0, "ERROR: Volume out of range. \n"); 
				snprintf(write_buf, 200, "Please select a value between %d and %d \n", MIN_VOLUME, MAX_VOLUME); 
				SCI_WRITE(&sci0, write_buf);
				
				snprintf(write_buf, 200, "Current volume: %d \n", self->current_volume);
				SCI_WRITE(&sci0, write_buf);
				break;
			}
		break; 
		
		////////////////////////////////////////////////////////////////////////////////////////
		// Tempo controls
		//
		// User defined tempo
		case 'T':

			// Delimiter
			self->buffer[self->count] = '\0';
			self->count = 0;
            
			// Take in the user defined volume
			self->current_tempo = atoi(self->buffer);
            
			// If no volume is selected, print current volume.
			if (self->current_tempo == 0)
			{
				SCI_WRITE(&sci0, "No tempo entered.\n");
				snprintf(write_buf, 200, "Current tempo: %d\n", SYNC(&musicPlay, getTempo, NULL));
				SCI_WRITE(&sci0, write_buf);
				break;
			}
	
			// If SYNC setTempo is successful, print new tempo
			if(SYNC(&musicPlay, setTempo, self->current_tempo) == self->current_tempo)
			{
				snprintf(write_buf, 200, "New tempo: %d \n", SYNC(&musicPlay, getTempo, NULL));
				SYNC(&toneGenerator, updateTempo, self->current_tempo); 
				SCI_WRITE(&sci0, write_buf);
				break;
			}
			
			// If user defined volume is out of range
			else
			{
				SCI_WRITE(&sci0, "ERROR: Tempo out of range. \n");
				snprintf(write_buf, 200, "Please select a value between %d and %d \n", MIN_TEMPO, MAX_TEMPO); 
				SCI_WRITE(&sci0, write_buf);
				snprintf(write_buf, 200, "Current tempo: %d", SYNC(&musicPlay, getTempo, NULL));
				SCI_WRITE(&sci0, write_buf);
			break;
			}
		break;
		
		// Increase the tempo by one step.
		
		// Notes: this method was being very fussy: would prefer to remove incr_volume values
		//		  and create the method like the V method, but wouldn't work.
		//			- Josh, 21.02.2024
		case 'E':
		
			// Clear the buffer: prevent preloading values.
			self->buffer[self->count] = '\0';
			self->count = 0;
		
			self->current_tempo = SYNC(&musicPlay, getTempo, NULL);
			
			int incr_tempo = self->current_tempo + TEMPO_INCR;
			
			// Checks if new volume is outside of range
			if(incr_tempo < MAX_TEMPO+1)
			{
				self->current_tempo = SYNC(&musicPlay, setTempo, incr_tempo);
				SYNC(&toneGenerator, updateTempo, self->current_tempo);
				snprintf(write_buf, 200, "New tempo: %d \n", self->current_tempo);
				SCI_WRITE(&sci0, write_buf);
				break;
			}
			
			// If new volume is out of range, give out to the user
			else
			{
				SCI_WRITE(&sci0, "ERROR: Tempo out of range \n"); 
				snprintf(write_buf, 200, "Please select a value between %d and %d \n", MIN_TEMPO, MAX_TEMPO); 
				SCI_WRITE(&sci0, write_buf);
				
				snprintf(write_buf, 200, "Current tempo: %d \n", self->current_tempo);
				SCI_WRITE(&sci0, write_buf);
				break;
			}
		break; 
 
		// Decrease the tempo by one step.
		
		// Notes: this method was being very fussy: would prefer to remove decr_volume values
		//		  and create the method like the V method, but wouldn't work.
		//			- Josh, 21.02.2024
		case 'W':
		
			// Clear the buffer: prevent preloading values.
			self->buffer[self->count] = '\0';
			self->count = 0;
		
			// Get the current volume of the system
			self->current_tempo = SYNC(&musicPlay, getTempo, NULL);
			
			// Decrease the current volume by an incremental value (found in toneGenerator.c)
			int decr_tempo = self->current_tempo - TEMPO_INCR;
			
			// Check that the decreasement of volume is valid.
			if(decr_tempo > MIN_TEMPO-1)
			{
				self->current_tempo = SYNC(&musicPlay, setTempo, decr_tempo);
				SYNC(&toneGenerator, updateTempo, self->current_tempo);
				snprintf(write_buf, 200, "New tempo: %d \n", self->current_tempo);
				SCI_WRITE(&sci0, write_buf);
				break;
			}
			
			// Give out to the user if volume is out of range
			else
			{
				SCI_WRITE(&sci0, "ERROR: Tempo out of range \n"); 
				snprintf(write_buf, 200, "Please select a value between %d and %d \n", MIN_TEMPO, MAX_TEMPO); 
				SCI_WRITE(&sci0, write_buf);
				
				snprintf(write_buf, 200, "Current tempo: %d \n", self->current_tempo);
				SCI_WRITE(&sci0, write_buf);
				break;
			}
		break; 
		
		case 'L' :
			
			if(self->CANMode == 0)
			{
				self->CANMode =1;
				
			}
			else{
				self->CANMode =0;
			}
			
			break;
		
		
		///////////////////////////////////////////////////////////////////////////////////////
		//
		// TROUBLESHOOTING METHODS
		case 'D' : ;  // remove the semicolon if error araises -> ; is the bandage to error on WCET
		
			// Get the values of the WCET average, and WCET max time
			int WCETstart   = SYNC(&musicPlay, getI, 0);
			//long WCETend     = SYNC(&toneGenerator, getWCETEndTime, 0);
			int newKey = SYNC(&toneGenerator, getKeyTG, 0);
			int newTempo = SYNC(&toneGenerator, getTempoTG, 0);
			long WCETDeadline = SYNC(&musicPlay, getDeadline, 0);
			
			SCI_WRITE(&sci0, "Worst Case Execution Time analysis: \n");
			
			snprintf(write_buf, 200, "I value: %d \n", WCETstart);
			SCI_WRITE(&sci0, write_buf);
			
			snprintf(write_buf, 200, "Key from toneGenerator: %d \n", newKey);
			SCI_WRITE(&sci0, write_buf);
			
			snprintf(write_buf, 200, "Tempo from toneGenerator: %d \n", newTempo);
			SCI_WRITE(&sci0, write_buf);
			
			snprintf(write_buf, 200, "WCET Deadline %ld \n", WCETDeadline);
			SCI_WRITE(&sci0, write_buf);
				
				
		break;

		default:
			break;
	}
}

void startApp(App *self, int arg) {
    CANMsg msg;
	
	ASYNC(&toneGenerator, toggle_DAC_output, 0);
	
	ASYNC(&musicPlay, nextNote, 0);
	
    CAN_INIT(&can0);
    SCI_INIT(&sci0);
    SCI_WRITE(&sci0, "Hello, hello...\n");
	
	// Instructions for the user
	SCI_WRITE(&sci0, "Welcome to the audio player\n");
	SCI_WRITE(&sci0, "//////////////////////////////////////////////////////////////////\n");
	SCI_WRITE(&sci0, "VOLUME INSTRUCTIONS: \n");
	SCI_WRITE(&sci0, "Type a value between 1 and 20, and press V to set the volume.\n");
	SCI_WRITE(&sci0, "RECOMMENDED VOLUME RANGE: 1 to 6.\n");
	SCI_WRITE(&sci0, "Press X to increase the volume by 1.\n");
	SCI_WRITE(&sci0, "Press Z to decrease the volume by 1.\n");
	SCI_WRITE(&sci0, "Press M to mute and unmute the volume.\n");
	SCI_WRITE(&sci0, "/////////////////////////////////////////////////////////////////\n");
	SCI_WRITE(&sci0, "KEY INSTRUCTIONS: \n");
	SCI_WRITE(&sci0, "Type a value between -5 and 5, and press K to select the song key.\n");
	SCI_WRITE(&sci0, "Press H to increase the key by 1.\n");
	SCI_WRITE(&sci0, "Press G to decrease the key by 1.\n");
	SCI_WRITE(&sci0, "/////////////////////////////////////////////////////////////////\n");
	SCI_WRITE(&sci0, "TEMPO INSTRUCTIONS: \n");
	SCI_WRITE(&sci0, "Type a value between 60 and 240, and press T to select the song tempo.\n");
	SCI_WRITE(&sci0, "Press E to increase the tempo by 10.\n");
	SCI_WRITE(&sci0, "Press W to decrease the tempo by 10.\n");
	SCI_WRITE(&sci0, "/////////////////////////////////////////////////////////////////\n");
    msg.msgId = 40;
    msg.nodeId = 1;
    msg.length = 6;
    msg.buff[0] = 'H';
    msg.buff[1] = 'e';
    msg.buff[2] = 'l';
    msg.buff[3] = 'l';
    msg.buff[4] = 'o';
    msg.buff[5] = 0;
    CAN_SEND(&can0, &msg);
}

int main() {
    INSTALL(&sci0, sci_interrupt, SCI_IRQ0);
	INSTALL(&can0, can_interrupt, CAN_IRQ0);
    TINYTIMBER(&app, startApp, 0);
    return 0;
}