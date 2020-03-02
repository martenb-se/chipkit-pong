#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"


uint8_t selected_option;
uint8_t button_press;
uint8_t in_menu;
uint8_t in_options = 0;
uint8_t option_row = 1;
uint8_t cur_option_pad;
uint8_t cur_option_ballsz;
uint8_t cur_option_ballspd;
uint8_t cur_option_diff;
uint8_t blink_off_or_on;
char menu_gametitle[] = "PONGYANG EXTREME";
char opt_pad_size[] = "PAD SZ:";
char opt_pad_size_small[] = "SMALL";
char opt_pad_size_medium[] = "MED";
char opt_pad_size_large[] = "LARGE";
char opt_ball_speed[] = "B SPD:";
char opt_ball_speed_slow[] = "SLOW";
char opt_ball_speed_fast[] = "FAST";
char opt_ball_size[] = "B SZ:";
char opt_ball_size_2x2[] = "2x2";
char opt_ball_size_4x4[] = "4x4";
char opt_ball_size_6x6[] = "6x6";
char opt_game_difficulty[] = "DIFF:";
char opt_diff_easy[] = "EASY";
char opt_diff_normal[] = "NORM";
char opt_diff_hard[] = "HARD";
// char opt_diff_godmode[] = "GODMODE";


// prints out text centered in the chosen section of the screen
void menu_layout(char arr[], uint8_t len, uint8_t row, uint8_t sect)
{
  uint8_t j, k, l, c;
  l = (64 - len*8) / 2;
  if(sect == 0)
    l = 0;
  if(sect == 2)
    l = l + 64;
  if(sect == 3)
    l = (128 - len*8) / 2;

  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(row);
  spi_send_recv(0x0);
  spi_send_recv(0x21);
  spi_send_recv(l);
  spi_send_recv(l + len*8 - 1);
  DISPLAY_CHANGE_TO_DATA_MODE;
  for(j = 0; j < len; j++)
  {
    c = arr[j];
    for(k = 0; k < 8; k++)
      spi_send_recv(font[(c*8 + k)]);
  }
}


void game_countdown(void)
{
  char game_count_down[] = "321GO!";
  uint8_t j, k, c;

  for(j = 0; j < 3; j++)
  {
    DISPLAY_CHANGE_TO_COMMAND_MODE;
    spi_send_recv(0x22);
    spi_send_recv(1);
    spi_send_recv(0x0);
    spi_send_recv(0x21);
    spi_send_recv(60);
    spi_send_recv(68);
    DISPLAY_CHANGE_TO_DATA_MODE;
    c = game_count_down[j];
    for(k = 0; k < 8; k++)
      spi_send_recv(font[(c*8 + k)]);
    quicksleep(3000000);
  }
  screen_clear();
  {
    DISPLAY_CHANGE_TO_COMMAND_MODE;
    spi_send_recv(0x22);
    spi_send_recv(1);
    spi_send_recv(0x0);
    spi_send_recv(0x21);
    spi_send_recv(52);
    spi_send_recv(76);
    DISPLAY_CHANGE_TO_DATA_MODE;
    for(j = 3; j < 6; j++)
    {
      c = game_count_down[j];
      for(k = 0; k < 8; k++)
        spi_send_recv(font[(c*8 + k)]);
    }
    quicksleep(3000000);
  }
}


void start_menu(void)
{
  char menu_oneplayer[] = "1P";
  char menu_twoplayer[] = "2P";
  char menu_options[] = "OPT";
  char menu_credits[] = "CRED";

  menu_layout(menu_gametitle, 16, 0, 0);
  menu_layout(menu_oneplayer, sizeof(menu_oneplayer), 2, 1);
  menu_layout(menu_twoplayer, sizeof(menu_twoplayer), 2, 2);
  menu_layout(menu_options, sizeof(menu_options), 3, 1);
  menu_layout(menu_credits, sizeof(menu_credits), 3, 2);
}


void blink_selected(uint8_t row, uint8_t len)
{
  uint8_t i, k, l;
  l = (64 - len*8) / 2 + 64;

  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(row);
  spi_send_recv(0x0);
  spi_send_recv(0x21);
  spi_send_recv(l);
  spi_send_recv(l + len*8);
  DISPLAY_CHANGE_TO_DATA_MODE;
  for(i = 0; i < len; i++)
  {
    for(k = 0; k < 8; k++)
      spi_send_recv(font[(0 + k)]);
  }
}


// makes the selected option blink with help of a timer (writes and removes every second time)
void blink_function(void)
{
  if(option_row == 1 && cur_option_pad == 0)
  {
    if(blink_off_or_on == 0)
      blink_selected(0, sizeof(opt_pad_size_large));
    if(blink_off_or_on == 1)
      menu_layout(opt_pad_size_medium, sizeof(opt_pad_size_medium), 0, 2);
  }
  else if(option_row == 1 && cur_option_pad == 1)
  {
    if(blink_off_or_on == 0)
      blink_selected(0, sizeof(opt_pad_size_large));
    if(blink_off_or_on == 1)
      menu_layout(opt_pad_size_large, sizeof(opt_pad_size_large), 0, 2);
  }
  else if(option_row == 1 && cur_option_pad == 2)
  {
    if(blink_off_or_on == 0)
      blink_selected(0, sizeof(opt_pad_size_small));
    if(blink_off_or_on == 1)
      menu_layout(opt_pad_size_small, sizeof(opt_pad_size_small), 0, 2);
  }
  else if(option_row == 2 && cur_option_ballsz == 0)
  {
    if(blink_off_or_on == 0)
      blink_selected(1, sizeof(opt_ball_size_2x2));
    if(blink_off_or_on == 1)
      menu_layout(opt_ball_size_2x2, sizeof(opt_ball_size_2x2), 1, 2);
  }
  else if(option_row == 2 && cur_option_ballsz == 1)
  {
    if(blink_off_or_on == 0)
      blink_selected(1, sizeof(opt_ball_size_4x4));
    if(blink_off_or_on == 1)
      menu_layout(opt_ball_size_4x4, sizeof(opt_ball_size_4x4), 1, 2);
  }
  else if(option_row == 2 && cur_option_ballsz == 2)
  {
    if(blink_off_or_on == 0)
      blink_selected(1, sizeof(opt_ball_size_6x6));
    if(blink_off_or_on == 1)
      menu_layout(opt_ball_size_6x6, sizeof(opt_ball_size_6x6), 1, 2);
  }
  else if(option_row == 3 && cur_option_ballspd == 0)
  {
    if(blink_off_or_on == 0)
      blink_selected(2, sizeof(opt_diff_normal));
    if(blink_off_or_on == 1)
      menu_layout(opt_diff_normal, sizeof(opt_diff_normal), 2, 2);
  }
  else if(option_row == 3 && cur_option_ballspd == 1)
  {
    if(blink_off_or_on == 0)
      blink_selected(2, sizeof(opt_ball_speed_fast));
    if(blink_off_or_on == 1)
      menu_layout(opt_ball_speed_fast, sizeof(opt_ball_speed_fast), 2, 2);
  }
  else if(option_row == 3 && cur_option_ballspd == 2)
  {
    if(blink_off_or_on == 0)
      blink_selected(2, sizeof(opt_ball_speed_slow));
    if(blink_off_or_on == 1)
      menu_layout(opt_ball_speed_slow, sizeof(opt_ball_speed_slow), 2, 2);
  }
  else if(option_row == 4 && cur_option_diff == 0)
  {
    if(blink_off_or_on == 0)
      blink_selected(3, sizeof(opt_diff_normal));
    if(blink_off_or_on == 1)
      menu_layout(opt_diff_normal, sizeof(opt_diff_normal), 3, 2);
  }
  else if(option_row == 4 && cur_option_diff == 1)
  {
    if(blink_off_or_on == 0)
      blink_selected(3, sizeof(opt_diff_hard));
    if(blink_off_or_on == 1)
      menu_layout(opt_diff_hard, sizeof(opt_diff_hard), 3, 2);
  }
  else if(option_row == 4 && cur_option_diff == 2)
  {
    if(blink_off_or_on == 0)
      blink_selected(3, sizeof(opt_diff_easy));
    if(blink_off_or_on == 1)
      menu_layout(opt_diff_easy, sizeof(opt_diff_easy), 3, 2);
  }
  blink_off_or_on++;
  if(blink_off_or_on > 1)
    blink_off_or_on = 0;
  menu_layout(opt_pad_size, sizeof(opt_pad_size), 0, 1);
}


void options_menu(void)
{
  menu_layout(opt_pad_size, sizeof(opt_pad_size), 0, 1);
  menu_layout(opt_pad_size_medium, sizeof(opt_pad_size_medium), 0, 2);
  menu_layout(opt_ball_size, sizeof(opt_ball_size), 1, 1);
  menu_layout(opt_ball_size_2x2, sizeof(opt_ball_size_2x2), 1, 2);
  menu_layout(opt_ball_speed, sizeof(opt_ball_speed), 2, 1);
  menu_layout(opt_diff_normal, sizeof(opt_diff_normal), 2, 2);
  menu_layout(opt_game_difficulty, sizeof(opt_game_difficulty), 3, 1);
  menu_layout(opt_diff_normal, sizeof(opt_diff_normal), 3, 2);

  while(in_options)
  {
  	if(option_row == 1 && cur_option_pad == 0)
    {
      player_heights[0] = 10;
      player_heights[1] = 10;
  	}
    else if(option_row == 1 && cur_option_pad == 1)
    {
      player_heights[0] = 12;
      player_heights[1] = 12;
    }
    else if(option_row == 1 && cur_option_pad == 2)
    {
      player_heights[0] = 8;
      player_heights[1] = 8;
    }
    else if(option_row == 2 && cur_option_ballsz == 0)
      ball_size = 2;
    else if(option_row == 2 && cur_option_ballsz == 1)
      ball_size = 4;
    else if(option_row == 2 && cur_option_ballsz == 2)
      ball_size = 6;
    else if(option_row == 3 && cur_option_ballspd == 0)
      ball_speed = 2;
    else if(option_row == 3 && cur_option_ballspd == 1)
      ball_speed = 1;
    else if(option_row == 3 && cur_option_ballspd == 2)
      ball_speed = 4;
    else if(option_row == 4 && cur_option_diff == 0)
      player_cpu = 2;
    else if(option_row == 4 && cur_option_diff == 1)
      player_cpu = 3;
    else if(option_row == 4 && cur_option_diff == 2)
      player_cpu = 1;

    if (((controller_input_a >> 4) & 1)                                           // if SELECT is pressed
    || ((controller_input_b >> 4) & 1))			                                      // if SELECT is pressed
    {
    	if(sound_on == 1)
				play_sound(200, 50, 1);

      // increments each individual option to save what it was previously
      if(option_row == 1)
        cur_option_pad++;
      if(option_row == 2)
        cur_option_ballsz++;
      if(option_row == 3)
        cur_option_ballspd++;
      if(option_row == 4)
        cur_option_diff++;
    	if(cur_option_pad > 2)
    		cur_option_pad = 0;
      if(cur_option_ballsz > 2)
    		cur_option_ballsz = 0;
      if(cur_option_ballspd > 2)
        cur_option_ballspd = 0;
      if(cur_option_diff > 2)
        cur_option_diff = 0;
      while(((controller_input_a >> 4) & 1) || ((controller_input_b >> 4) & 1))
      {
    		// Wait here until key is released
    		quicksleep(10);
    	}
    }

    if (((controller_input_a >> 1) & 1)			                                      // if player left DOWN is pressed
  	|| ((controller_input_b >> 1) & 1))	                                          // if player right DOWN is pressed
    {
    	if(sound_on == 1)
				play_sound(200, 50, 1);
      blink_off_or_on = 1;
      blink_function();
      option_row++;
      if(option_row > 4)
        option_row = 1;
      while(((controller_input_a >> 1) & 1) || ((controller_input_b >> 1) & 1))
      {
    		// Wait here until key is released
    		quicksleep(10);
    	}
    }

    if (((controller_input_a >> 2) & 1)			                                      // if player left UP is pressed
  	|| ((controller_input_b >> 2) & 1))	                                          // if player right UP is pressed
    {
    	if(sound_on == 1)
				play_sound(200, 50, 1);
      blink_off_or_on = 1;
      blink_function();
      option_row--;
      if(option_row < 1)
        option_row = 4;
      while(((controller_input_a >> 2) & 1) || ((controller_input_b >> 2) & 1))
      {
    		// Wait here until key is released
    		quicksleep(10);
    	}
    }

    if (((controller_input_a >> 5) & 1)			                                      // if player left B is pressed
  	|| ((controller_input_b >> 5) & 1))			                                      // if player right B is pressed
    {
      screen_clear();
      start_menu();
      in_options = 0;
      in_menu = 1;
      while(in_menu)
      {
    		check_buttons();
        select_option();
      }
    }
  }
}


// draws an arrow on the screen and turns it off on the location it was previously
void menu_select(uint8_t row1, uint8_t row2, uint8_t col1, uint8_t col2)
{
  uint8_t k;

  // points arrow to here
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(row1);
  spi_send_recv(0x0);
  spi_send_recv(0x21);
  spi_send_recv(col1);
  spi_send_recv(col1 + 8);
  DISPLAY_CHANGE_TO_DATA_MODE;
  for(k = 0; k < 8; k++)
    spi_send_recv(font[(1*8 + k)]);

  // turns off arrow here
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(row1);
  spi_send_recv(0x0);
  spi_send_recv(0x21);
  spi_send_recv(col2);
  spi_send_recv(col2 + 8);
  DISPLAY_CHANGE_TO_DATA_MODE;
  for(k = 0; k < 8; k++)
    spi_send_recv(font[(0 + k)]);

  // turns off arrow here
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(row2);
  spi_send_recv(0x0);
  spi_send_recv(0x21);
  spi_send_recv(col1);
  spi_send_recv(col1 + 8);
  DISPLAY_CHANGE_TO_DATA_MODE;
  for(k = 0; k < 8; k++)
    spi_send_recv(font[(0 + k)]);

  // turns off arrow here
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(row2);
  spi_send_recv(0x0);
  spi_send_recv(0x21);
  spi_send_recv(col2);
  spi_send_recv(col2 + 8);
  DISPLAY_CHANGE_TO_DATA_MODE;
  for(k = 0; k < 8; k++)
    spi_send_recv(font[(0 + k)]);
}


// enter a mode depending what option is selected in the main menu
void select_option(void)
{
  if (((controller_input_a >> 6) & 1)			             // if player left A is pressed
    || ((controller_input_b >> 6) & 1))			           // if player right A is pressed
  {
  	if(selected_option == 0)                           // if arrow points to 1-player mode
    {
      screen_clear();
      game_countdown();
      screen_clear();
      // Initiation
    	in_game = 1;
    	playing_reset(0);

    	// Get stuck here while in game
      while(in_game)
      {
        quicksleep(10);
      }

      // Get stuck here while quitting (so the game doesn't start again immediately)
  		while ((controller_input_a >> 6) & 1)			  // if player left A is pressed
      {
        quicksleep(10);
      }

      // Clear screen after game is done
      screen_clear();

      // Reload start menu
      start_menu();
    }

    if(selected_option == 1)                           // if arrow points to 2-player mode
    {
      screen_clear();
      game_countdown();
      screen_clear();

      // Initiation
    	in_game = 2;
    	playing_reset(0);

    	// Get stuck here while in game
      while(in_game)
      {
        quicksleep(10);
      }

      // Get stuck here while quitting (so the game doesn't start again immediately)
  		while (((controller_input_a >> 6) & 1)			  // if player left A is pressed
  		|| ((controller_input_b >> 6) & 1))			    	// if player right A is pressed
      {
        quicksleep(10);
      }

      // Clear screen after game is done
      screen_clear();

      // Reload start menu
      start_menu();
    }

    if(selected_option == 2)                           // if arrow points to options
    {
      screen_clear();
      in_options = 1;
      options_menu();
      while(1)
        quicksleep(10);
    }

    if(selected_option == 3)                          // if arrow points to credits
    {
      screen_clear();
      in_credits = 1;
      vertical_scrolling_credits();
      while(1)
        quicksleep(10);
    }
  }
}


// checks when select is pressed and released and moves an arrow around between the different options
void check_buttons(void)
{
  if (((controller_input_a >> 4) & 1)			          // if player left SELECT is pressed
  	|| ((controller_input_b >> 4) & 1))			        // if player right SELECT is pressed
  {
    button_press = 1;
    while(button_press == 1)
    {
      if ((!(controller_input_a >> 4) & 1)					// if player left SELECT is released
  			&& (!(controller_input_b >> 4) & 1))				// if player right SELECT is released
      {
      	if(sound_on == 1)
					play_sound(200, 50, 1);
        selected_option++;
        if(selected_option > 3)
          selected_option = 0;
        button_press = 0;
      }
    }
  }

  if(selected_option == 0)
    menu_select(2, 3, 0, 62);   // 1P
  if(selected_option == 1)
    menu_select(2, 3, 62, 0);   // 2P
  if(selected_option == 2)
    menu_select(3, 2, 0, 62);   // OPT
  if(selected_option == 3)
    menu_select(3, 2, 62, 0);   // CRED
}
