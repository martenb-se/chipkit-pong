#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"
#include <math.h>

#define PLAYER_INDENT 5 											// Constant value
#define ID_PLAYER_LEFT 0											// Player
#define ID_PLAYER_RIGHT 1											// Player
#define ID_PLAYER_CPU 1                       // CPU (is right player)
#define BUTTON_SUPER_SLOWDOWN	6								// 6 is button A
#define BUTTON_PAUSE 3												// 4 is button SELECT
#define BUTTON_BACK 5													// 5 is button B
#define BUTTON_CONFIRM 6											// 6 is button A

// Might be needed for math.h
int errno = 0;

// States
uint8_t play_time_left = 90;									// Time left to play
/*

Might be replaced with one uint8_t
uint8_t play_state = 0b00000000;

// Table
      STATE   DATA
0b    0000    0000 = Not used
0b    0000    0000 = Not used

0b    x1xx    000d = Pause				Data: who paused
0b    x1xx    00d0 = Pause				Data: init quit

0b    1xxx    000d = Game over		Stage

*/

uint8_t play_state = 0;												// State of play

// Input
uint8_t player_commands[] = {0, 0};						// Last commands: left and right
uint8_t player_holds[] = {0, 0};							// Number of frames player has held its input: left and right

// CPU
uint8_t player_cpu_hold = 0;									// Number of frames CPU has held input

// Graphics
uint8_t playbuffer[4][96];										// Array for pixel output
int play_timestamp = 0;												// Current timestamp in format: 12a34 (outputs as 12:34)

// Positions
uint8_t player_ball[] = {48, 16};							// Ball X/Y coordinates
uint8_t player_coordinates[] = {16, 16};			// Player coordinates: left and right	

// Movement
double player_ball_movement_pointer = 0;			// Array Pointer for movement
uint8_t player_ball_movements[2][96];				  // Movement coordinates: X and Y
double player_ball_direction = -1;						// Current direction of ball
double ball_speed_temp = 2;									// Temporary Frames/Movement for ball

// Customizable options
uint8_t ball_size = 2;												// Ball size squared
uint8_t ball_speed = 2;												// Frames/Movement for ball
uint8_t player_heights[] = {8, 8};					  // Player heights in pixels: left and right	
uint8_t player_speeds[] = {2, 2};							// Player speeds in frames/movement: left and right
uint8_t player_cpu = 3;												// CPU Level
uint8_t play_mode_timed = 90;									// Seconds to play for, max should be 4 minutes (240 seconds)
uint8_t play_mode_score = 99;									// Score to play to

// Scores
uint8_t sc1;
uint8_t sc2;

void display_score(uint8_t sc, int on_right)
{
	uint8_t j, k;

	DISPLAY_CHANGE_TO_COMMAND_MODE;
	spi_send_recv(0x22);
	spi_send_recv(0);
  spi_send_recv(0x0);
  if(on_right)
  	spi_send_recv(0x10);
  else
  	spi_send_recv(0x1f);
	DISPLAY_CHANGE_TO_DATA_MODE;
  for(j = 0; j < 2; j++)
  {
    if(sc > 99)
      sc = 0;
		for(k = 0; k < 8; k++)
    {
      if(j == 0)
        spi_send_recv(font[((0x30+sc/10))*8 + k]);
      else if(j == 1)
			   spi_send_recv(font[((0x30+sc%10))*8 + k]);
    }
	}
}

// Clear buffer (array) of all pixels
void playbuffer_clear(void) {
	int i;
	int j;

	for(j = 0; j < 4; j++) {

		for(i = 0; i < 96; i++) {
			playbuffer[j][i] = 0;

		}

	}

}

// Update screen with buffer
void play_xy_update(void) {
	int i;
	int j;

	for(j = 0; j < 4; j++) {

		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(j);	// row 0, 1, 2, 3
		spi_send_recv(0x0);
		spi_send_recv(0x11);
		//spi_send_recv(0x21);
		//spi_send_recv(16);
		//spi_send_recv(111);
		DISPLAY_CHANGE_TO_DATA_MODE;

		for(i = 0; i < 96; i++) {
			spi_send_recv(playbuffer[j][i]);

		}

	}

}

// Translate X Y to page and colum and set pixels
void play_xy_set(int x, int y) {

	playbuffer[y/8][x] = playbuffer[y/8][x] | (1 << y%8);

}

// Translate X Y to page and colum and unset pixels
void play_xy_clear(int x, int y) {

	playbuffer[y/8][x] = playbuffer[y/8][x] & ~(1 << y%8);

}

// Move player relative to current position
void move_player(uint8_t id_player, uint8_t rel_y) {
	player_coordinates[id_player] += rel_y;
	
	// Inside bounds
	if (player_coordinates[id_player] <= player_heights[id_player]/2 + 1)
		player_coordinates[id_player] = player_heights[id_player]/2 + 1;
	else if (player_coordinates[id_player] >= 31 - player_heights[id_player]/2)
		player_coordinates[id_player] = 31 - player_heights[id_player]/2;

}

// Calculate movements OR
// Move ball according to current movement array
void move_ball() {
	// Randomize first movement
	if (player_ball_direction == -1)
		player_ball_direction = PI/6 + (PI/6)/rand() + (PI/2) * (rand()%4);
	
	// Calculate new movements
	if (player_ball_movement_pointer == 0) {
		
		int i;
		// Calculations
		for(i = 0; i < 96; i++) {
			// Small angles should be straight
			if(player_ball_direction > PI - PI/12
				&& player_ball_direction < PI + PI/12
				|| player_ball_direction < PI/12
				&& player_ball_direction > 3*(PI/2) + (5*PI)/12) {
				
				player_ball_movements[1][i] = player_ball[1];
				
			// Angled
			} else {
				player_ball_movements[1][i] = (uint8_t)(player_ball[1] - (sin(player_ball_direction) * (double)(i+1)));
				
			}
			
			player_ball_movements[0][i] = (uint8_t)(player_ball[0] + (cos(player_ball_direction) * (double)(i+1)));
			
		}
		
	}
	
	// Move according to movement array
	player_ball[0] = player_ball_movements[0][(int)player_ball_movement_pointer];
	player_ball[1] = player_ball_movements[1][(int)player_ball_movement_pointer];
	
	if(ball_speed_temp != ball_speed && (1/ball_speed_temp) <= 1)
		player_ball_movement_pointer += 1/ball_speed_temp;
	else
		player_ball_movement_pointer += 1/(double)ball_speed;
	
}

// Player input to movement action
void player_input_movement_action(uint8_t id_player, uint8_t controller_data, uint8_t input, int8_t movement) {
	// Controller inout check
	if ((controller_data >> input) & 1) {
		// Speed slowdown
		if (player_speeds[id_player] > 1) {
			// Is player holding down button
			if (player_commands[id_player] == controller_data)
				player_holds[id_player]++;
			else
				player_holds[id_player] = 1;
			
			// Held for required amount of time
			// - Hold BUTTON_SUPER_SLOWDOWN for super slowdown
			if (((controller_data >> BUTTON_SUPER_SLOWDOWN) & 1) && player_holds[id_player] == (player_speeds[id_player]*2)
				|| !((controller_data >> BUTTON_SUPER_SLOWDOWN) & 1) && player_holds[id_player] == player_speeds[id_player]) {
				move_player(id_player, movement);
				// Reset hold
				player_holds[id_player] = 0;
			}
			
		// Use normal speed
		} else
			move_player(id_player, movement);
			
		// Set last commands
		player_commands[id_player] = controller_data;
	}
	
}

// Player input pause
void player_input_pause(uint8_t id_player, uint8_t controller_data) {
	
	// Not already paused..
	if (!((play_state >> 6) & 1)) {
		if (player_commands[id_player] != controller_data && (controller_data >> BUTTON_PAUSE) & 1) {
			// Set pause
			play_state = 0x40 + id_player;
		}
		player_commands[id_player] = controller_data;
	// ..or paused by current player
	} else if (((play_state >> 6) & 1) && (play_state & 1) == (id_player & 1)) {
		// Unpause
		if (player_commands[id_player] != controller_data && (controller_data >> BUTTON_PAUSE) & 1) {
			// Reset play state (unpaused)
			play_state = 0;
		// Quit not already initialized
		} else if (!((play_state >> 1) & 1) && (controller_data >> BUTTON_BACK) & 1) {
			// Intialize quit
			play_state |= 2;
		// Quit confirmed
		} else if (((play_state >> 1) & 1) && (controller_data >> BUTTON_CONFIRM) & 1) {
			// Reset state
			play_state = 0;
			playing_reset(1);
		}
		player_commands[id_player] = controller_data;
	}
	
}

// CPU movement
void cpu_movement() {
	
	if (player_ball[0] > (86 - ((player_cpu-1) * 10))
		&& player_cpu_hold >= (player_speeds[ID_PLAYER_LEFT]*(5-player_cpu)-1)
		|| player_ball[0] > (66 - ((player_cpu-1) * 10))
		&& player_cpu_hold >= (player_speeds[ID_PLAYER_LEFT]*(6-player_cpu)-1)
		|| player_ball[0] > (46 - ((player_cpu-1) * 10))
		&& player_cpu_hold >= (player_speeds[ID_PLAYER_LEFT]*(7-player_cpu)-1)
		|| player_cpu_hold >= (player_speeds[ID_PLAYER_LEFT]*(8-player_cpu)-1)) {
		if (player_coordinates[ID_PLAYER_CPU] > player_ball[1]) {
			move_player(ID_PLAYER_CPU, -1);
			
		} else if (player_coordinates[ID_PLAYER_CPU] < player_ball[1]) {
			move_player(ID_PLAYER_CPU, 1);
			
		}
		
		player_cpu_hold = 0;
		
	}
	
	player_cpu_hold++;
	
}

void check_player_moves(void) {
	
	// Not game over
	if(!((play_state >> 7) & 1)) {
	
		// Not paused
		if (!((play_state >> 6) & 1)) {
			
			// Controller support
			
			// Left controller
			// Up
			player_input_movement_action(ID_PLAYER_LEFT, controller_input_a, 2, -1);
			// Down
			player_input_movement_action(ID_PLAYER_LEFT, controller_input_a, 1, 1);
			
			// 1 Player mode
			if(in_game == 1) {
				cpu_movement();
			
			// 2 Player mode
			} else if(in_game == 2) {
			
				// Left controller
				// Up
				player_input_movement_action(ID_PLAYER_RIGHT, controller_input_b, 2, -1);
				// Down
				player_input_movement_action(ID_PLAYER_RIGHT, controller_input_b, 1, 1);
			
			}
		
		}
		
		// Pausing
		// Left player
		player_input_pause(ID_PLAYER_LEFT, controller_input_a);
		
		// Right can only pause in 2p mode
		if (in_game == 2) {
			player_input_pause(ID_PLAYER_RIGHT, controller_input_b);
		}
	
	// Game over
	} else {
		// Catch hold
		if(player_holds[ID_PLAYER_LEFT] || (player_holds[ID_PLAYER_RIGHT] && in_game == 2)) {
			if(!(controller_input_a) && (!(controller_input_b) || in_game == 1)) {
				player_holds[ID_PLAYER_LEFT] = 0;
				player_holds[ID_PLAYER_RIGHT] = 0;
				
			}
		
		// Game Over - Any button
		} else if(((play_state >> 7) & 1) && !(play_state & 1) && (controller_input_a || (player_holds[ID_PLAYER_RIGHT] && in_game == 2))) {
			// Next game over screen
			play_state |= 1;
			// Last command
			player_holds[ID_PLAYER_LEFT] = 1;
			player_holds[ID_PLAYER_RIGHT] = 1;
		
		// Game Over - Replay
		} else if(((play_state >> 7) & 1) && (play_state & 1) && ((controller_input_a >> BUTTON_CONFIRM) & 1 || (((controller_input_b >> BUTTON_CONFIRM) & 1) && in_game == 2) )) {
			playing_reset(0);
			
		// Game Over - Exit
		} else if(((play_state >> 7) & 1) && (play_state & 1) && ((controller_input_a >> BUTTON_BACK) & 1 || (((controller_input_b >> BUTTON_BACK) & 1) && in_game == 2) )) {
			playing_reset(1);
			
		}
		
	}
	
}

double ball_bounce_calculation(double angle, uint8_t use_y) {
	angle = (-angle + (use_y*PI) + (2*PI));
	if (angle > (2*PI))
		angle -= (2*PI);
	
	return angle;
}

void ball_collision_action_player_bounce(uint8_t id_player) {
	
	PORTE = 0;
	
	// Move ball
	player_ball[0] = (95 * id_player) + (1 - 2*id_player) * (PLAYER_INDENT + 2 + ball_size/2) + (1 - 2*id_player);
	
	// Bounce
	// Upper Edge
	if(player_ball[1] >= player_coordinates[id_player] - player_heights[id_player]/2 && player_ball[1] <= player_coordinates[id_player] - player_heights[id_player]/3) {

		// Make angle	
		if(player_ball_direction > PI - PI/12
			&& player_ball_direction < PI + PI/12
			|| (player_ball_direction < PI/12
			|| player_ball_direction > 3*(PI/2) + (5*PI)/12)) {
			player_ball_direction = (1+(id_player*4))*PI/6;
		
		// Moving down - return
		} else if(player_ball_direction > PI && player_ball_direction < 2*PI) {
			player_ball_direction += PI;
			if(player_ball_direction > 2*PI)
				player_ball_direction -= 2*PI;
				
			// Reset speed
			ball_speed_temp = ball_speed;
	
		// Moving up - Increase speed
		} else if (player_ball_direction > 0 && player_ball_direction < PI) {
			player_ball_direction = (2*(1-id_player))*PI/3;
			ball_speed_temp = (double)ball_speed/2;
		
		// Make angle	
		} else {
			player_ball_direction = (1+(id_player*4))*PI/6;
			
		}
		
	// Lower Edge
	} else if(player_ball[1] <= player_coordinates[id_player] + player_heights[id_player]/2 && player_ball[1] >= player_coordinates[id_player] + player_heights[id_player]/3) {
	
		// Make angle	
		if(player_ball_direction > PI - PI/12
			&& player_ball_direction < PI + PI/12 ||
			(player_ball_direction < PI/12
			|| player_ball_direction > 3*(PI/2) + (5*PI)/12)) {
			player_ball_direction = (11-4*id_player)*(PI/6);
			
		// Moving up - Return
		} else if (player_ball_direction > 0 && player_ball_direction < PI) {
			player_ball_direction += PI;
			if(player_ball_direction > 2*PI)
				player_ball_direction -= 2*PI;
			
			// Reset speed
			ball_speed_temp = ball_speed;
	
		// Moving down - Increase speed and angle
		} else if(player_ball_direction > PI && player_ball_direction < 2*PI) {
			player_ball_direction = (5-id_player)*(PI/3);
			ball_speed_temp = (double)ball_speed/2;
			
		// Make angle	
		} else {
			player_ball_direction = (11-4*id_player)*(PI/6);
			
		}
	
	// Middle	
	} else if (player_ball[1] >= player_coordinates[id_player] - 1 && player_ball[1] <= player_coordinates[id_player] + 1) {
		
		// Increase speed
		if(player_ball_direction > PI - PI/12
			&& player_ball_direction < PI + PI/12
			|| (player_ball_direction < PI/12
			|| player_ball_direction > 3*(PI/2) + (5*PI)/12)) {
			ball_speed_temp = (double)ball_speed/2;
		}
		
		player_ball_direction = (id_player)*PI;
	
	// Bounce	
	} else {
		
		player_ball_direction = ball_bounce_calculation(player_ball_direction, 1);
		
	}
	
	if(sound_on == 1)
		play_sound(300, 10, 1);
	
	// Reset movement pointer
	player_ball_movement_pointer = 0;
	
}

void ball_collision_action_score(uint8_t on_right) {
	// Reset speed
	ball_speed_temp = ball_speed;

	// Reset ball
	player_ball[0] = 48;
	player_ball[1] = 16;
	
	// Direct opposite
	player_ball_direction = ((on_right)*(PI/2) + PI/6) + (PI/6)/rand() + ((3-(on_right*2))*PI/2) * (rand()%2);
	player_ball_movement_pointer = 0;
	
	// Update scores
	if (!on_right) {
		sc2++;
		display_score(sc2, 0);
	} else {
		sc1++;
		display_score(sc1, 1);
	}
	
	// Game over?
	if(play_mode_score > 0 && (sc1 == play_mode_score || sc2 == play_mode_score)) {
		play_state = 0x80;
		// Catch holding
		player_holds[ID_PLAYER_LEFT] = 1;
		player_holds[ID_PLAYER_RIGHT] = 1;
	}
	
}

void ball_collision_detection(void) {
	
	// Out of bounds fix
	if (player_ball[1] <= ball_size/2)
		player_ball[1] = ball_size/2 + 2;
		
	if (player_ball[1] >= 32 - ball_size/2)
		player_ball[1] = 30 - (ball_size/2);
	
	// Check for walls
	// Collision on upper wall
	if (player_ball[1] == ball_size/2 + 1 && 
		player_ball_direction > 0 && player_ball_direction < PI) {
			
		// Move ball
		player_ball[1] = ball_size/2 + 2;
		
		// Bounce
		player_ball_direction = ball_bounce_calculation(player_ball_direction, 0);
		
		// Reset movement pointer
		player_ball_movement_pointer = 0;
		
	// Collision on lower wall
	} else if (player_ball[1] == 31 - ball_size/2 &&
		player_ball_direction > PI && player_ball_direction < 2*PI) {
		
		// Move ball
		player_ball[1] = 30 - (ball_size/2);
		
		// Bounce
		player_ball_direction = ball_bounce_calculation(player_ball_direction, 0);
		
		// Reset movement pointer
		player_ball_movement_pointer = 0;
	
	// Collistion on left player
	} else if (player_ball[0] <= PLAYER_INDENT + 2 + ball_size/2 && player_ball[0] >= PLAYER_INDENT + 1 + ball_size/2
		&& (player_ball[1] >= player_coordinates[ID_PLAYER_LEFT] - player_heights[ID_PLAYER_LEFT]/2 && player_ball[1] <= player_coordinates[ID_PLAYER_LEFT] + player_heights[ID_PLAYER_LEFT]/2)) {
		
		ball_collision_action_player_bounce(ID_PLAYER_LEFT);
		
	// Collistion on score (beside left player)
	} else if (player_ball[0] <= ball_size/2 + 2) {
		
		ball_collision_action_score(0);
			
		
	// Collistion on right player
	} else if (player_ball[0] >= 95 - (PLAYER_INDENT + 2 + ball_size/2) && player_ball[0] <= 95 - (PLAYER_INDENT + 1 + ball_size/2)
		&& (player_ball[1] >= player_coordinates[ID_PLAYER_RIGHT] - player_heights[ID_PLAYER_LEFT]/2 && player_ball[1] <= player_coordinates[ID_PLAYER_RIGHT] + player_heights[ID_PLAYER_LEFT])) {

		ball_collision_action_player_bounce(ID_PLAYER_RIGHT);
		
	// Collistion on score (beside right player)
	} else if (player_ball[0] >= 94 - ball_size/2) {
		
		ball_collision_action_score(1);
		
	}
	
}

// Draw playing field bounds
void draw_playing_field(void) {
	int i;
	for(i=0;i<96;i++) {
		play_xy_set(i,0);
		play_xy_set(i,31);
	}
}

void draw_players(void) {
	int i;
	
	for(i=0;i<player_heights[ID_PLAYER_LEFT];i++) {
		play_xy_set(PLAYER_INDENT+0,(player_coordinates[ID_PLAYER_LEFT] - player_heights[ID_PLAYER_LEFT]/2)+i);
		play_xy_set(PLAYER_INDENT+1,(player_coordinates[ID_PLAYER_LEFT] - player_heights[ID_PLAYER_LEFT]/2)+i);
	}
	
	for(i=0;i<player_heights[ID_PLAYER_RIGHT];i++) {
		play_xy_set(-PLAYER_INDENT+94,(player_coordinates[ID_PLAYER_RIGHT] - player_heights[ID_PLAYER_RIGHT]/2)+i);
		play_xy_set(-PLAYER_INDENT+95,(player_coordinates[ID_PLAYER_RIGHT] - player_heights[ID_PLAYER_RIGHT]/2)+i);
	}
	
}

void draw_ball(void) {
	int i, j;
	
	for(i=0;i<ball_size;i++) {
		for(j=0;j<ball_size;j++) {
			play_xy_set((player_ball[0] - ball_size/2)+i ,(player_ball[1] - ball_size/2)+j);
			
		}
	}
}

void draw_message(char *message) {

	int r, i, j, k, row_off;
	int message_lines = 0;
	int row_size[] = {0, 0, 0};
	int message_size = 0;
	int total_size = 0;
	
	// Get message size
	for(i = 0; i < 33; i++) {
		if(*message == 0xa) {
			message++;
			total_size++;
			if(row_size[message_lines] > message_size)
				message_size = row_size[message_lines];
			message_lines++;
		} else if(*message) {
			row_size[message_lines]++;
			message++;
			total_size++;
		}
	}
	
	if(row_size[message_lines] > message_size)
		message_size = row_size[message_lines];
	
	// Reset
	message -= total_size;
	
	// Clear Square
	for(i=8-(message_lines*3);i<23+(message_lines*3);i++) {
		for(j=(46-4*message_size);j<(49+4*message_size);j++) {
			play_xy_clear(j,i);
		}
	}
	
	// Square / Top+Bottom
	for(i=(46-4*message_size);i<(49+4*message_size);i++) {
		play_xy_set(i,8-(message_lines*3));
		play_xy_set(i,22+(message_lines*3));
	}
	
	// Square / Left+Right
	for(i=8-(message_lines*3);i<23+(message_lines*3);i++) {
		play_xy_set((46-4*message_size),i);
		play_xy_set((49+4*message_size),i);
	}

	// Text
	r = 0;
	row_off = 0;
	for(i = 0; i < total_size; i++) {
		
		if(message[i] == 0xa) {
			row_off += row_size[r]+1;
			r++;
			continue;
		}
		
		for(j = 0; j < 8; j++) {
			for(k = 0; k < 8; k++) {
				if ((font[message[i]*8+j] >> k) & 1)
					play_xy_set((48-4*row_size[r])+((i-row_off)*8+j), 12+k-(message_lines*4)+(r*8));
			}
		}
	}
}

void timer_countdown(void) {
	
	// Timestamp to seconds
	//int time_seconds = ((play_timestamp >> 16) & 0xf) * 60 * 10 + ((play_timestamp >> 12) & 0xf) * 60 + ((play_timestamp >> 4) & 0xf) * 10 + (play_timestamp & 0xf);
	
	// Decrement
	play_time_left--;
	
	if (play_time_left <= 0) {
		play_state = 0x80;
		// Catch holding
		player_holds[ID_PLAYER_LEFT] = 1;
		player_holds[ID_PLAYER_RIGHT] = 1;	
		
	} else
		play_timestamp = ((play_time_left/(60*10) & 0xf) << 16) | ((((play_time_left%(60*10))/60) & 0xf) << 12) | 0x00a00 | (((((play_time_left%(60*10))%60)/10) & 0xf) << 4) | (((((play_time_left%(60*10))%60)%10) & 0xf));
	
}

void draw_timer(int time) {
	int i, j, k;
	
	// Clear Square
	for(i=27;i<32;i++) {
		for(j=(47-(2*5+2));j<(48+(2*5+3));j++) {
			play_xy_clear(j,i);
		}
	}
	
	// Text
	for(i = 0; i < 5; i++) {
		for(j = 0; j < 4; j++) {
			for(k = 0; k < 4; k++) {
				if ((numbers_mini[((time >> 4*(4-i)) & 0xf)*4+j] >> (3-k)) & 1)
					play_xy_set((48-(2*5+2))+(i*4+j)+i, 28+k);
			}
		}
	}
}

void playing_field_update(void) {	
	
	// Clear playing field
	playbuffer_clear();
	
	// Draw field
	draw_playing_field();
	
	// Draw players
	draw_players();
	
	// Normal state
	if (play_state == 0) {
	
		// Timer
		if(play_mode_timed != 0) {
			// Set first timestamp
			if(play_timestamp == 0)
				play_timestamp = ((play_time_left/(60*10) & 0xf) << 16) | ((((play_time_left%(60*10))/60) & 0xf) << 12) | 0x00a00 | (((((play_time_left%(60*10))%60)/10) & 0xf) << 4) | (((((play_time_left%(60*10))%60)%10) & 0xf));
			
			draw_timer(play_timestamp);
		
		}
	
	}
	
	// Draw ball
	draw_ball();
	
	// Paused state
	if ((play_state >> 6) & 1) {
		// Pause overlay
		if (((play_state >> 1) & 1)) {
			draw_message("QUIT?\nCONFIRM\nWITH 'A'");
		} else if (((play_state >> 6) & 1) && (play_state & 1) == (ID_PLAYER_LEFT & 1)) {
			draw_message("<PAUSE<");
		} else if (((play_state >> 6) & 1) && (play_state & 1) == (ID_PLAYER_RIGHT & 1)) {
			draw_message(">PAUSE>");
		}
		
	// Game over
	} else if (((play_state >> 7) & 1) && !(play_state & 1)) {
		// Score met
		if(sc1 > sc2)
			draw_message("GAME OVER\nLEFT WON!");
		else if(sc2 > sc1)
			if(in_game == 1)
				draw_message("GAME OVER\nCPU WON!");
			else
				draw_message("GAME OVER\nRIGHT WON!");
		else if(sc1 == sc2)
			draw_message("GAME OVER\nIT'S A\nDRAW");
		
	} else if (((play_state >> 7) & 1) && (play_state & 1)) {
		draw_message("(A) REPLAY\n(B) EXIT");
		
	}
	
	play_xy_update();
}

void playing_reset(int exit) {
	// Reset status
	ball_speed_temp = ball_speed;
	play_time_left = play_mode_timed; 
	play_state = 0;
	play_timestamp = 0;
	player_ball_direction = -1;
	player_ball[0] = 48;
	player_ball[1] = 16;
	player_coordinates[0] = 16;
	player_coordinates[1] = 16;
	player_commands[0] = 0;
	player_commands[1] = 0;
	player_holds[0] = 0;
	player_holds[1] = 0;
	
	// Unload all data
	sc1 = 0;
	sc2 = 0;

	// Reset game	
  //screen_clear();
  //game_countdown();
  playbuffer_clear();
  screen_clear();
	display_score(0, 0);
	display_score(0, 1);
	
	// Exit to menu
	if (exit)
		in_game = 0;
	
}