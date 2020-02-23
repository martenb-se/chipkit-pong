#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"

void input_init() {
	// Button 1-3
  //TRISD = TRISD | 0xfe0; // 0b0000 1111 1110 0000
  TRISD = TRISD | 0xe0; // 0b0000 0000 1110 0000
  // Button 4
  TRISF = TRISF | 1;
  
  // External controller
  // - Timer
  TMR4 = 0;
  T4CONSET = 0x40;
  
  // Use a period of (50000/(8*2)/5) 625 (8*5 rising edges per frame)
  PR4 = 625; // Get 5 inputs every frame
  //PR4 = 5000;
  // Clear flag
  IFSCLR(0) = 0x10000;
  
  // Enable interrupts for Timer 4
	IEC(0) = 0x10000; //
	IPC(4) = 0x1f; // Max
	
	// Start timer
  T4CONSET = 0x8000; // Bit 15
  
  // - Output
  TRISFCLR = 0b000000000000100;				// Clock
  TRISFCLR = 0b000000000001000;				// Latch
  TRISDSET = 0b000000100000000; 			// Data Input 1
  TRISDSET = 0b000000000000001; 			// Data Input 2
  //TRISFCLR = 0b000000000000010;
  //TRISDCLR = 0b000000000000010;
  //TRISDCLR = 0b000000000000100;
  //TRISDCLR = 0b000001000000000;
  
  PORTFCLR = 0b000000000000100;				// 00000001
	PORTFCLR = 0b000000000001000;				// 00000010
	//PORTDSET = 0b000000100000000;				// 00000100
	//PORTDSET = 0b000000000000001;				// 00001000
	//PORTFSET = 0b000000000000010;				// 00010000
	//PORTDSET = 0b000000000000010;				// 00100000
	//PORTDSET = 0b000000000000100;				// 01000000
	//PORTDSET = 0b000001000000000;				// 10000000
  
	
}

int input_get_buttons(void)
{
	int buttons;
	buttons = ((PORTD >> 4) & 0xE) + ((PORTF >> 1) & 1);
	return buttons;
}
