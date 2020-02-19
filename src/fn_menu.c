#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"

char gametitle[] = "PONGYANG EXTREME";
char oneplayer[] = "1P";
char twoplayer[] = "2P";
char options[] = "OPT";
char credits[] = "CRED";
char exit[] = "EXIT";
int option;
int button;


void start_menu(void)
{
  int i, j, k, c;

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
		if(c & 0x80)
			continue;
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
		if(c & 0x80)
			continue;
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
		if(c & 0x80)
			continue;
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
		if(c & 0x80)
			continue;
		for(k = 0; k < 8; k++)
      spi_send_recv(font[(c*8 + k)]);
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
