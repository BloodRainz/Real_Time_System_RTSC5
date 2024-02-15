/*
 * Part 1 Submission: I/O interface
 * 
 * RTS-C5: 	Emil Johansson (emiuo@chalmers.se) 
 * 		Kavineshver Sivaraman Kathiresan (kavkat@chalmers.se) 
 * 		Joshua Geraghty (joshuag@chalmers.se)
 * 
 * Verified by Lab TA, 
 * 
 * - Allows the user to input multiple digits, and store the value to a three integer memory in FIFO style. 
 * - Use 'e' to store the entered digits as a number. 
 * - Allows the user to clear the memory, using F (MUST be capital F)
 * - Prints the sum of the memory, and the median of the memory.
 * 
 * KNOWN BUGS:
 * - Typing in massive numbers to the buffer will go out of bounds - so please don't type 5 billion gazillion and press e.  
 * - No SYNC or ASYNC methods used: so theoretically possible to press 
 * 		two inputs in at the same time.
 * - Whitespace created after last character within period loop.
 * 
 */
 
#include "TinyTimber.h"
#include "sciTinyTimber.h"
#include "canTinyTimber.h"
#include <stdlib.h>
#include <stdio.h>

#include "notes.h"

#include "sound.h"

typedef struct {
    Object super;
    int count;
    char c;
	char buffer[50];  // buffer to store the user input
	int three_num[3]; // buffer to store the user input when e is pressed
	int storage;	  // determines how many user input values are stored
	int key;		  // user defined key
} App;

App app = { initObject(), 0, 'X', {0}, {0}, 0, 0 };

Sound sound = initSound();

//Sound sound = {initObject(),  {0}, 0, 0, 0, 'X'};

void reader(App*, int);
void receiver(App*, int);

// Method to determine the median of one, two and three values.
int parseMedian(App*,int);

// 
int period(int, int);

// 
//void key_press(App*, int);

Serial sci0 = initSerial(SCI_PORT0, &app, reader);

Can can0 = initCan(CAN_PORT0, &app, receiver);

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
		
			// Get the sum of all number within the history.
			int sum3 = self->three_num[0] + self->three_num[1] + self->three_num[2];
			
			// Get the median of all numbers within the history.
			int median3 = parseMedian(self, 0);
			
			// Lab specified output. 
			snprintf(write_buf, 200, "Entered integer: %d : sum = %d , median = %d \n", self->three_num[0], sum3, median3);
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
			
			if (-5 <= self->key && self->key <= 5)
			{
				SCI_WRITE(&sci0, "Key: ");
				
				snprintf(write_buf, 32, "%d", self->key);
				SCI_WRITE(&sci0, write_buf);
				SCI_WRITE(&sci0, "\n");
				
				SCI_WRITE(&sci0, "Note periods: \n");
				
				// Prints individual period per counter number.
				
				for(int i = 0; i < song_length; i++){
           
					int currentnote = song[i];
       
					int currentperiod = period(currentnote, self->key);
					
					// period function returns a 0 when idx is outside of range.
					if (currentperiod == 0)
					{
						SCI_WRITE(&sci0, "ERROR: Note went outside of range \n");
						break;
					}
					
					snprintf(write_buf, 200, "%d ", currentperiod);
					SCI_WRITE(&sci0, write_buf);
				}
				SCI_WRITE(&sci0, "\n");
				SCI_WRITE(&sci0, "All periods printed. \n");
				break;
			}
			else
			{
				SCI_WRITE(&sci0, "Invalid key chosen\n");
				break;
			}
		
		case 'P':
		
			SYNC(&sound, getSound, 5);
			SYNC(&sound, toggle_DAC_output, NULL);
			break;
		
		case 'M':
			SYNC(&sound,mute,NULL);
			break;
		// Case where invalid values are typed
		default:
			break;
	}
}

int parseMedian(App* self, int unused)
{
	int med; 		// Median value output 
	int i, j, t; 	// Integers for bubble sorting algorithm
	int a[3]; 		// Array to store the sorted version of three_history
	
	// Obtain the median when one value is entered.
	// its just the user entered value for one value.
	if (self->storage == 1)
	{
		med = self->three_num[0];
		return med;
	}
	
	// Obtain the median when two values are entered.
	// Essentially average of two values.
	else if (self->storage == 2)
	{
		med = ((self->three_num[0] + self->three_num[1]) * 0.5);
		return med;	
	}
	
	// Obtain the median when three values are entered.
	// Sort the array in order, then pick the middle value.
	// However, don't change the position of the values within the three_history,
	// so use a temporary array to store ordered three_history
	else if (self->storage == 3)
	{
		a[0] = self->three_num[0];
		a[1] = self->three_num[1];
		a[2] = self->three_num[2];
		
		// Bubble sort algo: sorts the array from min to max.
		// has a poor worst case time, so not applicable for large data sets - change in future.
		for (i = 0 ; i <= self->storage-1 ; i++) 
		{ 
			for (j = 0 ; j <= self->storage-i ; j++) 
			{
				if (a[j] <= a[j+1])
				{ 
					t = a[j];
					a[j] = a[j+1];
					a[j+1] = t;
				}
				else
					continue ;
			}
		}
		med = a[1]; // Median is middle value of the sorted array.
		return med;
	}
	else
		return 0;
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

    CAN_INIT(&can0);
    SCI_INIT(&sci0);
    SCI_WRITE(&sci0, "Hello, hello...\n");

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
