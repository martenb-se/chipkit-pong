#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"

char leftscorebuffer[] = "00";
char rightscorebuffer[] = "00";
int sc1 = -1;
int sc2 = -1;

void display_left_score_update(void)
{
	int j, k, c;
  sc1++;

	DISPLAY_CHANGE_TO_COMMAND_MODE;
	spi_send_recv(0x22);
	spi_send_recv(0);
  spi_send_recv(0x0);
  spi_send_recv(0x10);

	DISPLAY_CHANGE_TO_DATA_MODE;
  for(j = 0; j < 2; j++)
  {
		c = leftscorebuffer[j];
    if(sc1 > 99)
      sc1 = 0;
		if(c & 0x80)
			continue;
		for(k = 0; k < 8; k++)
    {
      if(j == 0)
        spi_send_recv(font[((c+sc1/10))*8 + k]);
      else if(j == 1)
			   spi_send_recv(font[((c+sc1%10))*8 + k]);
    }
	}
}

void display_right_score_update(void)
{
	int j, k, c;
  sc2++;

	DISPLAY_CHANGE_TO_COMMAND_MODE;
	spi_send_recv(0x22);
	spi_send_recv(0);
  spi_send_recv(0x0);
  spi_send_recv(0x1f);

	DISPLAY_CHANGE_TO_DATA_MODE;
  for(j = 0; j < 2; j++)
  {
		c = rightscorebuffer[j];
    if(sc2 > 99)
      sc2 = 0;
		if(c & 0x80)
			continue;
		for(k = 0; k < 8; k++)
    {
      if(j == 0)
        spi_send_recv(font[((c+sc2/10))*8 + k]);
      else if(j == 1)
			  spi_send_recv(font[((c+sc2%10))*8 + k]);
    }
	}
}
