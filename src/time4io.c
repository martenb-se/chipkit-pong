#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

	//1111 1100 0000 0000
	//5432 1098 7654 3210
	
	//0000 0000 0000 0000
	
	//0000 1111 0000 0000
	//0000 0000 1110 0000 -> 0000 0000 0000 0111
	//                                      1110
	
	/*
	
	1000 = 8  = 8
	1001 = 9  = 9
	1010 = 10 = A
	1011 = 11 = B
	1100 = 12 = C
	1101 = 13 = D
	1110 = 14 = E
	1111 = 15 = F
	
	*/

// 1.F
int getsw( void )
{
	int switches;
	switches = (PORTD >> 8) & 0xF;
	return switches;
}

// 1.G
int getbtns( void )
{
	// 1.G
	//int buttons;
	//buttons = (PORTD >> 5) & 0x7;
	//return buttons;
	
	// 1 (optional)
	int buttons;
	buttons = ((PORTD >> 4) & 0xE) + ((PORTF >> 1) & 1);
	return buttons;
	
}
