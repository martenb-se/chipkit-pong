#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"


uint8_t selected_option;
uint8_t button_press;
uint8_t in_menu;
uint8_t in_options;
uint8_t option_row;
// uint8_t option_loop;


void menu_layout(char arr[], uint8_t len, uint8_t row, uint8_t sect)
{
  uint8_t j, k, l, c;
  l = (64 - len*8) / 2;
  if(sect == 0)
    l = 0;
  if(sect == 2)
    l = l + 64;

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

/*

void options_menu_one(char arr[], uint8_t row, uint8_t len)
{
  uint8_t j, k, l, c;
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

void options_menu_two(char arr1[], char arr2[], uint8_t row, uint8_t len1, uint8_t len2)
{
  uint8_t j, k, l, c;
  l = (128 - (len1*8 + len2*8)) / 3;

  // print array 1
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(row);
  spi_send_recv(0x0);
  spi_send_recv(0x21);
  spi_send_recv(l);
  spi_send_recv(l + len1*8);
  DISPLAY_CHANGE_TO_DATA_MODE;
  for(j = 0; j < len1; j++)
  {
    c = arr1[j];
    for(k = 0; k < 8; k++)
      spi_send_recv(font[(c*8 + k)]);
  }

  // print array 2
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(row);
  spi_send_recv(0x0);
  spi_send_recv(0x21);
  spi_send_recv(l + len1*8 + l);
  spi_send_recv(l + len1*8 + l + len2*8);
  DISPLAY_CHANGE_TO_DATA_MODE;
  for(j = 0; j < len2; j++)
  {
    c = arr2[j];
    for(k = 0; k < 8; k++)
      spi_send_recv(font[(c*8 + k)]);
  }
}

*/

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
  char menu_gametitle[] = "PONGYANG EXTREME";
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
  quicksleep(1000000);
}

/*
void blink_select(uint8_t row, uint8_t len1, uint8_t len2)
{
  uint8_t i, k, l;
  l = (128 - (len1*8 + len2*8)) / 3;

  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(row);
  spi_send_recv(0x0);
  spi_send_recv(0x21);
  spi_send_recv(l + len1*8);
  spi_send_recv(127);
  DISPLAY_CHANGE_TO_DATA_MODE;
  for(i = 0; i < (127 - (l + len1*8)); i++)
  {
    for(k = 0; k < 8; k++)
      spi_send_recv(font[(0 + k)]);
  }
  quicksleep(2000000);
}
*/

void options_menu(void)
{
  char opt_pad_size[] = "PAD SZ:";
  char opt_pad_size_small[] = "SMALL";
  char opt_pad_size_medium[] = "MED";
  char opt_pad_size_large[] = "LARGE";
  char opt_ball_speed[] = "B SPD:";
  char opt_ball_speed_slow[] = "SLOW";
  char opt_ball_speed_fast[] = "FAST";
  char opt_ball_size[] = "B SIZE:";
  char opt_ball_size_2x2[] = "2x2";
  char opt_ball_size_4x4[] = "4x4";
  char opt_ball_size_6x6[] = "6x6";
  char opt_game_difficulty[] = "DIFF:";
  char opt_diff_asy[] = "EASY";
  char opt_diff_normal[] = "NORM";
  char opt_diff_hard[] = "HARD";
  char opt_diff_godmode[] = "GODMODE";
  uint8_t cur_option;

  menu_layout(opt_pad_size, sizeof(opt_pad_size), 0, 1);
  menu_layout(opt_pad_size_medium, sizeof(opt_pad_size_medium), 0, 2);
  menu_layout(opt_ball_size, sizeof(opt_ball_size), 1, 1);
  menu_layout(opt_ball_size_2x2, sizeof(opt_ball_size_2x2), 1, 2);
  menu_layout(opt_ball_speed, sizeof(opt_ball_speed), 2, 1);
  menu_layout(opt_ball_speed_fast, sizeof(opt_ball_speed_fast), 2, 2);
  menu_layout(opt_game_difficulty, sizeof(opt_game_difficulty), 3, 1);
  menu_layout(opt_diff_normal, sizeof(opt_diff_normal), 3, 2);

  while(in_options)
  {
  	if(option_row == 0 && cur_option == 0)
    {
      blink_selected(0, sizeof(opt_pad_size_large));
      menu_layout(opt_pad_size_medium, sizeof(opt_pad_size_medium), 0, 2);
    	quicksleep(1000000);
  	}
    else if(option_row == 0 && cur_option == 1)
    {
      blink_selected(0, sizeof(opt_pad_size_large));
  		menu_layout(opt_pad_size_large, sizeof(opt_pad_size_large), 0, 2);
	    quicksleep(1000000);
    }
    else if(option_row == 0 && cur_option == 2)
    {
      blink_selected(0, sizeof(opt_pad_size_large));
		  menu_layout(opt_pad_size_small, sizeof(opt_pad_size_small), 0, 2);
		  quicksleep(1000000);
    }
    else if(option_row == 1 && cur_option == 0)
    {
      blink_selected(1, sizeof(opt_ball_size_2x2));
      menu_layout(opt_ball_size_2x2, sizeof(opt_ball_size_2x2), 1, 2);
      quicksleep(1000000);
    }
    else if(option_row == 1 && cur_option == 1)
    {
      blink_selected(1, sizeof(opt_ball_size_4x4));
      menu_layout(opt_ball_size_4x4, sizeof(opt_ball_size_4x4), 1, 2);
      quicksleep(1000000);
    }
    else if(option_row == 1 && cur_option == 2)
    {
      blink_selected(1, sizeof(opt_ball_size_6x6));
      menu_layout(opt_ball_size_6x6, sizeof(opt_ball_size_6x6), 1, 2);
      quicksleep(1000000);
    }
    else if(option_row == 2 && cur_option == 0)
    {
      blink_selected(2, sizeof(opt_diff_normal));
      menu_layout(opt_diff_normal, sizeof(opt_diff_normal), 2, 2);
      quicksleep(1000000);
    }
    else if(option_row == 2 && cur_option == 1)
    {
      blink_selected(2, sizeof(opt_ball_speed_fast));
      menu_layout(opt_ball_speed_fast, sizeof(opt_ball_speed_fast), 2, 2);
      quicksleep(1000000);
    }
    else if(option_row == 2 && cur_option == 2)
    {
      blink_selected(2, sizeof(opt_ball_speed_slow));
      menu_layout(opt_ball_speed_slow, sizeof(opt_ball_speed_slow), 2, 2);
      quicksleep(1000000);
    }
    else if(option_row == 3 && cur_option == 0)
    {
      blink_selected(2, sizeof(opt_diff_normal));
      menu_layout(opt_diff_normal, sizeof(opt_diff_normal), 3, 2);
      quicksleep(1000000);
    }
    else if(option_row == 3 && cur_option == 1)
    {
      blink_selected(2, sizeof(opt_diff_hard));
      menu_layout(opt_diff_hard, sizeof(opt_diff_hard), 3, 2);
      quicksleep(1000000);
    }
    else if(option_row == 3 && cur_option == 2)
    {
      blink_selected(2, sizeof(opt_diff_asy));
      menu_layout(opt_diff_asy, sizeof(opt_diff_asy), 3, 2);
      quicksleep(1000000);
    }

    if (((controller_input_a >> 4) & 1)                                               // if SELECT is pressed
    || ((controller_input_b >> 4) & 1))			                                          // if SELECT is pressed
    {
    	cur_option++;
    	if(cur_option > 2)
    		cur_option = 0;
      /*while(((controller_input_a >> 4) & 1) || ((controller_input_b >> 4) & 1))
      {
    		// Wait here until key is released
    		quicksleep(10);
    	}*/
    }

    if (((controller_input_a >> 1) & 1)			     // if player left DOWN is pressed
  	|| ((controller_input_b >> 1) & 1))	         // if player right DOWN is pressed
    {
      option_row++;
      /*if(option_row > 3)
        option_row = 0;*/
      cur_option = 0;
      /*while(((controller_input_a >> 1) & 1) || ((controller_input_b >> 1) & 1))
      {
    		// Wait here until key is released
    		quicksleep(10);
    	}*/
    }

    if (((controller_input_a >> 2) & 1)			     // if player left UP is pressed
  	|| ((controller_input_b >> 2) & 1))	         // if player right UP is pressed
    {
      option_row--;
      /*if(option_row < 0)
        option_row = 3;*/
      cur_option = 0;
      /*while(((controller_input_a >> 2) & 1) || ((controller_input_b >> 2) & 1))
      {
    		// Wait here until key is released
    		quicksleep(10);
    	}*/
    }

    if (((controller_input_a >> 5) & 1)			     // if player left B is pressed
  	|| ((controller_input_b >> 5) & 1))			     // if player right B is pressed
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

/*
  option_row = 0;
  in_options = 1;

  while(option_row == 0)
  {
    option_loop = 0;
    while(option_loop == 0)                                                             // while pad size is selected
    {
      options_menu_two(opt_pad_size, opt_pad_size_medium, 0, sizeof(opt_pad_size), sizeof(opt_pad_size_medium));
      quicksleep(2000000);
      blink_select(0, sizeof(opt_pad_size), sizeof(opt_pad_size_medium));                    // first option blinking (opt_pad_size_medium)
      if (((controller_input_a >> 4) & 1)                                               // if SELECT is pressed
      || ((controller_input_b >> 4) & 1))			                                          // if SELECT is pressed
      {
        while(option_loop == 0)
        {
          options_menu_two(opt_pad_size, opt_pad_size_large, 0, sizeof(opt_pad_size), sizeof(opt_pad_size_large));
          quicksleep(2000000);
          blink_select(0, sizeof(opt_pad_size), sizeof(opt_pad_size_large));
          if (((controller_input_a >> 4) & 1)
          || ((controller_input_b >> 4) & 1))
          {
            while(option_loop == 0)
            {
              options_menu_two(opt_pad_size, opt_pad_size_small, 0, sizeof(opt_pad_size), sizeof(opt_pad_size_small));
              quicksleep(2000000);
              blink_select(0, sizeof(opt_pad_size), sizeof(opt_pad_size_small));
              if (((controller_input_a >> 4) & 1)
              || ((controller_input_b >> 4) & 1))
              {
                option_loop = 1;
              }
            }
          }
        }
      }
    }
  }


    while(option_row == 1)
    {
      option_loop = 0;
      while(option_loop == 0)
      {
        options_menu_two(ball_size, ball_size_2x2, 1, sizeof(ball_size), sizeof(ball_size_2x2));
        quicksleep(2000000);
        blink_select(1, sizeof(ball_size), sizeof(ball_size_2x2));
        if (((controller_input_a >> 4) & 1)
        || ((controller_input_b >> 4) & 1))
        {
          while(option_loop == 0)
          {
            options_menu_two(ball_size, ball_size_4x4, 1, sizeof(ball_size), sizeof(ball_size_4x4));
            quicksleep(2000000);
            blink_select(1, sizeof(ball_size), sizeof(ball_size_4x4));
            if (((controller_input_a >> 4) & 1)
            || ((controller_input_b >> 4) & 1))
            {
              while(option_loop == 0)
              {
                options_menu_two(ball_size, ball_size_6x6, 1, sizeof(ball_size), sizeof(ball_size_6x6));
                quicksleep(2000000);
                blink_select(1, sizeof(ball_size), sizeof(ball_size_6x6));
                if (((controller_input_a >> 4) & 1)
                || ((controller_input_b >> 4) & 1))
                {
                  option_loop = 1;
                }
              }
            }
          }
        }
      }
    }

    while(option_row == 2)
    {
      option_loop = 0;
      while(option_loop == 0)
      {
        options_menu_two(opt_ball_speed, opt_diff_normal, 1, sizeof(opt_ball_speed), sizeof(opt_diff_normal));
        quicksleep(2000000);
        blink_select(1, sizeof(opt_ball_speed), sizeof(opt_diff_normal));
        if (((controller_input_a >> 4) & 1)
        || ((controller_input_b >> 4) & 1))
        {
          while(option_loop == 0)
          {
            options_menu_two(opt_ball_speed, opt_ball_speed_fast, 1, sizeof(opt_ball_speed), sizeof(opt_ball_speed_fast));
            quicksleep(2000000);
            blink_select(1, sizeof(opt_ball_speed), sizeof(opt_ball_speed_fast));
            if (((controller_input_a >> 4) & 1)
            || ((controller_input_b >> 4) & 1))
            {
              while(option_loop == 0)
              {
                options_menu_two(opt_ball_speed, opt_ball_speed_slow, 1, sizeof(opt_ball_speed), sizeof(opt_ball_speed_slow));
                quicksleep(2000000);
                blink_select(1, sizeof(opt_ball_speed), sizeof(opt_ball_speed_slow));
                if (((controller_input_a >> 4) & 1)
                || ((controller_input_b >> 4) & 1))
                {
                  option_loop = 1;
                }
              }
            }
          }
        }
      }
    }

    while(option_row == 3)
    {
      option_loop = 0;
      while(option_loop == 0)
      {
        options_menu_two(opt_game_difficulty, opt_diff_normal, 1, sizeof(opt_game_difficulty), sizeof(opt_diff_normal));
        quicksleep(2000000);
        blink_select(1, sizeof(opt_game_difficulty), sizeof(opt_diff_normal));
        if (((controller_input_a >> 4) & 1)
        || ((controller_input_b >> 4) & 1))
        {
          while(option_loop == 0)
          {
            options_menu_two(opt_game_difficulty, opt_diff_hard, 1, sizeof(opt_game_difficulty), sizeof(opt_diff_hard));
            quicksleep(2000000);
            blink_select(1, sizeof(opt_game_difficulty), sizeof(opt_diff_hard));
            if (((controller_input_a >> 4) & 1)
            || ((controller_input_b >> 4) & 1))
            {
              while(option_loop == 0)
              {
                options_menu_two(opt_game_difficulty, opt_diff_godmode, 1, sizeof(opt_game_difficulty), sizeof(opt_diff_godmode));
                quicksleep(2000000);
                blink_select(1, sizeof(opt_game_difficulty), sizeof(opt_diff_godmode));
                if (((controller_input_a >> 4) & 1)
                || ((controller_input_b >> 4) & 1))
                {
                  while(option_loop == 0)
                  {
                    options_menu_two(opt_game_difficulty, opt_diff_asy, 1, sizeof(opt_game_difficulty), sizeof(opt_diff_asy));
                    quicksleep(2000000);
                    blink_select(1, sizeof(opt_game_difficulty), sizeof(opt_diff_asy));
                    if (((controller_input_a >> 4) & 1)
                    || ((controller_input_b >> 4) & 1))
                    {
                      option_loop = 1;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }


    if (((controller_input_a >> 5) & 1)			     // if player left B is pressed
  	|| ((controller_input_b >> 5) & 1))			     // if player right B is pressed
    {
      screen_clear();
      in_options = 0;
      in_menu = 1;
      while(in_menu)
      {
        start_menu();
    		select_option();
    		check_buttons();
      }
    }
}
*/

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

/*
void menu_arrow_select(uint8_t row, uint8_t col1, uint8_t col2, uint8_t col3)
{
  uint8_t i, j, k, c;

  row = prev_row;
  col = prev_col;



  static uint8_t count = 0;
  static uint8_t col = 16;
  uint8_t prev_col = 36;
  prev_row = 3;
  row = 2;
  c = 0;

  for(i = 0; i < 2; i++)
  {
    if(prev_row == 3)

    // points arrow to 1P
    DISPLAY_CHANGE_TO_COMMAND_MODE;
    spi_send_recv(0x22);
    spi_send_recv(row);
    spi_send_recv(0x0);
    spi_send_recv(0x21);
    spi_send_recv(col);
    spi_send_recv(col + 8);
    DISPLAY_CHANGE_TO_DATA_MODE;
    for(k = 0; k < 8; k++)
      spi_send_recv(font[(c*8 + k)]);


    count++;
    if(count == 1)
    {
      row = 3;
    }
    c = 0;

  }

  row = 2;
  col = col + 24;
  count++;


}

void menu_select_1p(void)
{
  uint8_t k;

  // points arrow to 1P
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(2);
  spi_send_recv(0x0);
  spi_send_recv(0x21);
  spi_send_recv(16);
  spi_send_recv(24);
  DISPLAY_CHANGE_TO_DATA_MODE;
  for(k = 0; k < 8; k++)
    spi_send_recv(font[(1*8 + k)]);

  // turns off arrow to 2P
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(2);
  spi_send_recv(0x0);
  spi_send_recv(0x21);
  spi_send_recv(64);
  spi_send_recv(72);
  DISPLAY_CHANGE_TO_DATA_MODE;
  for(k = 0; k < 8; k++)
    spi_send_recv(font[(0 + k)]);

  // turns off arrow to OPT
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(3);
  spi_send_recv(0x0);
  spi_send_recv(0x21);
  spi_send_recv(36);
  spi_send_recv(46);
  DISPLAY_CHANGE_TO_DATA_MODE;
  for(k = 0; k < 8; k++)
    spi_send_recv(font[(0 + k)]);
}

void menu_select_2p(void)
{
  uint8_t k;

  // points arrow to 2P
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(2);
  spi_send_recv(0x0);
  spi_send_recv(0x21);
  spi_send_recv(64);
  spi_send_recv(72);
  DISPLAY_CHANGE_TO_DATA_MODE;
  for(k = 0; k < 8; k++)
    spi_send_recv(font[(1*8 + k)]);

  // turns off arrow to P1
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(2);
  spi_send_recv(0x0);
  spi_send_recv(0x21);
  spi_send_recv(16);
  spi_send_recv(24);
  DISPLAY_CHANGE_TO_DATA_MODE;
  for(k = 0; k < 8; k++)
    spi_send_recv(font[(0 + k)]);

  // turns off arrow to OPT
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(3);
  spi_send_recv(0x0);
  spi_send_recv(0x21);
  spi_send_recv(36);
  spi_send_recv(46);
  DISPLAY_CHANGE_TO_DATA_MODE;
  for(k = 0; k < 8; k++)
    spi_send_recv(font[(0 + k)]);
}

void menu_select_opt(void)
{
  uint8_t k;

  // points arrow to OPT
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(3);
  spi_send_recv(0x0);
  spi_send_recv(0x21);
  spi_send_recv(36);
  spi_send_recv(46);
  DISPLAY_CHANGE_TO_DATA_MODE;
  for(k = 0; k < 8; k++)
    spi_send_recv(font[(1*8 + k)]);

  // turns off arrow to P1
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(2);
  spi_send_recv(0x0);
  spi_send_recv(0x21);
  spi_send_recv(16);
  spi_send_recv(24);
  DISPLAY_CHANGE_TO_DATA_MODE;
  for(k = 0; k < 8; k++)
    spi_send_recv(font[(0 + k)]);

  // turns off arrow to 2P
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(2);
  spi_send_recv(0x0);
  spi_send_recv(0x21);
  spi_send_recv(64);
  spi_send_recv(72);
  DISPLAY_CHANGE_TO_DATA_MODE;
  for(k = 0; k < 8; k++)
    spi_send_recv(font[(0 + k)]);
}
*/

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
      //check_buttons();          // probably not needed
    }

    if(selected_option == 3)
    {
      // launch rolling credits here
    }
  }
}


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
