#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"

char gametitle[] = "PONGYANG EXTREME";
char oneplayer[] = "1P";
char twoplayer[] = "2P";
char options[] = "OPT";
char paddlesize[] = "PAD SIZE:";
char ballspeed[] = "BALL SPEED:";
char ballsize[] = "BALL SIZE:";
char medium[] = "MED";
char fast[] = "FAST";
char twobytwo[] = "2x2";
char countdown[] = "321GO!";
int option;
int button;


void options_menu_one(char arr[], int row, int len)
{
  int i, j, k, l, c;
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

void options_menu_two(char arr1[], char arr2[], int row, int len1, int len2)
{
  int i, j, k, l, c;
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

void game_countdown(void)
{
  int j, k, c;

  for(j = 0; j < 3; j++)
  {
    DISPLAY_CHANGE_TO_COMMAND_MODE;
    spi_send_recv(0x22);
    spi_send_recv(1);
    spi_send_recv(0x0);
    spi_send_recv(0x21);
    spi_send_recv(59);
    spi_send_recv(67);
    DISPLAY_CHANGE_TO_DATA_MODE;
    c = countdown[j];
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
    spi_send_recv(51);
    spi_send_recv(75);
    DISPLAY_CHANGE_TO_DATA_MODE;
    for(j = 3; j < 6; j++)
    {
      c = countdown[j];
      for(k = 0; k < 8; k++)
        spi_send_recv(font[(c*8 + k)]);
    }
    quicksleep(3000000);
  }
}

void start_menu(void)
{
  options_menu_one(gametitle, 0, sizeof(gametitle));
  options_menu_two(oneplayer, twoplayer, 2, sizeof(oneplayer), sizeof(twoplayer));
  options_menu_one(options, 3, sizeof(options));
}


void options_menu()
{
  options_menu_two(paddlesize, medium, 0, sizeof(paddlesize), sizeof(medium));
  options_menu_two(ballsize, twobytwo, 1, sizeof(ballsize), sizeof(twobytwo));
  options_menu_two(ballspeed, fast, 3, sizeof(ballspeed), sizeof(fast));

  while(1)
  {
    if ((input_get_buttons() >> 1) & 1)         // if button 2 is pressed
    {
      screen_clear();
      while(1)
      {
        start_menu();
    		select_option();
    		check_buttons();
      }
    }
  }
}

void menu_select_1p(void)
{
  int k;

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
  int k;

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
  int k;

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

void select_option(void)
{
  if ((input_get_buttons() >> 2) & 1)          // if button 3 is pressed
  {
    if(option == 1)                           // if arrow points to 2-player mode
    {
      screen_clear();
      game_countdown();
      screen_clear();
      // Initiation
    	// - In game
    	frame_init(); // Enable timer for frames
    	playing_field_init(); // Initiate playing field
    	// -- Scoreboard
    	display_left_score_update();
    	display_right_score_update();

    	// Enable interrupts
    	enable_interrupt();
      while(1)
      {
        quicksleep(10);
      }
    }

    if(option == 2)                           // if arrow points to options
    {
      screen_clear();
      options_menu();
      check_buttons();
    }
  }
}

void check_buttons(void)
{
  if ((input_get_buttons() >> 3) & 1)        // if button 4 is pressed
  {
    button = 1;
    while(button == 1)
    {
      if ((!(input_get_buttons() >> 3) & 1))   // if button 4 is released
      {
        option++;
        if(option > 2)
          option = 0;
        button = 0;
      }
    }
  }

  if(option == 0)
    menu_select_1p();
  if(option == 1)
    menu_select_2p();
  if(option == 2)
    menu_select_opt();
}


/*
void start_menu(void)
{
  int j, k, c;

  // gametitle label
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(0);
  spi_send_recv(0x0);
  spi_send_recv(0x10);
  DISPLAY_CHANGE_TO_DATA_MODE;
  for(j = 0; j < sizeof(gametitle); j++)
  {
		c = gametitle[j];
		for(k = 0; k < 8; k++)
      spi_send_recv(font[(c*8 + k)]);
	}

  // oneplayer label
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(2);
  spi_send_recv(0x0);
  spi_send_recv(0x21);
  spi_send_recv(32);
  spi_send_recv(48);
  DISPLAY_CHANGE_TO_DATA_MODE;
  for(j = 0; j < sizeof(oneplayer); j++)
  {
		c = oneplayer[j];
		for(k = 0; k < 8; k++)
      spi_send_recv(font[(c*8 + k)]);
	}

  // twoplayer label
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(2);
  spi_send_recv(0x0);
  spi_send_recv(0x21);
  spi_send_recv(80);
  spi_send_recv(96);
  DISPLAY_CHANGE_TO_DATA_MODE;
  for(j = 0; j < sizeof(twoplayer); j++)
  {
		c = twoplayer[j];
		for(k = 0; k < 8; k++)
      spi_send_recv(font[(c*8 + k)]);
	}

  // options label
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(3);
  spi_send_recv(0x0);
  spi_send_recv(0x21);
  spi_send_recv(52);
  spi_send_recv(76);
  DISPLAY_CHANGE_TO_DATA_MODE;
  for(j = 0; j < sizeof(options); j++)
  {
		c = options[j];
		for(k = 0; k < 8; k++)
      spi_send_recv(font[(c*8 + k)]);
	}
}
*/


/*
void check_buttons(void)
{
  if ((input_get_buttons() >> 3) & 1)        // if button 4 is pressed
  {
    button = 1;
    while(button == 1)
    {
      if ((!(input_get_buttons() >> 3) & 1))   // if button 4 is released
      {
        option++;
        if(option > 2)
          option = 0;
        button = 0;
      }
    }
  }

  if(option == 0)
    menu_select_1p();
  if(option == 1)
    menu_select_2p();
  if(option == 2)
    menu_select_opt();
}

void select_option(void)
{
  if ((input_get_buttons() >> 2) & 1)          // if button 3 is pressed
  {
    if(option == 1)
    {
      screen_clear();
      frame_init(); // Enable timer for frames
    	playing_field_init(); // Initiate playing field
    	// -- Scoreboard
    	display_left_score_update();
    	display_right_score_update();

    	// Enable interrupts
    	enable_interrupt();

    	while(1)
    	{
    		// Do nothing
    		quicksleep(10);
    	}
    }

  }
}



  /*
  if ((input_get_buttons() >> 2) & 1)          // if button 3 is pressed
  {
    button = 1;
    while(button == 1)
    {
      if ((!(input_get_buttons() >> 2) & 1))   // if button 3 is released
      {
        if(options == 1)
        {
          // Initiation
          // - In game
          frame_init(); // Enable timer for frames
          playing_field_init(); // Initiate playing field
          // -- Scoreboard
          display_left_score_update();
          display_right_score_update();

          // Enable interrupts
          enable_interrupt();
        }
        button = 0;
      }
    }

  }
*/




/*
8 8 8 8 8 62 28 8


00000000
00000000
00000100
00000110
11111111
00000110
00000100
00000000
*/
