#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include <math.h>
#include "project.h"

uint8_t playbuffer[4][96];
uint8_t player_left = 16;
uint8_t player_right = 16;
uint8_t player_ball[] = {48, 16};
uint8_t player_ball_x[96];
uint8_t player_ball_y[96];
uint8_t player_ball_movement_pointer = 0;
uint8_t player_height = 8;
uint8_t ball_width = 2;
uint8_t ball_height = 2;
double player_ball_direction = PI+PI/4;
uint8_t player_ball_speed = 1;

void screen_clear(void) {
	int i;
	int j = 0;

	// Clear screen
	for(j;j<4;j++) {

		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(j);	// row 0, 1, 2, 3
		spi_send_recv(0x0);
		spi_send_recv(0x10);
		DISPLAY_CHANGE_TO_DATA_MODE;

		i = 0;

		for(;i<128;i++) {
			spi_send_recv(0);
		}

	}
}

void playbuffer_clear(void) {
	int i;
	int j;

	for(j = 0; j < 4; j++) {

		for(i = 0; i < 96; i++) {
			playbuffer[j][i] = 0;

		}

	}

}

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

// Translate X Y to page and colum
void play_xy(int x, int y) {

	playbuffer[y/8][x] = playbuffer[y/8][x] | (1 << y%8);

}

void playing_field_init(void) {
	int i;
	for(i=0;i<96;i++) {
		play_xy(i,0);
		play_xy(i,31);
	}
}

void move_player_left(uint8_t rel_y) {
	player_left += rel_y;
	
	// Inside bounds
	if (player_left <= player_height/2 + 1)
		player_left = player_height/2 + 1;
	else if (player_left >= 31 - player_height/2)
		player_left = 31 - player_height/2;
}

void move_player_right(uint8_t rel_y) {
	player_right += rel_y;
	
	// Inside bounds
	if (player_right <= player_height/2 + 1)
		player_right = player_height/2 + 1;
	else if (player_right >= 31 - player_height/2)
		player_right = 31 - player_height/2;
}


void move_ball() {
	// Calculate new movement
	if (player_ball_movement_pointer == 0) {
		int i;
		// Calculations
		for(i = 0; i < 96; i++) {
			player_ball_x[i] = (uint8_t)(player_ball[0] + (cos(player_ball_direction) * (double)(i+1)));
			player_ball_y[i] = (uint8_t)(player_ball[1] - (sin(player_ball_direction) * (double)(i+1)));
			
		}
		
	}
	
	// Move
	player_ball[0] = player_ball_x[player_ball_movement_pointer];
	player_ball[1] = player_ball_y[player_ball_movement_pointer];
	
	player_ball_movement_pointer++;
	
}

void check_player_moves(void) {
	// Button inputs
	int buttons = input_get_buttons();
	
	// Button 4 is pressed
	if ((buttons >> 3) & 1) {
		move_player_left(-1);
	}
	
	// Button 3 is pressed
	if ((buttons >> 2) & 1)
	{
		move_player_left(1);
	}
		
	// Button 2 is pressed
	if ((buttons >> 1) & 1)
	{
		move_player_right(-1);
	}
		
	// Button 1 is pressed
	if (buttons & 1) {
		move_player_right(1);
	}
	
}

void ball_collision_detection(void) {
	
}

void draw_players(void) {
	int i;
	
	for(i=0;i<player_height;i++) {
		play_xy(0,(player_left - player_height/2)+i);
		play_xy(1,(player_left - player_height/2)+i);
	}
	
	for(i=0;i<player_height;i++) {
		play_xy(94,(player_right - player_height/2)+i);
		play_xy(95,(player_right - player_height/2)+i);
	}
	
}
void draw_ball(void) {
	int i, j;
	
	for(i=0;i<ball_width;i++) {
		for(j=0;j<ball_height;j++) {
			play_xy((player_ball[0] - ball_width/2)+i ,(player_ball[1] - ball_height/2)+j);
			
		}
	}
}


void playing_field_update(void) {
	int i;
	
	// Clear playing field
	playbuffer_clear();
	
	// Draw field
	playing_field_init();
	
	// Draw players
	draw_players();
	
	// Draw ball
	draw_ball();
	
	play_xy_update();
}