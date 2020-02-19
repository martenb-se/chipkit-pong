#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"

int framecount = 0;

void frame_init(void) {
	TMR2 = 0;
  // Clock is running at 80MHz
  // 80*10^6 clocks/second, so 80*10^4 clocks/0.01 second
  // 800 000 is too big, 65 536 is max
  // 800 000 / 65 536 = 12..
  // Prescale of at least 12 should be used, so let's use 16:1.
  //T2CONbits.TCKPS = 0x7;
  T2CON = T2CON | 0x40;
  
  // 800 000 / 16 = 50000
  // Use a period of 50000
  PR2 = 50000;
  // Clear flag
  //IFS0bits.T2IF = 0;
  IFSCLR(0) = 0x00000100;
  
  // Don't use multi vector mode
  INTCONCLR = 0x1000;
  
  // Enable interrupts for Timer 2
	IEC(0) = 0x100; // pg 53 - http://ww1.microchip.com/downloads/en/DeviceDoc/61143H.pdf
	IPC(2) = 0x1f; // pg 53 - http://ww1.microchip.com/downloads/en/DeviceDoc/61143H.pdf
	
	// Start timer
  //T2CONbits.ON = 1;
  T2CONSET = 0x8000; // Bit 15
  
}

void frame_update(void) {
	
	// Run every frame (every 0.01 seconds)
	playing_field_update();
		
	// Run every 5 frames (every 0.1 seconds)
	if (framecount%5 == 0) {
		// Allow one pixel/0.05 seconds
		check_player_moves();
	
	// Run every 10 frames (every 0.1 seconds)
	} else if (framecount%10 == 0) {
	
	// Run every 100 frames (every second)
	} else if (framecount == 99) {
		// Update scores
		display_left_score_update();
		display_right_score_update();
		
	}
	
	// Frame counter
	framecount++;
	if (framecount > 99) {
		framecount = 0;
	}
	
}