#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"

char leftscorebuffer[4];
char rightscorebuffer[4];
int sc1 = 3;
int sc2 = 0;

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
      quicksleep(1000000/3);
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
      quicksleep(1000000/3);
      spi_send_recv(0xff);
    }
  }
}

void display_left_score(int *s)
{
	int i;
	for(i = 0; i < 4; i++)
  {
			leftscorebuffer[i] = *s;
			s++;
  }
}

void display_right_score(int *s)
{
	int i;
	for(i = 0; i < 4; i++)
  {
			rightscorebuffer[i] = *s;
			s++;
  }
}

void display_left_score_update(void)
{
	int i, j, k;
	int c;
	DISPLAY_CHANGE_TO_COMMAND_MODE;
	spi_send_recv(0x22);
	spi_send_recv(1);
  spi_send_recv(0x0);
  spi_send_recv(0x10);

	DISPLAY_CHANGE_TO_DATA_MODE;

	for(j = 0; j < 1; j++)
  {
		c = leftscorebuffer[j];
		if(c & 0x80)
			continue;

		for(k = 0; k < 8; k++)
    {
      quicksleep(1000000/3);
			spi_send_recv(font[(c+sc1)*8 + k]);
    }
	}
  sc1++;
  if(sc1 > 9)
    sc1 = 0;
}

void display_right_score_update(void)
{
	int i, j, k;
	int c;
	DISPLAY_CHANGE_TO_COMMAND_MODE;
	spi_send_recv(0x22);
	spi_send_recv(1);
  spi_send_recv(0x0);
  spi_send_recv(0x1f);

	DISPLAY_CHANGE_TO_DATA_MODE;

	for(j = 0; j < 1; j++)
  {
		c = rightscorebuffer[j];
		if(c & 0x80)
			continue;

		for(k = 0; k < 8; k++)
    {
      quicksleep(1000000/3);
			spi_send_recv(font[(c+sc2)*8 + k]);
    }
	}
  sc2++;
  if(sc2 > 9)
    sc2 = 0;
}
