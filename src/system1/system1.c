#include <reg51.h>
#include <absacc.h>
// The frequency for microcontroller is 11.0592MHz
// The frequency for instruction cycles is 921.6kHz
// To create a pulse with frequency 1.8k We use timer
// The value to set for a full timer is 921.6k/(2*555)=1.807
#define pulseSet = 2

// Setting the intial values and specifying pins for microcontroller
int pulseCount = 0;
int i = 0;
sbit pulse = P1^1;
int memoryAddPointer = 0x2800;
int value;
int help;
int dutyCycle = 127;	// It must be a number between 0 and 255
											// 127 specifies 50% duty cycle
int start = 0;
sbit startSignal = P1^0;


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

int main(void){
	ET0 = 1;
	ET1 = 1;
	EA = 1;					// Enable Timer and global interrupts
	
	// Initial conditions for microcontroller ports
	P1 = 0x00;
	P3 = 0x00;
	// Other ports are given values as you read from external memory
	
	// Using timer 1 for pulse
	TMOD &= 0xDF;
	TMOD |= 0x10;		// Set Timer 1 Mode 1, Timer 0 remains the same
	TH1 = 0x00;
	TL1 = 0x00;
	TR0 = 0;
	
	while(1){
		if(pulseCount >= 10){
			start = 1;
			break;
		}
	}
	if(start){
		// Now that we have counted the signal, we use Timer 1 for serial communication
		TMOD &= 0x2F;
		SCON = 0x50;				// The value used for baud rate 9600
		// The baud rate 19200 can not be used since the value for it is 1.5
		// but the value for baud rate determination must be a decimal like 3.
		TH1 = 0xFD;
		TR1 = 1;						// Start Timer 1
		// Now we have received ten consecutive pulses so we send active signal
		startSignal = 1;		// Sending the start signal to frying oven
		while(1){
			value = XBYTE[0x3801];							// Getting data from port B of 8255
			XBYTE[memoryAddPointer] = value;		// Saving given data
			SBUF = value;
			while(TI == 0);											// Wait until the transmit is complete
			TI = 0;
			memoryAddPointer = memoryAddPointer + 1;
																					// Incrementing memoryAdd Pointer
			if(memoryAddPointer == 0x3800){			
				// Checks whether we have reached the end of RAM
				memoryAddPointer = 0x2800;				// Starts to rewrite data
			}
			// We are now setting a 1 second delay
			for(i = 0; i < 1000; i++){
				delay();				// One milisecond delay
			}
		}
	}
	return 0;
}