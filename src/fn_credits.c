#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"


char arr1[] = "MADE BY";
char marten[] = "MARTEN";
char jens[] = "AND JENS";
uint8_t in_credits;

void write_text_centered(char arr[], uint8_t row, uint8_t len)
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

void vertical_scrolling_credits(void)
{
    write_text_centered(menu_gametitle, 4, 16);
    write_text_centered(arr1, 5, sizeof(arr1));
    write_text_centered(marten, 6, sizeof(marten));
    write_text_centered(jens, 7, sizeof(jens));

    DISPLAY_CHANGE_TO_COMMAND_MODE;
    spi_send_recv(0x2E);              // This command stops the motion of scrolling
    spi_send_recv(0x2A);              // 29h, X1X0 = 01b : Vertical and Right Horizontal Scroll
    spi_send_recv(0x00);              // A[7:0] : Dummy byte
    spi_send_recv(0x00);              // B[2:0] : Define start page address
    spi_send_recv(0b001);             // C[2:0] : Set time interval between each scroll step in terms of frame frequency
    spi_send_recv(0x03);              // D[2:0] : Define end page address
    spi_send_recv(01);                // E[5:0] : Vertical scrolling offset
    spi_send_recv(0x2F);              // Activate scrolling

    while(in_credits)
    {
      if (((controller_input_a >> 5) & 1)			                                      // if player left B is pressed
    	|| ((controller_input_b >> 5) & 1))			                                      // if player right B is pressed
      {
        spi_send_recv(0x2E);                        // This command stops the motion of scrolling
        screen_clear();
        start_menu();
        in_credits = 0;
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
  */

/*
// Sample code
29h // Vertical and right horizontal scroll
00h // Dummy byte
00h // Define PAGE0 as start page adress
00h // Set time interval between each scroll step as 6 frames
07h // Define PAGE7 as end page adress
01h // Set vertical scrolling offset as 1 row
2Fh // Activate scrolling
*/
