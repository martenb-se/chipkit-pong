/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int mytime = 0x5957;
// 1.D
int ledcount = 0;
// 2.C
int timeoutcount = 0;
// 3.B
int prime = 1234567;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
// 3.D
void user_isr( void ) {
	
	if((IFS(0) >> 8) & 1) {
	
		// 3.E
		// Clear interrupt flag
		IFSCLR(0) = 0x00000100;
	
		// 3.F
		// Every second
		if (!(timeoutcount)) {
			time2string( textstring, mytime );
			display_string( 3, textstring );
			display_update();
			tick( &mytime );
		}
		
		// 3.F
		// Count
		timeoutcount++;
		if (timeoutcount > 9)
			timeoutcount = 0;
		
	} else if((IFS(0) >> 19) & 1) {
		
  	volatile unsigned int* port_e = (volatile unsigned int*) 0xbf886110;
  	*port_e = ledcount++;
		
		// Clear interrupt flag
		IFSCLR(0) = 0x80000;
		
	}
	
}

/* Lab-specific initialization goes here */
void labinit( void )
{
	
	// 1.C
	// TRISE
  volatile unsigned int* tris_e = (volatile unsigned int*) 0xbf886100;
  *tris_e = *tris_e & 0xFF00;
  
  // 1.D
  // PORTE
  volatile unsigned int* port_e = (volatile unsigned int*) 0xbf886110;
  *port_e = ledcount++;
  
  // 1.E
  TRISD = TRISD | 0xfe0; // 0b0000 1111 1110 0000
  
  // 1 (optional)
  TRISF = TRISF | 1;
  
  //
  T2CONCLR = 0x8000; // Bit 15
  
  // 2.B
  // Timers SFR Summary in http://ww1.microchip.com/downloads/en/DeviceDoc/61105F.pdf
  // Clock is running at 80MHz
  TMR2 = 0;
  // 80*10^6 clocks/second, so 80*10^5 clocks/0.1 second
  // 8 000 000 is too big, 65 536 is max
  // 8 000 000 / 65 536 = 122
  // Prescale of at least 122 should be used, so let's use 256:1.
  //T2CONbits.TCKPS = 0x7;
  T2CON = T2CON | 0x70; // Bit 6:4
  
  // 8 000 000 / 256 = 31250
  // Use a period of 31250
  PR2 = 31250;
  // Clear flag
  //IFS0bits.T2IF = 0;
  IFSCLR(0) = 0x00000100;
  
  // Don't use multi vector mode
  INTCONCLR = 0x1000;
  
  // 3.G
  // Enable interrupts for Timer 2
	IEC(0) = 0x100; // pg 53 - http://ww1.microchip.com/downloads/en/DeviceDoc/61143H.pdf
	IPC(2) = 0x1f; // pg 53 - http://ww1.microchip.com/downloads/en/DeviceDoc/61143H.pdf
  
  // Surprise
  // Enable interrupts for SW4
	IEC(0) = IEC(0) | 0x80000;
	IPC(4) = 0x1c000000;
	
  //time2string( textstring, 0xABCD );
	//display_string( 3, textstring );
	//display_update();
	//tick( &mytime );
  
  // 3.G
  // Enable interrupts
  enable_interrupt();
  //asm volatile("ei");
  
  // Start timer
  //T2CONbits.ON = 1;
  T2CONSET = 0x8000; // Bit 15
  
}

/* This function is called repetitively from the main program */
// 3.C
void labwork( void ) {
	prime = nextprime( prime );
	display_string( 0, itoaconv( prime ) );
	display_update();
}