#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"

void input_init() {
	// Button 1-3
  //TRISD = TRISD | 0xfe0; // 0b0000 1111 1110 0000
  TRISD = TRISD | 0xe0; // 0b0000 0000 1110 0000
  // Button 4
  TRISF = TRISF | 1;
	
}

int input_get_buttons(void)
{
	int buttons;
	buttons = ((PORTD >> 4) & 0xE) + ((PORTF >> 1) & 1);
	return buttons;
}
