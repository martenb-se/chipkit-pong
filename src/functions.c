#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"

uint8_t clock_edge = 0;
uint8_t clock_count = 0;
uint8_t timer_count = 0;
uint8_t framecount = 0;
uint8_t controller_input_a = 0;
uint8_t controller_input_a_buffer = 0;
uint8_t controller_input_b = 0;
uint8_t controller_input_b_buffer = 0;

// Random number generation
unsigned int rand_next = 0xd131;

// Generate quite unpredictable numbers with a
// timer not in sync with the other game timer
unsigned int rand(void)
{
	rand_next = TMR3;
	return rand_next;
}

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

/*void display_string(int line, char *s) {
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
}*/

void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

void frame_update(void) {
	
	// Run every frame (every 0.01 seconds)
	playing_field_update();

	if (play_state == 0) {
		// Move ball on every frame so speed can be controlled
		ball_collision_detection();
		move_ball();
	}
	
	// Allow one pixel/0.01 seconds
	check_player_moves();
		
	// Run every 2 frames (every 0.02 seconds)
	if (framecount%2 == 0) {
	
	// Run every 5 frames (every 0.05 seconds)
	} else if (framecount%5 == 0) {

	
	// Run every 10 frames (every 0.1 seconds)
	} else if (framecount%10 == 0) {
	
	// Run every 100 frames (every second)
	} else if (framecount == 99) {
		if(play_mode_timed != 0 && play_state == 0)
			timer_countdown();
		
	}
	
	// Frame counter
	framecount++;
	if (framecount > 99) {
		framecount = 0;
	}
	
}

void timer_init(void) {
	
	// Random number timer
	TMR3 = 0;
	T3CONCLR = 0x70; // Clear 6:4 (set prescale 1:1)
	PR3 = 65293;
	// Turn on
	T3CONSET = 0x8000; // Bit 15
	
	// Controller and frame timer
	TRISD = TRISD | 0xe0; // 0b0000 0000 1110 0000
  TRISF = TRISF | 1;
  TMR4 = 0;
  T4CONSET = 0x40;
  
  // Use a period of (50000/(8*2)/5) 625 (8*5 rising edges per frame)
  // Get 5 inputs every frame
  // One frame every 0.01s
  PR4 = 625; 
  
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

void user_isr(void) {
	// Timer 2: Frame update
	/*if((IFS(0) >> 8) & 1) {
		IFSCLR(0) = 0x100;

		// Run frame updates if in game
		if(in_game > 0) {
			frame_update();
		}

	}*/

	// Timer 4: 
	// Controller Read
	// Frame update
	if((IFS(0) >> 16) & 1) {
		// Clear flag
	  IFSCLR(0) = 0x10000;
	  
	  // Frames
	  if (timer_count >= 80 && in_game > 0) {
	  	frame_update();
	  	timer_count = 0;
	  	
	  }

	  // Clock controller
	  if (clock_edge == 0) {
			PORTFSET = 0b000000000000100;
			clock_edge = 1;
			
		} else {
			PORTFCLR = 0b000000000000100;

			// Set latch
			if(clock_count == 0) {
				PORTFSET = 0b000000000001000;
			// Reset latch
			} else {
				PORTFCLR = 0b000000000001000;
			}

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
		
		timer_count++;

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
