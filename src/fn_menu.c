#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"

char gametitle[] = "PONGYANG EXTREME";
char oneplayer[] = "1P";
char twoplayer[] = "2P";
char options[] = "OPT";
char credits[] = "CRED";
char exit[] = "EXIT";


void start_menu(void)
{
  int i, j, k;
	int c;

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
    {
        spi_send_recv(font[(c*8 + k)]);
    }
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
    {
        spi_send_recv(font[(c*8 + k)]);
    }
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
    {
        spi_send_recv(font[(c*8 + k)]);
    }
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
    {
        spi_send_recv(font[(c*8 + k)]);
    }
	}
/*
  // exit label
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x22);
  spi_send_recv(3);
  spi_send_recv(0x0);
  spi_send_recv(0x21);
  spi_send_recv(48);
  spi_send_recv(80);


  DISPLAY_CHANGE_TO_DATA_MODE;
  for(j = 0; j < sizeof(exit); j++)
  {
		c = exit[j];
		if(c & 0x80)
			continue;
		for(k = 0; k < 8; k++)
    {
        spi_send_recv(font[(c*8 + k)]);
    }
	}
*/
}
