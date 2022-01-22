#include <reg51.h>
#include <absacc.h>
// The code for setup of LCD is taken from a website mentioned in references
unsigned char str[5] = "START";
int i = 0;
int memoryAddPointer = 0x3800;


void delay(){
	TMOD &= 0xF1;		// Timer 0 Mode 1, Timer 1 remains the same
	TH0 = 0xFC;			// Initial value for 1milisecond delay
	TL0 = 0x66;
	TR0 = 1;
	while(TF0 == 0){
		TF0 = 0;
		TR0 = 0;
	}
}

// Functions for LCD module
void LCDdisplay(unsigned char ch){
	XBYTE[0x5000] = ch;
	XBYTE[0x5002] = 0x05;
	delay();
	XBYTE[0x5002] = 0x01;
}

void LCDcommand(unsigned char ch){
	XBYTE[0x5000] = ch;
	XBYTE[0x5002] = 0x04;
	delay();
	XBYTE[0x5002] = 0x00;
}

void ISR() interrupt 4{
	XBYTE[memoryAddPointer] = SBUF;		// Receive value from serial interrupt
	memoryAddPointer = memoryAddPointer + 1;
	if(memoryAddPointer == 0x4800){
		memoryAddPointer = 0x3800;
	}
	for(i = 0; i < 5; i++){
		LCDdisplay(str[i]);
		delay();
	}
}

int main(void){
	EA = 1;
	ET1 = 1;
	ET0 = 1;
	ES = 1;				// Enabling interrupts
	// The following setup code is from Dr. Jahed slides on LCD interfacing
	// 5*7 Matrix for LCD
	LCDcommand(0x38);
	delay();
	// Display on, blinking cursor
	LCDcommand(0x0F);
	delay();
	LCDcommand(0x01);
	delay();
	// Cursor moves to the start
	LCDcommand(0x81);
	delay();
	
	// Initialization for Timer 1 to be used in serial communication
	TMOD = 0x20;
	SCON = 0x50;
	TH1 = 0xFD;
	TR1 = 1;
	while(1);			// Wait for the data from system 1
	// return 0;
}