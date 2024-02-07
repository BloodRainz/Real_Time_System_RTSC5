#include "TinyTimber.h"
#include "sciTinyTimber.h"
#include "canTinyTimber.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    Object super;
    int count;
    char c;
	char buffer[50];
	int three_num[3];
	int storage;
} App;

App app = { initObject(), 0, 'X', {0}, {0}, 0 };

void reader(App*, int);
void receiver(App*, int);
int parseMedian(App*,int);

Serial sci0 = initSerial(SCI_PORT0, &app, reader);

Can can0 = initCan(CAN_PORT0, &app, receiver);

void receiver(App *self, int unused) {
    CANMsg msg;
    CAN_RECEIVE(&can0, &msg);
    SCI_WRITE(&sci0, "Can msg received: ");
    SCI_WRITE(&sci0, msg.buff);
}

void reader(App *self, int c) {
	switch(c)
	{
		// Consider making an if statement
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
			// Add valid value into buffer	fer
			self->buffer[self->count++] = c;
			
			// Create a char to print text and value together
			SCI_WRITE(&sci0, "Rcv: \'");
			SCI_WRITECHAR(&sci0, c);
			SCI_WRITE(&sci0, "\'\n");
			
			// Troubleshooting buffer	fer state
			int bufferValue = atoi(self->buffer);
			char snbufOne[200];
			snprintf(snbufOne,200,"Input: %d\n", bufferValue);
			SCI_WRITE(&sci0, snbufOne);
			
			// Troubleshooting current count
			char snbufCount[200];
			snprintf(snbufCount,200,"Count: %d\n", self->count);
			SCI_WRITE(&sci0, snbufCount);
			break;
			
		// Case where delimiter, e, is typed.
		case 'e':
			self->buffer[self->count] = '\0';
			SCI_WRITE(&sci0, "Entered Number is : ");
			SCI_WRITE(&sci0,  self->buffer);
			SCI_WRITE(&sci0, "\'\n");
			self->count = 0;
		
			self->three_num[2] = self->three_num[1];
			self->three_num[1] = self->three_num[0];
			self->three_num[0] = atoi(self->buffer);
			// Verifies that the 0s input by the user are accounted for within median calculation,
			// by adding a 'storage' counter which shows how many user inputs are within the buffer.
			// The median is calculated differently depending on the amount of values to calculate.
			int fullsize = sizeof(self->three_num)/sizeof(self->three_num[0]);
			
			if (self->storage < fullsize)
			{
				self->storage++;
			}
			//print  number history
		
			char write_buf[200];
			snprintf(write_buf, 200, "3 History nos : %d, %d, %d. \n", self->three_num[0],self->three_num[1],self->three_num[2]);
			SCI_WRITE(&sci0, write_buf);
		
			//print average and sum
			int sum3 = self->three_num[0] + self->three_num[1] + self->three_num[2];
			int median3 = parseMedian(self, 0);
			snprintf(write_buf, 200, "Sum of 3 numbers: %d, Median of 3 numbers: %d. . Storage value: %d\n", sum3, median3, self->storage);
			SCI_WRITE(&sci0, write_buf);
			break;
		case 'F':
			// Erase history numbers
			self->three_num[0] = 0;
			self->three_num[1] = 0;
			self->three_num[2] = 0;
			
			self->storage = 0;
			
			SCI_WRITE(&sci0, "The 3 history is erased\n");
			SCI_WRITE(&sci0, "\'\n"); 
			
		// Case where invalid values are typed
		default:
			break;
	}
   // SCI_WRITE(&sci0, "Rcv: \'");
   // SCI_WRITECHAR(&sci0, c);
   // SCI_WRITE(&sci0, "\'\n");
}

int parseMedian(App* self, int unused)
{
	int med, i, j, t; 
	int a[3];
	
	char snbufOne[200];
	if (self->storage == 1)
	{
		med = self->three_num[0];
		snprintf(snbufOne,200,"Med: %d    Threenum0: %d\n", med, self->three_num[0]);
		SCI_WRITE(&sci0, snbufOne);
		return med;
	}
	else if (self->storage == 2)
	{
		med = ((self->three_num[0] + self->three_num[1]) * 0.5);
		return med;	
	}
			
	else if (self->storage == 3)
	{
		a[0] = self->three_num[0];
		a[1] = self->three_num[1];
		a[2] = self->three_num[2];
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
		med = a[1];
		return med;
	}
	else
		return 0;
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
