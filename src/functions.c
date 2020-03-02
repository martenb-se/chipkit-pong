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
int blink_counter = 0;
int credits_counter = 0;

uint8_t controller_plugged = 1;

// Sounds
uint8_t timer_count_sounds = 0;
uint8_t sound_status = 0;
uint16_t sound_period = 0;
uint16_t sound_duration = 0;
uint16_t sound_delay = 0;

// Music
uint8_t music_play_got = 0;
uint8_t music_iteration = 0;

// Random number generation
unsigned int rand_next = 0xd131;

// Generate quite unpredictable numbers with a
// timer not in sync with the other game timer
unsigned int rand(void)
{
	rand_next = TMR3;
	return rand_next;
}

// Enable input/output
void io_init(void) {
	// - Output
  TRISFCLR = 0b000000000000100;				// Controller - Clock
  TRISFCLR = 0b000000000001000;				// Controller - Latch
  TRISDSET = 0b000000100000000; 			// Controller - Data Input 1
  TRISDSET = 0b000000000000001; 			// Controller - Data Input 2
  //TRISFCLR = 0b000000000000010;
  TRISDCLR = 0b000000000000010;				// Buzzer (OC2)
  //TRISDCLR = 0b000000000000100;
  //TRISDCLR = 0b000001000000000;
  
  PORTFCLR = 0b000000000000100;				// 00000001
	PORTFCLR = 0b000000000001000;				// 00000010
	//PORTDSET = 0b000000100000000;				// 00000100
	//PORTDSET = 0b000000000000001;				// 00001000
	//PORTFSET = 0b000000000000010;				// 00010000
	PORTDCLR = 0b000000000000010;				// 00100000
	//PORTDSET = 0b000000000000100;				// 01000000
	//PORTDSET = 0b000001000000000;				// 10000000

}

// Initiate sounds
void sound_init(void) {
	// Turn off the OC2 when performing the setup
	OC2CON = 0;
	// Initialize primary compare register (master)
  OC2R = 0; 
  // Initialize secondary compare register (slave)
  OC2RS = 0;
  
  OC2CON = 0x6; // Configure for PWM mode without Fault pin enabled
  
  // User Timer 2
  //OC2CONCLR = 0x8; // No need since all bits are cleared
  
  // Enable OC2
  OC2CONSET = 0x8000;// Enable OC2
}

// Play sound
void play_sound(uint16_t frequency, uint16_t duration, uint16_t delay) {
	
	if(sound_status == 0) {
		// Reset timer
		TMR2 = 0;
		
		// Calculate period, duration and delay
		sound_period = 80000000 / 64 / frequency;
		sound_duration = duration;
		sound_delay = delay;
		
		// Bug
		if (sound_delay == 0)
			sound_delay = 1;
		
		// Initialize the PWM 
		PR2 = sound_period;
		
		// Initialize slave register
		OC2RS = (int) (sound_period/10);
		
		// Set status flag
		sound_status = 1;

	}
	
}

// Initiate display
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

// Send and recieve data over SPI
uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));									// 0000 0000 0000 1000 = SPITBE (while SPITBE == 0)
																						  // Waiting until transmitter is ready
	SPI2BUF = data;
	while(!(SPI2STAT & 1));											// 0000 0000 0000 0001 = SPIRBF (while SPITBF == 0)
																					    // Waiting until byte is recieved
	return SPI2BUF;
}

// Sleep for some time
void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

// Update frames
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

// Play music from Game of Thrones
void music_got_playing() {
	
	if (sound_status == 0 && music_play_got) {
		
		if(music_iteration < 8*4) {
			if(music_iteration%4 == 0)
				play_sound(392, 500, 0);
			if(music_iteration%4 == 1)
				play_sound(262, 500, 0);
			if(music_iteration%4 == 2 && (music_iteration < 4*4 || music_iteration >= 7*4))
				play_sound(311, 250, 0);
			else 
				play_sound(330, 250, 0);
			if(music_iteration%4 == 3)
				play_sound(349, 250, 0);
		}
		
		if(music_iteration >= 7*4 && (music_iteration - 7*4) < 2*4) {
			if((music_iteration - 7*4)%4 == 0)
				play_sound(294, 500, 0);
			if((music_iteration - 7*4)%4 == 1)
				play_sound(196, 500, 0);	
			if((music_iteration - 7*4)%4 == 2)
				play_sound(233, 250, 0);
			if((music_iteration - 7*4)%4 == 3)
				play_sound(262, 250, 0);
		}
		
		if(music_iteration >= 7*4 + 2*4) {
			music_iteration = 0;
			music_play_got = 0;
			
			// Loop in credits
			if(in_credits == 1) {
				music_play_got = 1;
			}
			
		} else {
			music_iteration++;
		}
		
	}
		
}

// Initiate timers
void timer_init(void) {

	// - Random number timer
	TMR3 = 0;
	PR3 = 65293;
	// Set prescale to 1:1
	T3CONCLR = 0x70;
	// Turn on
	T3CONSET = 0x8000;
	
	// - Sound timer
	TMR2 = 0;
	PR2 = 0;
	// Clear flag
  IFSCLR(0) = 0x100;
  // Enable interrupts for Timer 2
  IECSET(0) = 0x100;
  IPCSET(2) = 0x1c; // Lower than controller
	// Set prescale to 1:64 and turn on
	T2CONSET = 0x8060;

	// - Controller and frame timer
	//TRISD = TRISD | 0xe0; // 0b0000 0000 1110 0000
  //TRISF = TRISF | 1;
  TMR4 = 0;
	// Use a period of (50000/(8*2)/5) 625 (8*5 rising edges per frame)
  // Get 5 inputs every frame
  // One frame every 0.01s
  PR4 = 625;

  // Clear flag
  IFSCLR(0) = 0x10000;
	// Enable interrupts for Timer 4
	IEC(0) = 0x10000;
	IPC(4) = 0x1f; // Max

	// Set prescale to 1:16 and turn on
  T4CONSET = 0x8040;

}

// User interrupt handler
void user_isr(void) {
	// Timer 2: Sounds
	if((IFS(0) >> 8) & 1) {
		IFSCLR(0) = 0x100;

	}

	// Timer 4:
	// Controller Read
	// Frame update
	if((IFS(0) >> 16) & 1) {
		// Clear flag
	  IFSCLR(0) = 0x10000;
	  
	  // Unplugged controllers
	  //controller_catch();
	  
	  // Sounds
	  // Start playing at every millisecond
	  if (controller_plugged && timer_count_sounds >= 8) {
	  	timer_count_sounds = 0;
	  	
	  	if (sound_status == 1) {
	  	
		  	// Keep playing sound
		  	if (sound_duration > 0)
		  		sound_duration--;
				// Stop playing
				else if (sound_duration == 0) {
					OC2RS = 0;
		  		PR2 = 0;
				}
		  		
		  	// Wait until next sound
		  	if (sound_duration == 0 && sound_delay > 0)
		  		sound_delay--;
		  	// Play next sound
		  	else if (sound_duration == 0 && sound_delay == 0) {
		  		
		  		sound_status = 0;
		  		
		  	}
	  	
	  	}
	  	
	  }
	  
	  // Music
	  if (controller_plugged && timer_count >= 80) {
	  	//konami();
	  	music_got_playing();
	  }

	  // Frames
	  if (controller_plugged && timer_count >= 80 && in_game > 0) {
	  	frame_update();
	  	timer_count = 0;

	  }

		// Options blinker
		if(controller_plugged && blink_counter >= 2000 && in_options == 1)
		{
			blink_function();
			blink_counter = 0;
		}

		// Credits screen
		if(controller_plugged && credits_counter >= 28000 && in_credits)
		{
			credits_function();
			credits_counter = 0;
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
        //PORTE = ~controller_input_a_buffer; // Show input on lamps
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
		timer_count_sounds++;
		blink_counter++;
		credits_counter++;
	}

}

// Clear whole screen
void screen_clear(void) {
	int i;
	int j = 0;

	// Clear screen
	for(j;j<8;j++) {

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

