#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"


char made_by[] = "MADE BY";
char marten[] = "MARTEN";
char jens[] = "AND JENS";
char thank_you[] = "THANK YOU";
char for_playing[] = "FOR PLAYING";
char the_the[] = "THE";
char end_end[] = "END";
uint8_t credits_page = 0;
uint8_t in_credits = 0;


// writes the text outside the defined page addresses
// replaced every time the time the timer icrements credits_page by one
void credits_function(void)
{
  if(credits_page == 0)
  {
    screen_clear();
    menu_layout(marten, sizeof(marten), 4, 3);
    menu_layout(jens, sizeof(jens), 7, 3);
  }
  if(credits_page == 1)
  {
    screen_clear();
    menu_layout(thank_you, sizeof(thank_you), 4, 3);
    menu_layout(for_playing, sizeof(for_playing), 7, 3);
  }
  if(credits_page > 1)
  {
    screen_clear();
    menu_layout(the_the, sizeof(the_the), 4, 3);
    menu_layout(end_end, sizeof(end_end), 6, 3);
  }

  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x2E);              // This command stops the motion of scrolling
  spi_send_recv(0x2A);              // 29h, X1X0 = 01b : Vertical and Right Horizontal Scroll
  spi_send_recv(0x00);              // A[7:0] : Dummy byte
  spi_send_recv(0x00);              // B[2:0] : Define start page address
  spi_send_recv(0b110);             // C[2:0] : Set time interval between each scroll step in terms of frame frequency
  spi_send_recv(0x00);              // D[2:0] : Define end page address
  spi_send_recv(01);                // E[5:0] : Vertical scrolling offset
  spi_send_recv(0x2F);              // Activate scrolling

  credits_page++;
}


// define start and end page address to zero to achieve vertical scrolling
void vertical_scrolling_credits(void)
{
  credits_counter = 0;
  menu_layout(menu_gametitle, 16, 4, 3);
  menu_layout(made_by, sizeof(made_by), 7, 3);

  DISPLAY_CHANGE_TO_COMMAND_MODE;
  spi_send_recv(0x2E);              // This command stops the motion of scrolling
  spi_send_recv(0x2A);              // 29h, X1X0 = 01b : Vertical and Right Horizontal Scroll
  spi_send_recv(0x00);              // A[7:0] : Dummy byte
  spi_send_recv(0x00);              // B[2:0] : Define start page address
  spi_send_recv(0b110);             // C[2:0] : Set time interval between each scroll step in terms of frame frequency
  spi_send_recv(0x00);              // D[2:0] : Define end page address
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
      credits_page = 0;
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
