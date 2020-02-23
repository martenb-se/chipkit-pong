#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"
#include <math.h>

int errno = 0;

uint8_t play_mode_score = 99;
int play_mode_timed = 90;
int play_time_left = 90;
int play_timestamp = 0;
uint8_t player_cpu = 3;
uint8_t player_cpu_hold = 0;
uint8_t player_left_pause = 0;
uint8_t player_right_pause = 0;
uint8_t play_game_over = 0;
uint8_t play_quit_init = 0;
uint8_t ball_speed = 2;
uint8_t ball_speed_temp = 2;
uint8_t player_left_speed = 2;
uint8_t player_right_speed = 2;
uint8_t player_left_command = 0;
uint8_t player_right_command = 0;
uint8_t player_left_hold = 0;
uint8_t player_right_hold = 0;
uint8_t player_indent = 5;
uint8_t playbuffer[4][96];
uint8_t player_left = 16;
uint8_t player_right = 16;
uint8_t player_ball[] = {48, 16};
uint8_t player_ball_x[96];
uint8_t player_ball_y[96];
double player_ball_movement_pointer = 0;
uint8_t player_left_height = 8;
uint8_t player_right_height = 8;
uint8_t ball_width = 2;
uint8_t ball_height = 2;
double player_ball_direction = -1;
unsigned int rand_next = 0xd131;

unsigned int rand(void)
{
	rand_next = TMR3;
	
	return rand_next;
}

double pow_simle(double base, int exp) {
	int i;
	double res = base;
	if (exp > 0) {
		for(i = 0; i < exp; i++){
			res *= base;
			
		}
	} else if (exp < 0) {
		for(i = 0; i > exp; i--){
			res *= 1/base;
			
		}
	} else {
		return 1;
	}
}

double acos_simple(double x) {
   return (-0.69813170079773212 * x * x - 0.87266462599716477) * x + 1.5707963267948966;
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

void play_xy_clear(int x, int y) {

	playbuffer[y/8][x] = playbuffer[y/8][x] & ~(1 << y%8);

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
	if (player_left <= player_left_height/2 + 1)
		player_left = player_left_height/2 + 1;
	else if (player_left >= 31 - player_left_height/2)
		player_left = 31 - player_left_height/2;
}

void move_player_right(uint8_t rel_y) {
	player_right += rel_y;
	
	// Inside bounds
	if (player_right <= player_right_height/2 + 1)
		player_right = player_right_height/2 + 1;
	else if (player_right >= 31 - player_right_height/2)
		player_right = 31 - player_right_height/2;
}


void move_ball() {
	// Randomize first movement
	if (player_ball_direction == -1)
		player_ball_direction = PI/6 + (PI/6)/rand() + (PI/2) * (rand()%4);
	
	// Calculate new movementsdf
	if (player_ball_movement_pointer == 0) {
		
		int i;
		// Calculations
		for(i = 0; i < 96; i++) {
			// Small angles should be straight
			if(player_ball_direction > PI - PI/12
				&& player_ball_direction < PI + PI/12
				|| player_ball_direction < PI/12
				&& player_ball_direction > 3*(PI/2) + (5*PI)/12) {
				
				player_ball_y[i] = player_ball[1];
				
			// Angled
			} else {
				player_ball_y[i] = (uint8_t)(player_ball[1] - (sin(player_ball_direction) * (double)(i+1)));
				
			}
			
			player_ball_x[i] = (uint8_t)(player_ball[0] + (cos(player_ball_direction) * (double)(i+1)));
			
		}
		
	}
	
	// Move
	player_ball[0] = player_ball_x[(int)player_ball_movement_pointer];
	player_ball[1] = player_ball_y[(int)player_ball_movement_pointer];
	
	if(ball_speed_temp != ball_speed)
		player_ball_movement_pointer += 1/(double)ball_speed_temp;
	else
		player_ball_movement_pointer += 1/(double)ball_speed;
	
}

void check_player_moves(void) {
	
	// Debug
	//if(!controller_input_a || PORTE > 254)
	//	PORTE = 0;
	//else
	//	PORTE++;
	
	// Not game over
	if(play_game_over == 0) {
	
		// Not paused
		if (player_left_pause == 0 && player_right_pause == 0) {
			
			// Controller support
			
			// Left controller UP is pressed
			if ((controller_input_a >> 2) & 1) {
				// Speed slowdown
				if (player_left_speed > 1) {
					// Is player holding down button
					if (player_left_command == controller_input_a)
						player_left_hold++;
					else
						player_left_hold = 1;
					
					// Held for required amount of 
					// - Hold A for super slowdown
					if (((controller_input_a >> 6) & 1) && player_left_hold == (player_left_speed*2)
						|| !((controller_input_a >> 6) & 1) && player_left_hold == player_left_speed) {
						move_player_left(-1);
						// Reset hold
						player_left_hold = 0;
					}
					
				// Use normal speed
				} else
					move_player_left(-1);
					
				// Set last commands
				player_left_command = controller_input_a;
			}
			
			// Left controller DOWN is pressed
			if ((controller_input_a >> 1) & 1)
			{
				// Speed slowdown
				if (player_left_speed > 1) {
					// Is player holding down button
					if (player_left_command == controller_input_a)
						player_left_hold++;
					else
						player_left_hold = 1;
					
					// Held for required amount of time
					// - Hold A for super slowdown
					if (((controller_input_a >> 6) & 1) && player_left_hold == (player_left_speed*2)
						|| !((controller_input_a >> 6) & 1) && player_left_hold == player_left_speed) {
						move_player_left(1);
						// Reset hold
						player_left_hold = 0;
					}
					
				// Use normal speed
				} else
					move_player_left(1);
					
				// Set last commands
				player_left_command = controller_input_a;
			}
			
			// 1 Player mode
			if(in_game == 1) {
				// CPU Level 1 - Easy
				// Distance and speed:
				//  < 10 px = User/3
				//  < 30 px = User/4
				//  < 50 px = User/5
				//  > 50 px = User/6
				if (player_cpu == 1) {
					if (player_ball[0] > 86
						&& player_cpu_hold >= (player_left_speed*3-1)
						|| player_ball[0] > 66
						&& player_cpu_hold >= (player_left_speed*4-1)
						|| player_ball[0] > 46
						&& player_cpu_hold >= (player_left_speed*5-1)
						|| player_cpu_hold >= (player_left_speed*6-1)) {
						if (player_right > player_ball[1]) {
							move_player_right(-1);
							
						} else if (player_right < player_ball[1]) {
							move_player_right(1);
							
						}
						
						player_cpu_hold = 0;
						
					}
					
					player_cpu_hold++;
					
				// CPU Level 2 - Medium
				// Distance and speed:
				//  < 10 px = User/2
				//  < 30 px = User/3
				//  < 50 px = User/4
				//  > 50 px = User/5
				} else if (player_cpu == 2) {
					if (player_ball[0] > 86
						&& player_cpu_hold >= (player_left_speed*2-1)
						|| player_ball[0] > 66
						&& player_cpu_hold >= (player_left_speed*3-1)
						|| player_ball[0] > 46
						&& player_cpu_hold >= (player_left_speed*4-1)
						|| player_cpu_hold >= (player_left_speed*5-1)) {
						if (player_right > player_ball[1]) {
							move_player_right(-1);
							
						} else if (player_right < player_ball[1]) {
							move_player_right(1);
							
						}
						
						player_cpu_hold = 0;
						
					}
					
					player_cpu_hold++;
					
				// CPU Level 3 - Hard
				// Distance and speed:
				//  < 10 px = User
				//  < 30 px = User/2
				//  < 50 px = User/3
				//  > 50 px = User/4
				} else if (player_cpu == 3) {
					if (player_ball[0] > 86
						&& player_cpu_hold >= (player_left_speed-1)
						|| player_ball[0] > 66
						&& player_cpu_hold >= (player_left_speed*2-1)
						|| player_ball[0] > 46
						&& player_cpu_hold >= (player_left_speed*3-1)
						|| player_cpu_hold >= (player_left_speed*4-1)) {
						if (player_right > player_ball[1]) {
							move_player_right(-1);
							
						} else if (player_right < player_ball[1]) {
							move_player_right(1);
							
						}
						
						player_cpu_hold = 0;
						
					}
					
					player_cpu_hold++;
					
				// CPU Level 4 - Impossible
				// Distance and speed:
				//  < 50 px = User
				//  > 50 px = User/2
				} else if (player_cpu == 4) {
					if (player_ball[0] > 46
						&& player_cpu_hold >= (player_left_speed-1)
						|| player_cpu_hold >= (player_left_speed*2-1)) {
						if (player_right > player_ball[1]) {
							move_player_right(-1);
							
						} else if (player_right < player_ball[1]) {
							move_player_right(1);
							
						}
						
						player_cpu_hold = 0;
						
					}
					
					player_cpu_hold++;
					
				}
			
			// 2 Player mode
			} else if(in_game == 2) {
			
				// Right controller UP is pressed
				if ((controller_input_b >> 2) & 1) {
					// Speed slowdown
					if (player_right_speed > 1) {
						// Is player holding down button
						if (player_right_command == controller_input_b)
							player_right_hold++;
						else
							player_right_hold = 1;
						
						// Held for required amount of time
						// - Hold A for super slowdown
						if (((controller_input_a >> 6) & 1) && player_right_hold == (player_right_speed*2)
							|| !((controller_input_a >> 6) & 1) && player_right_hold == player_right_speed) {
							move_player_right(-1);
							// Reset hold
							player_right_hold = 0;
						}
						
					// Use normal speed
					} else
						move_player_right(-1);
						
					// Set last commands
					player_right_command = controller_input_b;
				}
				
				// Right controller DOWN is pressed
				if ((controller_input_b >> 1) & 1)
				{
					// Speed slowdown
					if (player_right_speed > 1) {
						// Is player holding down button
						if (player_right_command == controller_input_b)
							player_right_hold++;
						else
							player_right_hold = 1;
						
						// Held for required amount of time
						// - Hold A for super slowdown
						if (((controller_input_a >> 6) & 1) && player_right_hold == (player_right_speed*2)
							|| !((controller_input_a >> 6) & 1) && player_right_hold == player_right_speed) {
							move_player_right(1);
							// Reset hold
							player_right_hold = 0;
						}
						
					// Use normal speed
					} else
						move_player_right(1);
						
					// Set last commands
					player_right_command = controller_input_b;
				}
			
			}
		
		}
		
		// Pause: Left Player
		// Not paused by right player
		if (player_right_pause == 0) {
			// Pause - left player
			if (player_left_pause == 0) {
				if (player_left_command != controller_input_a && (controller_input_a >> 3) & 1) {
					player_left_pause = 1;
				}
				player_left_command = controller_input_a;
			// Unpause/Quit - left player
			} else if (player_left_pause == 1) {
				// Unpause
				if (player_left_command != controller_input_a && (controller_input_a >> 3) & 1) {
					player_left_pause = 0;
					play_quit_init = 0;
				// Quit?
				} else if (play_quit_init == 0 && (controller_input_a >> 5) & 1) {
					play_quit_init = 1;
				// Confirm quit
				} else if (play_quit_init == 1 && (controller_input_a >> 6) & 1) {
					player_left_pause = 0;
					play_quit_init = 0;
					playing_exit();
				}
				player_left_command = controller_input_a;
			}
		}
		
		// Pause: Right Player
		// Not paused by left player and only in 2p mode
		if (player_left_pause == 0 && in_game == 2) {
			// Pause - right player
			if (player_right_pause == 0) {
				if (player_right_command != controller_input_b && (controller_input_b >> 3) & 1) {
					player_right_pause = 1;
				}
				player_right_command = controller_input_b;
			// Unpause/Quit - right player
			} else if (player_right_pause == 1) {
				// Unpause
				if (player_right_command != controller_input_b && (controller_input_b >> 3) & 1) {
					player_right_pause = 0;
					play_quit_init = 0;
				// Quit?
				} else if (play_quit_init == 0 && (controller_input_b >> 5) & 1) {
					play_quit_init = 1;
				// Confirm quit
				} else if (play_quit_init == 1 && (controller_input_b >> 6) & 1) {
					player_right_pause = 0;
					play_quit_init = 0;
					playing_exit();
				}
				player_right_command = controller_input_b;
			}
		}
	
	// Game over
	} else {
		// Catch hold
		if(player_left_hold || (player_right_hold && in_game == 2)) {
			if(!(controller_input_a) && (!(controller_input_b) || in_game == 1)) {
				player_left_hold = 0;
				player_right_hold = 0;
				
			}
		
		// Game Over - Any button
		} else if(play_game_over == 1 && (controller_input_a || (player_right_hold && in_game == 2))) {
			// Next game over screen
			play_game_over = 2;
			// Last command
			player_left_hold = 1;
			player_right_hold = 1;
		
		// Game Over - Replay
		} else if(play_game_over == 2 && ((controller_input_a >> 6) & 1 || (((controller_input_b >> 6) & 1) && in_game == 2) )) {
			playing_replay();
			
		// Game Over - Exit
		} else if(play_game_over == 2 && ((controller_input_a >> 5) & 1 || (((controller_input_b >> 5) & 1) && in_game == 2) )) {
			playing_exit();
			
		}
		
	}
	
}

double ball_bounce_calculation_x(double angle) {
	double angle_bounce = 0;
	
	angle_bounce = (-angle + (2*PI));
	if (angle_bounce > (2*PI))
		angle_bounce -= (2*PI);
	
	return angle_bounce;
}

double ball_bounce_calculation_y(double angle) {
	double angle_bounce = 0;
	
	angle_bounce = (-angle + PI + (2*PI));
	if (angle_bounce > (2*PI))
		angle_bounce -= (2*PI);
	
	return angle_bounce;
}

void ball_collision_detection(void) {
	
	// Out of bounds fix
	if (player_ball[1] <= ball_height/2)
		player_ball[1] = ball_height/2 + 2;
		
	if (player_ball[1] >= 32 - ball_height/2)
		player_ball[1] = 30 - (ball_height/2);
	
	// Check for walls
	// Collision on upper wall
	if (player_ball[1] == ball_height/2 + 1 && 
		player_ball_direction > 0 && player_ball_direction < PI) {
			
		// Move ball
		player_ball[1] = ball_height/2 + 2;
		
		// Bounce
		player_ball_direction = ball_bounce_calculation_x(player_ball_direction);
		
		// Reset movement pointer
		player_ball_movement_pointer = 0;
		
	// Collision on lower wall
	} else if (player_ball[1] == 31 - ball_height/2 &&
		player_ball_direction > PI && player_ball_direction < 2*PI) {
		
		// Move ball
		player_ball[1] = 30 - (ball_height/2);
		
		// Bounce
		player_ball_direction = ball_bounce_calculation_x(player_ball_direction);
		
		// Reset movement pointer
		player_ball_movement_pointer = 0;
	
	// Collistion on left player
	} else if (player_ball[0] <= player_indent + 2 + ball_width/2 && player_ball[0] >= player_indent + 1 + ball_width/2
		&& (player_ball[1] >= player_left - player_left_height/2 && player_ball[1] <= player_left + player_left_height/2)) {
		
		// Move ball
		player_ball[0] = player_indent + 2 + ball_width/2 + 1;
		
		// Bounce
		// Upper Edge
		if(player_ball[1] >= player_left - player_left_height/2 && player_ball[1] < player_left - player_left_height/3) {
			
			// Make angle	
			if(player_ball_direction > PI - PI/12
				&& player_ball_direction < PI + PI/12) {
				player_ball_direction = PI/6;
			
			// Moving down - return
			} else if(player_ball_direction > PI && player_ball_direction < 2*PI) {
				player_ball_direction += PI;
				if(player_ball_direction > 2*PI)
					player_ball_direction -= 2*PI;
					
				// Reset speed
				ball_speed_temp = ball_speed;
		
			// Moving up - Increase speed
			} else if (player_ball_direction > 0 && player_ball_direction < PI) {
				//player_ball_direction = ball_bounce_calculation_y(player_ball_direction);
				player_ball_direction = PI/3;
				ball_speed_temp = ball_speed/2;
			
			// Make angle	
			} else {
				player_ball_direction = PI/6;
				
			}
			
		// Lower Edge
		} else if(player_ball[1] <= player_left + player_left_height/2 && player_ball[1] > player_left + player_left_height/3) {

			// Make angle	
			if(player_ball_direction > PI - PI/12
				&& player_ball_direction < PI + PI/12) {
				player_ball_direction = 11*(PI/6);
				
			// Moving up - Return
			} else if (player_ball_direction > 0 && player_ball_direction < PI) {
				player_ball_direction += PI;
				if(player_ball_direction > 2*PI)
					player_ball_direction -= 2*PI;
				
				// Reset speed
				ball_speed_temp = ball_speed;
		
			// Moving down - Increase speed and angle
			} else if(player_ball_direction > PI && player_ball_direction < 2*PI) {
				//player_ball_direction = ball_bounce_calculation_y(player_ball_direction);
				player_ball_direction = 5*(PI/3);
				ball_speed_temp = ball_speed/2;
				
			// Make angle	
			} else {
				player_ball_direction = 11*(PI/6);
				
			}
		
		// Middle	
		} else if (player_ball[1] >= player_left - 1 && player_ball[1] <= player_left + 1) {
			// Increase speed
			if(player_ball_direction > PI - PI/12
				&& player_ball_direction < PI + PI/12) {
				ball_speed_temp = ball_speed/2;
			}
			
			player_ball_direction = 0;
		
		// Bounce	
		} else {
			player_ball_direction = ball_bounce_calculation_y(player_ball_direction);
			
		}
		
		// Reset movement pointer
		player_ball_movement_pointer = 0;
		
	// Collistion on score (beside left player)
	} else if (player_ball[0] <= ball_width/2 + 2) {
		
		// Reset speed
		ball_speed_temp = ball_speed;
		
		// Reset ball
		player_ball[0] = 48;
		player_ball[1] = 16;
		
		// Direct opposite
		//player_ball_direction = PI/3;
		player_ball_direction = PI/6 + (PI/6)/rand() + (3*(PI/2)) * (rand()%2);
		
		player_ball_movement_pointer = 0;
		
		// Update scores
		display_right_score_update();
		
		// Game over?
		if(play_mode_score > 0 && sc2 == play_mode_score) {
			play_game_over = 1;
			// Catch holding
			player_left_hold = 1;
			player_right_hold = 1;	
		}
			
		
	// Collistion on right player
	} else if (player_ball[0] >= 95 - (player_indent + 2 + ball_width/2) && player_ball[0] <= 95 - (player_indent + 1 + ball_width/2)
		&& (player_ball[1] >= player_right - player_right_height/2 && player_ball[1] <= player_right + player_right_height/2)) {
			
		// Reset speed
		ball_speed_temp = ball_speed;
		
		// Move ball
		player_ball[0] = 95 - (player_indent + 2 + ball_width/2) - 1;
		
		// Bounce
		//player_ball_direction = ball_bounce_calculation_y(player_ball_direction);
		// Bounce
		// Upper Edge
		if(player_ball[1] >= player_right - player_right_height/2 && player_ball[1] < player_right - player_right_height/3) {
			
			// Make angle	
			if(player_ball_direction < PI/12
				|| player_ball_direction > 3*(PI/2) + (5*PI)/12) {
				player_ball_direction = 5*(PI/6);
			
			// Moving down - return
			} else if(player_ball_direction > PI && player_ball_direction < 2*PI) {
				player_ball_direction += PI;
				if(player_ball_direction > 2*PI)
					player_ball_direction -= 2*PI;
		
			// Moving up - Increase speed
			} else if (player_ball_direction > 0 && player_ball_direction < PI) {
				//player_ball_direction = ball_bounce_calculation_y(player_ball_direction);
				player_ball_direction = 2*(PI/3);
				ball_speed_temp = ball_speed/2;
			
			// Make angle	
			} else {
				player_ball_direction = 5*(PI/6);
				
			}
			
		// Lower Edge
		} else if(player_ball[1] <= player_right + player_right_height/2 && player_ball[1] > player_right + player_right_height/3) {

			// Make angle	
			if(player_ball_direction < PI/12
				|| player_ball_direction > 3*(PI/2) + (5*PI)/12) {
				player_ball_direction = 7*(PI/6);
				
			// Moving up - Return
			} else if (player_ball_direction > 0 && player_ball_direction < PI) {
				player_ball_direction += PI;
				if(player_ball_direction > 2*PI)
					player_ball_direction -= 2*PI;
		
			// Moving down - Increase speed
			} else if(player_ball_direction > PI && player_ball_direction < 2*PI) {
				//player_ball_direction = ball_bounce_calculation_y(player_ball_direction);
				player_ball_direction = 4*(PI/3);
				ball_speed_temp = ball_speed/2;
				
			// Make angle	
			} else {
				player_ball_direction = 7*(PI/6);
				
			}
		
		// Middle	
		} else if (player_ball[1] >= player_right - 1 && player_ball[1] <= player_right + 1) {
			// Increase speed
			if(player_ball_direction < PI/12
				|| player_ball_direction > 3*(PI/2) + (5*PI)/12) {
				ball_speed_temp = ball_speed/2;
			}
			
			player_ball_direction = PI;
		
		// Bounce	
		} else {
			player_ball_direction = ball_bounce_calculation_y(player_ball_direction);
			
		}
			
		player_ball_movement_pointer = 0;
		
	// Collistion on score (beside right player)
	} else if (player_ball[0] >= 94 - ball_width/2) {
		
		// Reset speed
		ball_speed_temp = ball_speed;

		// Reset ball
		player_ball[0] = 48;
		player_ball[1] = 16;
		
		// Direct opposite
		player_ball_direction = (PI/2 + PI/6) + (PI/6)/rand() + (PI/2) * (rand()%2);
		
		player_ball_movement_pointer = 0;
		
		// Update scores
		display_left_score_update();
		
		// Game over?
		if(play_mode_score > 0 && sc1 == play_mode_score) {
			play_game_over = 1;
			// Catch holding
			player_left_hold = 1;
			player_right_hold = 1;	
		}
		
	}
	
}

void draw_players(void) {
	int i;
	
	for(i=0;i<player_left_height;i++) {
		play_xy(player_indent+0,(player_left - player_left_height/2)+i);
		play_xy(player_indent+1,(player_left - player_left_height/2)+i);
	}
	
	for(i=0;i<player_right_height;i++) {
		play_xy(-player_indent+94,(player_right - player_right_height/2)+i);
		play_xy(-player_indent+95,(player_right - player_right_height/2)+i);
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
		play_xy(i,8-(message_lines*3));
		play_xy(i,22+(message_lines*3));
	}
	
	// Square / Left+Right
	for(i=8-(message_lines*3);i<23+(message_lines*3);i++) {
		play_xy((46-4*message_size),i);
		play_xy((49+4*message_size),i);
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
					play_xy((48-4*row_size[r])+((i-row_off)*8+j), 12+k-(message_lines*4)+(r*8));
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
		play_game_over = 1;
		// Catch holding
		player_left_hold = 1;
		player_right_hold = 1;
		
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
					play_xy((48-(2*5+2))+(i*4+j)+i, 28+k);
			}
		}
	}
}

void playing_field_update(void) {	
	// Clear playing field
	playbuffer_clear();
	
	// Draw field
	playing_field_init();
	
	// Draw players
	draw_players();
	
	// Timer
	if(play_mode_timed != 0) {
		// Set first timestamp
		if(play_timestamp == 0)
			play_timestamp = ((play_time_left/(60*10) & 0xf) << 16) | ((((play_time_left%(60*10))/60) & 0xf) << 12) | 0x00a00 | (((((play_time_left%(60*10))%60)/10) & 0xf) << 4) | (((((play_time_left%(60*10))%60)%10) & 0xf));
		
		draw_timer(play_timestamp);
	
	}
	
	// Draw ball
	draw_ball();
	
	play_xy_update();
}

void playing_field_paused(void) {
	// Clear playing field
	playbuffer_clear();
	
	// Draw field
	playing_field_init();
	
	// Draw players
	draw_players();
	
	// Draw ball
	draw_ball();
	
	// Pause overlay
	if (play_quit_init == 1) {
		draw_message("QUIT?\nCONFIRM\nWITH 'A'");
	} else if (player_left_pause == 1) {
		draw_message("<PAUSE<");
	} else if (player_right_pause == 1) {
		draw_message(">PAUSE>");
	}
	
	play_xy_update();
}

void playing_field_game_over(void) {
	// Clear playing field
	playbuffer_clear();
	
	// Draw field
	playing_field_init();
	
	// Draw players
	draw_players();
	
	// Draw ball
	draw_ball();
	
	if (play_game_over == 1) {
		// Score met
		if(sc1 > sc2)
			draw_message("GAME OVER\nLEFT WON!");
		else if(sc2 > sc1)
			if(player_cpu > 0)
				draw_message("GAME OVER\nCPU WON!");
			else
				draw_message("GAME OVER\nRIGHT WON!");
		else if(sc1 == sc2)
			draw_message("GAME OVER\nIT'S A\nDRAW");
		
	} else if (play_game_over == 2) {
		draw_message("(A) REPLAY\n(B) EXIT");
		
	}
		
	play_xy_update();
}

void playing_replay(void) {
	// Reset status
	ball_speed_temp = ball_speed;
	play_time_left = play_mode_timed; 
	play_game_over = 0;
	player_left_pause = 0;
	player_right_pause = 0;
	player_ball_direction = -1;
	
	// Unload all data
	sc1 = -1;
	sc2 = -1;

	// Reset game	
  screen_clear();
  game_countdown();
  screen_clear();
	playing_field_init();
	display_left_score_update();
	display_right_score_update();
	
}

void playing_exit(void) {
	// Display score and so on..
	
	
	// Reset status
	ball_speed_temp = ball_speed;
	play_time_left = play_mode_timed; 
	play_game_over = 0;
	player_left_pause = 0;
	player_right_pause = 0;
	player_ball_direction = -1;
	
	// Unload all data
	sc1 = -1;
	sc2 = -1;
	
	// Exit to menu
	in_game = 0;
}