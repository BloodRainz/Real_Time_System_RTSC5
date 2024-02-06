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
} App;

App app = { initObject(), 0, 'X' };

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
    SCI_WRITE(&sci0, "Rcv: \'");
    SCI_WRITECHAR(&sci0, c);
    SCI_WRITE(&sci0, "\'\n");
	
	
	
	if ( ((c >= '0') & (c <= '9')) | (c == '-') ) {
		self->buffer[self->count++] = c;
	}
	else if ( c== 'e') {
		
		self->buffer[self->count] = '\0';
		SCI_WRITE(&sci0, "Entered Number is : ");
		SCI_WRITE(&sci0,  self->buffer);		
		SCI_WRITE(&sci0, "\'\n");
		self->count = 0;
		
		self->three_num[0] = self->three_num[1];
		self->three_num[1] = self->three_num[2];
		self->three_num[2] = atoi(self->buffer);
		
		//print  number history
		
		char write_buf[200];
		snprintf(write_buf, 200, "3 History nos : %d, %d, %d. \n", self->three_num[0],self->three_num[1],self->three_num[2]);
		SCI_WRITE(&sci0, write_buf);
		
		//print average and sum
		int sum3 = self->three_num[0] + self->three_num[1] + self->three_num[2];
		int average = sum3 / 3;
		snprintf(write_buf, 200, "Sum of 3 numbers: %d, Average of 3 numbers: %d. \n", sum3, average);
		SCI_WRITE(&sci0, write_buf);
		
	}
	
	else if (c == 'F'){
		// Erase history numbers
		self->three_num[0] = 0;
		self->three_num[1] = 0;
		self->three_num[2] = 0;
		SCI_WRITE(&sci0, "The 3 history is erased\n"); 
	}
	
	else {
		SCI_WRITE(&sci0, "not a number or command\'\n");
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
