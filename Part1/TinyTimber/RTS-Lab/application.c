/*
 * Part 1 Section 3 Submission: Deadlines to the rescue
 * 
 * RTS-C5:	Emil Johansson (emiuo@chalmers.se) 
 * 		Kavineshver Sivaraman Kathiresan (kavkat@chalmers.se) 
 * 		Joshua Geraghty (joshuag@chalmers.se)
 * 
 * Verified by Lab TA, Feb 21 19:30
 * 
 * Bug fixes: Feb 21, 23:23
 * 
 * 1. Attempted to fix problem by initialising soundObject with
 *    a notePeriod of 500: noise is audible with headphones,
 *    when system is muted and initially started up.
 * 2. Changed setfrequency1Khz ->  self->notePeriod = USEC(500)
 *    to self->notePeriod = 500.
 *    Conversion to USEC happens within toggle_DAC_output -> SEND
 *    statement. Older version caused double conversion.
 */
 
#include "TinyTimber.h"
#include "sciTinyTimber.h"
#include "canTinyTimber.h"
#include <stdlib.h>
#include <stdio.h>
#include "sound.h"

#include "backgroundTask.h"

#include "notes.h"

#include "WCET.h"

typedef struct {
    Object super;
    int count;
    char c;
	char buffer[50];	// buffer to store the user input
	int three_num[3];	// buffer to store the user input when e is pressed
	int storage;		// determines how many user input values are stored
	int key;			// user defined key
} App;

App app = { initObject(), 0, 'X', {0}, {0}, 0 };
SoundObject sound = initSound();
Backgroundtask task = initBackgroundTask();

void reader(App*, int);
void receiver(App*, int);

int period(int, int);


Serial sci0 = initSerial(SCI_PORT0, &app, reader);

Can can0 = initCan(CAN_PORT0, &app, receiver);

int current_load;

int current_volume = 0;

void receiver(App *self, int unused) {
    CANMsg msg;
    CAN_RECEIVE(&can0, &msg);
    SCI_WRITE(&sci0, "Can msg received: ");
    SCI_WRITE(&sci0, msg.buff);
}

void reader(App *self, int c) {
	
	// Create a char to print text and value together
	SCI_WRITE(&sci0, "Rcv: \'");
	SCI_WRITECHAR(&sci0, c);
	SCI_WRITE(&sci0, "\'\n");
	
	// Buffer for writing
	char write_buf[200];
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
			self->buffer[self->count++] = c;
			break;
			
		// Case where delimiter, e, is typed.
		case 'e':

			self->buffer[self->count] = '\0';
			self->count = 0;
			
			// Shift the buffer to the right:
			// FIFO style storage
			self->three_num[2] = self->three_num[1];
			self->three_num[1] = self->three_num[0];
			self->three_num[0] = atoi(self->buffer);
			
			// Determine the full size of the buffer:
			// size of entire buffer / size of one buffer value.
			int fullsize = sizeof(self->three_num)/sizeof(self->three_num[0]);
			
			// Ensure that storage does not increase above the allocated memory.
			if (self->storage < fullsize)
			{
				self->storage++;
			}
			
			// Lab specified output. 
			snprintf(write_buf, 200, "Entered integer: %d\n", self->three_num[0]);
			SCI_WRITE(&sci0, write_buf);
			break;
			
		// Erase three history buffer case, when capital F is pressed.
		case 'F':
			// Erase history of numbers.
			self->three_num[0] = 0;
			self->three_num[1] = 0;
			self->three_num[2] = 0;
			
			// Reset storage value, since no user values are stored.
			self->storage = 0;
			
			SCI_WRITE(&sci0, "The 3-history has been erased\n");
			break;
			
		case 'K':
			
			snprintf(write_buf, 200, "Entered key mode. \n");
			SCI_WRITE(&sci0,write_buf);
			
			self->buffer[self->count] = '\0';
			self->count = 0;
			
			self->key = atoi(self->buffer);
			
			if (-5 < self->key && self->key < 5)
			{
				SCI_WRITE(&sci0, "Key: ");
				
				snprintf(write_buf, 32, "%d", self->key);
				SCI_WRITE(&sci0, write_buf);
				SCI_WRITE(&sci0, "\n");
				
				SCI_WRITE(&sci0, "Note periods: \n");
				// FOR LOOP CRASHING CODE:
				for(int i = 0; i < 32; i++){
           
					int currentnote = song[i];
       
					int currentperiod = period(currentnote, self->key);
					
					// period function returns a 0 when idx is outside of range.
					if (currentperiod == 0)
					{
						SCI_WRITE(&sci0, "ERROR: Note went outside of range \n");
						break;
					}
					
					snprintf(write_buf, 200, "%d", currentperiod);
					SCI_WRITE(&sci0, write_buf);
					SCI_WRITE(&sci0, "\n");
				}
				SCI_WRITE(&sci0, "All periods printed. \n");
				break;
			}
			else
			{
				SCI_WRITE(&sci0, "Invalid key chosen. \n");
				break;
			}

		////////////////////////////////////////////////////////////////////////////////////////
		// Volume controls
		//
		// User defined volume
		case 'V':

			// Delimiter
			self->buffer[self->count] = '\0';
			self->count = 0;
            
			// Take in the user defined volume
			current_volume = atoi(self->buffer);
            
			// If no volume is selected, print current volume.
			if (current_volume == 0)
			{
				SCI_WRITE(&sci0, "No volume entered.\n");
				snprintf(write_buf, 200, "Current volume: %d\n", SYNC(&sound, getSound, NULL));
				SCI_WRITE(&sci0, write_buf);
				break;
			}
	
			// If SYNC setlevel is successful, print new volume
			if(SYNC(&sound, setLevel, current_volume) == current_volume)
			{
				snprintf(write_buf, 200, "New volume: %d \n", SYNC(&sound, getSound, NULL));
				SCI_WRITE(&sci0, write_buf);
				break;
			}
			
			// If user defined volume is out of range
			else
			{
				SCI_WRITE(&sci0, "ERROR: Volume out of range. \n");
				snprintf(write_buf, 200, "%d", SYNC(&sound, getSound, NULL));
				SCI_WRITE(&sci0, write_buf);
				SCI_WRITE(&sci0, " current volume \n");
			break;
			}
		break;
		
		// Mute functionality
		case 'M':
			// Clear the buffer: prevent preloading values.
			self->buffer[self->count] = '\0';
			self->count = 0;
		
			// Sync with MUTE function in sound.c
			SYNC(&sound, mute, NULL);
			SCI_WRITE(&sci0, "MUTED \n");
			break;
		
		// Increase the volume by one step.
		
		// Notes: this method was being very fussy: would prefer to remove incr_volume values
		//		  and create the method like the V method, but wouldn't work.
		//			- Josh, 21.02.2024
		case 'X':
		
			// Clear the buffer: prevent preloading values.
			self->buffer[self->count] = '\0';
			self->count = 0;
		
			current_volume = SYNC(&sound, getSound, NULL);
			
			int incr_volume = current_volume + VOL_INCR;
			
			// Checks if new volume is outside of range
			if(incr_volume < MAX_VOLUME+1)
			{
				current_volume = SYNC(&sound, setLevel, incr_volume);
				snprintf(write_buf, 200, "New volume: %d \n", current_volume);
				SCI_WRITE(&sci0, write_buf);
				break;
			}
			
			// If new volume is out of range, give out to the user
			else
			{
				SCI_WRITE(&sci0, "ERROR: Volume out of range \n"); 
				snprintf(write_buf, 200, "Please select a value between %d and %d \n", MIN_VOLUME, MAX_VOLUME); 
				SCI_WRITE(&sci0, write_buf);
				
				snprintf(write_buf, 200, "Current volume: %d \n", current_volume);
				SCI_WRITE(&sci0, write_buf);
				break;
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
			current_volume = SYNC(&sound, getSound, NULL);
			
			// Decrease the current volume by an incremental value (found in sound.c)
			int decr_volume = current_volume - VOL_INCR;
			
			// Check that the decreasement of volume is valid.
			if(decr_volume > MIN_VOLUME-1)
			{
				current_volume = SYNC(&sound, setLevel, decr_volume);
				snprintf(write_buf, 200, "New volume: %d \n", current_volume);
				SCI_WRITE(&sci0, write_buf);
				break;
			}
			
			// Give out to the user if volume is out of range
			else
			{
				SCI_WRITE(&sci0, "ERROR: Volume out of range \n"); 
				snprintf(write_buf, 200, "Please select a value between %d and %d \n", MIN_VOLUME, MAX_VOLUME); 
				SCI_WRITE(&sci0, write_buf);
				
				snprintf(write_buf, 200, "Current volume: %d \n", current_volume);
				SCI_WRITE(&sci0, write_buf);
				break;
			}
		break; 
		
		// Initialise the DAC to play a 1kHz signal
		case 'P':
		
			// Clear the buffer: prevent preloading values.
			self->buffer[self->count] = '\0';
			self->count = 0;
			
			// Call 'setfrequency1Khz within sound.c
			SYNC(&sound, setfrequency1Khz, period);
			SCI_WRITE(&sci0, "DAC Initialised.\n");
			
			snprintf(write_buf, 200, "Current volume: %d \n", SYNC(&sound, getSound, NULL));
			SCI_WRITE(&sci0, write_buf);
		
			
		break;
			
		/////////////////////////////////////////////////////////////////////
		// BUSY CONTROLS
		
		
		// User set load 
		case 'L':

			self->buffer[self->count] = '\0';
			self->count = 0;
            
			// Take user input
			current_load = atoi(self->buffer);
			
			// If no input is entered, print the current load.
			if (current_load == 0)
			{
				SCI_WRITE(&sci0, "No background load selected. \n");
				snprintf(write_buf, 200, "Current background load: %d \n", SYNC(&task, getLoopRange, NULL));
				SCI_WRITE(&sci0, write_buf);
				break;
			}
			
			// If user set load is within range, change it and print the new load.
			if(SYNC(&task, setLoopRange, current_load) == 0)
			{
				snprintf(write_buf, 200, "New background load: %d \n", SYNC(&task, getLoopRange, NULL));
				SCI_WRITE(&sci0, write_buf);
				break;
			}
			
			// Give out to the user if load is outside of range.
			else
			{
				SCI_WRITE(&sci0, "ERROR: Load out of range \n"); 
				snprintf(write_buf, 200, "Please select a value between %d and %d \n", MIN_LOOP_RANGE, MAX_LOOP_RANGE); 
				SCI_WRITE(&sci0, write_buf);
				
				snprintf(write_buf, 200, "Current background load: %d \n", SYNC(&task, getLoopRange, NULL));
				SCI_WRITE(&sci0, write_buf);
				break;
			}
		break;
		
		// Increase busy load by 500
		case 'I':
		
			// Clear the buffer: prevent preloading values.
			self->buffer[self->count] = '\0';
			self->count = 0;
		
			// Get the current load
			current_load = SYNC(&task, getLoopRange, 0);
			
			// If setLoopRange is successful, print the new load.
			if(SYNC(&task, setLoopRange, current_load+LOOP_INCR) == 0)
			{
				snprintf(write_buf, 200, "New background load: %d \n", SYNC(&task, getLoopRange, NULL));
				SCI_WRITE(&sci0, write_buf);
				break;
			}
			
			// Give out to the user for invalid load.
			else
			{
				SCI_WRITE(&sci0, "ERROR: Load out of range \n"); 
				snprintf(write_buf, 200, "Please select a value between %d and %d \n", MIN_LOOP_RANGE, MAX_LOOP_RANGE); 
				SCI_WRITE(&sci0, write_buf);
				
				snprintf(write_buf, 200, "Current load: %d \n", SYNC(&task, getLoopRange, NULL));
				SCI_WRITE(&sci0, write_buf);
				break;
			}
			break; 

		// Decrease busy load by 500
		case 'U':
			// Clear the buffer: prevent preloading values.
			self->buffer[self->count] = '\0';
			self->count = 0;
		
			current_load = SYNC(&task, getLoopRange, 0);
			
			if(SYNC(&task, setLoopRange, current_load-LOOP_INCR) == 0)
			{
				snprintf(write_buf, 200, "New background load: %d \n", SYNC(&task, getLoopRange, NULL));
				SCI_WRITE(&sci0, write_buf);
				break;
			}
			else
			{
				SCI_WRITE(&sci0, "ERROR: Load out of range \n"); 
				snprintf(write_buf, 200, "Please select a value between %d and %d \n", MIN_LOOP_RANGE, MAX_LOOP_RANGE); 
				SCI_WRITE(&sci0, write_buf);
				
				snprintf(write_buf, 200, "Current load: %d \n", SYNC(&task, getLoopRange, NULL));
				SCI_WRITE(&sci0, write_buf);
				break;
			}
			break; 


		//////////////////////////////////////////////////////////////////////////
		// Deadline controls
		
		// Enable/disable deadlines
		
		case 'D':
			// Clear the buffer: prevent preloading values.
			self->buffer[self->count] = '\0';
			self->count = 0;
			
			// Sync tasks for flipping 'enable deadline' values.
			int taskDlStatus = SYNC(&task, enableTaskDeadline, 0);
			int soundDlStatus = SYNC(&sound, enableSoundDeadline, 0);
			
			SCI_WRITE(&sci0, "Task deadlines changed. \n");
			
			snprintf(write_buf, 200, "Task deadline set to: %d , sound deadline set to: %d\n",taskDlStatus, soundDlStatus);
			SCI_WRITE(&sci0, write_buf);

			break;
		
		//////////////////////////////////////////////////////////////////////////////////
		// WCET Controls
		
		// Print WCET of a task
		
		case 'W':
		
			// Get the values of the WCET average, and WCET max time
			long WCETaverage = SYNC(&task.wcet, getWCETAverage, 0);
			long WCETmaxTime = SYNC(&task.wcet, getWCETMaxTime, 0);
			long WCETtotalTime = SYNC(&task.wcet, getWCETTotalTime, 0);
			
			SCI_WRITE(&sci0, "Worst Case Execution Time analysis: \n");
			
			snprintf(write_buf, 200, "Average execution time: %ld \n", WCETaverage);
			SCI_WRITE(&sci0, write_buf);
			
			snprintf(write_buf, 200, "Worst case timing: %ld \n", WCETmaxTime);
			SCI_WRITE(&sci0, write_buf);
			
			snprintf(write_buf, 200, "WCET Total time %ld \n", WCETtotalTime);
			SCI_WRITE(&sci0, write_buf);
			
			break;
			
		default:
			break;
	}
}

int period(int i, int Keynote){
	
    int idx = i + Keynote + f0_pos;
	
    if(idx < 0 || idx > 25)
    {
        return 0;
    }
	else
	{
		return notes[idx][1];
	}
}


void startApp(App *self, int arg) {
    CANMsg msg;
	ASYNC(&sound, toggle_DAC_output, 0);
	
	// Uncomment next line to add busy feature
	ASYNC(&task, beBusy, 0);
	
    CAN_INIT(&can0);
    SCI_INIT(&sci0);
    SCI_WRITE(&sci0, "Hello, hello...\n");
	
	// Instructions for the user
	SCI_WRITE(&sci0, "Welcome to the audio player\n");
	SCI_WRITE(&sci0, "//////////////////////////////////////////////////////////////////\n");
	SCI_WRITE(&sci0, "AUDIO INSTRUCTIONS: \n");
	SCI_WRITE(&sci0, "Type a value between -5 and 5, and press K to select the song key.\n");
	SCI_WRITE(&sci0, "Press P to play the 1kHz audio through the speaker.\n");
	SCI_WRITE(&sci0, "Type a value between 1 and 6, and press V to set the volume.\n");
	SCI_WRITE(&sci0, "RECOMMENDED VOLUME RANGE: 1 to 6.\n");
	SCI_WRITE(&sci0, "Press X to increase the volume by 1.\n");
	SCI_WRITE(&sci0, "Press Z to decrease the volume by 1.\n");
	SCI_WRITE(&sci0, "Press M to mute and unmute the volume.\n");
	SCI_WRITE(&sci0, "/////////////////////////////////////////////////////////////////\n");
	SCI_WRITE(&sci0, "BUSY TASK INSTRUCTIONS: \n");
	SCI_WRITE(&sci0, "Type a value between 1000 and 8000, and press L to set the loop range.\n");
	SCI_WRITE(&sci0, "Press I to increase the loop range by 500.\n");
	SCI_WRITE(&sci0, "Press U to decrease the loop range by 500.\n");
	SCI_WRITE(&sci0, "Press D to enable/disable deadlines.\n");
	SCI_WRITE(&sci0, "/////////////////////////////////////////////////////////////////\n");

    msg.msgId = 1;
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