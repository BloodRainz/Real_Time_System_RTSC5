#include "TinyTimber.h"
#include "sciTinyTimber.h"
#include "canTinyTimber.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    Object super;
    int count;
    char c;
	char buf[50];
} App;

App app = { initObject(), 0, 'X', {0} };

void reader(App*, int);
void receiver(App*, int);

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
			// Add valid value into buffer
			self->buf[self->count++] = c;
			
			// Create a char to print text and value together
			SCI_WRITE(&sci0, "Rcv: \'");
			SCI_WRITECHAR(&sci0, c);
			SCI_WRITE(&sci0, "\'\n");
			
			// Troubleshooting buffer state
			int bufferValue = atoi(self->buf);
			char snbufOne[200];
			snprintf(snbufOne,200,"Input: %d\n", bufferValue;
			SCI_WRITE(&sci0, snbufOne);
			
			// Troubleshooting current count
			char snbufCount[200];
			snprintf(snbufCount,200,"Count: %d\n", self->count);
			SCI_WRITE(&sci0, snbufCount);
			break;
			
		// Case where delimiter, e, is typed.
		case 'e':
			self->buf[self->count] = '\0';
			// Convert buffer to an integer value
			int valueEnd = atoi(self->buf);
			// Reset the count to 0
			self->count = 0;
			// Create a char to print text and value together
			char snbuf[200];
			snprintf(snbuf,200,"Entered integer: %d\n", valueEnd);
			SCI_WRITE(&sci0, snbuf);
			break;
			
		// Case where invalid values are typed
		default:
			break;
	}
   // SCI_WRITE(&sci0, "Rcv: \'");
   // SCI_WRITECHAR(&sci0, c);
   // SCI_WRITE(&sci0, "\'\n");
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
