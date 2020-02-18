#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"

void score_borders(void)
{
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

    // right border
    spi_send_recv(0x21);
    spi_send_recv(112);
    spi_send_recv(127);

    DISPLAY_CHANGE_TO_DATA_MODE;
  }


}
