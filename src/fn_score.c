#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"

void score_borders(void)
{
  int j = 0;
  for(j;j<4;j++)
  {
    DISPLAY_CHANGE_TO_COMMAND_MODE;

    // row 0-3
    spi_send_recv(0x22);
    spi_send_recv(j);
    spi_send_recv(0x0);

      // left border
    spi_send_recv(0x21);
    spi_send_recv(0x0);
    spi_send_recv(0x15);

    DISPLAY_CHANGE_TO_DATA_MODE;
    int i;
    for(i=0;i<16;i++)
    {
      spi_send_recv(0xff);
    }

    DISPLAY_CHANGE_TO_COMMAND_MODE;

    // row 0-3
    spi_send_recv(0x22);
    spi_send_recv(j);
    spi_send_recv(0x0);

    // right border
    spi_send_recv(0x21);
    spi_send_recv(112);
    spi_send_recv(127);

    DISPLAY_CHANGE_TO_DATA_MODE;
    for(i=0;i<16;i++)
    {
      spi_send_recv(0xff);
    }
  }

  void display_score(int line, char *s, char *t) {
  	int i;
  	if(line < 0 || line >= 4)
  		return;
  	if(!s)
  		return;

  	for(i = 0; i < 16; i++)
  		if(*s) {
  			textbuffer[line][i] = *s;
  			s++;
  		} else
  			textbuffer[line][i] = ' ';
  }

  void display_update(void) {
  	int i, j, k;
  	int c;
  	for(i = 0; i < 4; i++) {
  		DISPLAY_CHANGE_TO_COMMAND_MODE;
  		spi_send_recv(0x22);
  		spi_send_recv(i);

  		spi_send_recv(0x0);
  		spi_send_recv(0x10);

  		DISPLAY_CHANGE_TO_DATA_MODE;

  		for(j = 0; j < 16; j++) {
  			c = textbuffer[i][j];
  			if(c & 0x80)
  				continue;

  			for(k = 0; k < 8; k++)
  				spi_send_recv(font[c*8 + k]);
  		}
  	}
  }






}
