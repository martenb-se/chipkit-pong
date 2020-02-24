#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"

void display_init(void) {
  DISPLAY_CHANGE_TO_COMMAND_MODE;
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD;
	quicksleep(1000000);

	spi_send_recv(0xAE);
	DISPLAY_ACTIVATE_RESET;
	quicksleep(10);
	DISPLAY_DO_NOT_RESET;
	quicksleep(10);

	spi_send_recv(0x8D);
	spi_send_recv(0x14);

	spi_send_recv(0xD9);
	spi_send_recv(0xF1);

	DISPLAY_ACTIVATE_VBAT;
	quicksleep(10000000);

	spi_send_recv(0xA1);
	spi_send_recv(0xC8);

	spi_send_recv(0xDA);
	spi_send_recv(0x20);

	spi_send_recv(0xAF);
}



uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));									// 0000 0000 0000 1000 = SPITBE (while SPITBE == 0)
																						  // Waiting until transmitter is ready
	SPI2BUF = data;
	while(!(SPI2STAT & 1));											// 0000 0000 0000 0001 = SPIRBF (while SPITBF == 0)
																					    // Waiting until byte is recieved
	return SPI2BUF;
}

void display_string(int line, char *s) {
	int i;
	if(line < 0 || line >= 4)
		return;
	if(!s)
		return;

	for(i = 0; i < 16; i++)
		if(*s) {
			textbuffer[line][i] = *s;
			s++;
		} else
			textbuffer[line][i] = ' ';
}

void display_update(void) {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(0x0);
		spi_send_recv(0x10);

		DISPLAY_CHANGE_TO_DATA_MODE;

		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;

			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}

void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

int clock_edge = 0;
int clock_count = 0;
uint8_t controller_input_a = 0b00000000;
uint8_t controller_input_a_buffer = 0b00000000;
uint8_t controller_input_b = 0b00000000;
uint8_t controller_input_b_buffer = 0b00000000;

void user_isr(void) {
	// Timer 2: Frame update
	if((IFS(0) >> 8) & 1) {
		IFSCLR(0) = 0x100;

		// Run frame updates if in game
		if(in_game > 0) {
			frame_update();
		}

	}

	// Timer 4: Controller Read
	if((IFS(0) >> 16) & 1) {
		// Clear flag
	  IFSCLR(0) = 0x10000;

	  // Clock controller
	  if (clock_edge == 0) {
			//PORTFSET = 0b000000000000100;				// 00000001
	  	//PORTFSET = 0b000000000001000;				// 00000010
			//PORTDSET = 0b000000100000000;				// 00000100
			//PORTDSET = 0b000000000000001;				// 00001000
	  	//PORTFSET = 0b000000000000010;				// 00010000
			//PORTDSET = 0b000000000000010;				// 00100000
			//PORTDSET = 0b000000000000100;				// 01000000
			//PORTDSET = 0b000001000000000;				// 10000000

			PORTFSET = 0b000000000000100;

			clock_edge = 1;
		} else {
			//PORTFCLR = 0b000000000000100;				// 00000001
	  	//PORTFCLR = 0b000000000001000;				// 00000010
			//PORTDCLR = 0b000000100000000;				// 00000100
			//PORTDCLR = 0b000000000000001;				// 00001000
	  	//PORTFCLR = 0b000000000000010;				// 00010000
			//PORTDCLR = 0b000000000000010;				// 00100000
			//PORTDCLR = 0b000000000000100;				// 01000000
			//PORTDCLR = 0b000001000000000;				// 10000000

			PORTFCLR = 0b000000000000100;

			// Set latch
			if(clock_count == 0) {
				PORTFSET = 0b000000000001000;
			// Reset latch
			} else {
				PORTFCLR = 0b000000000001000;
			}
			/*
			// Get controller data
			controller_input_a_buffer = controller_input_a_buffer | (((PORTD >> 8) & 1) << (7 - clock_count));
			controller_input_b_buffer = controller_input_b_buffer | ((PORTD & 1) << (7 - clock_count));

			clock_count++;

			if(clock_count > 7) {
				PORTE = ~controller_input_a_buffer; // Show input on lamps
				//PORTE = ~controller_input_b_buffer; // Show input on lamps
				controller_input_a = ~controller_input_a_buffer;
				controller_input_b = ~controller_input_b_buffer;
				clock_count = 0;
				controller_input_a_buffer = 0;
				controller_input_b_buffer = 0;

			}
*/
      // Get controller data
      controller_input_a_buffer = controller_input_a_buffer | ((PORTD & 1) << (7 - clock_count));
      controller_input_b_buffer = controller_input_b_buffer | (((PORTD >> 8) & 1) << (7 - clock_count));

      clock_count++;

      if(clock_count > 7) {
        PORTE = ~controller_input_a_buffer; // Show input on lamps
        //PORTE = ~controller_input_b_buffer; // Show input on lamps
        controller_input_a = ~controller_input_a_buffer;
        controller_input_b = ~controller_input_b_buffer;
        clock_count = 0;
        controller_input_a_buffer = 0;
        controller_input_b_buffer = 0;

      }

			clock_edge = 0;

		}

	}

}

void screen_clear(void) {
	int i;
	int j = 0;

	// Clear screen
	for(j;j<4;j++) {

		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(j);	// row 0, 1, 2, 3
		spi_send_recv(0x0);
		spi_send_recv(0x21);
    spi_send_recv(0);
    spi_send_recv(127);
		DISPLAY_CHANGE_TO_DATA_MODE;

		i = 0;

		for(;i<128;i++) {
			spi_send_recv(0);
		}

	}
}
