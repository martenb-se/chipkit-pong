#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"

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
