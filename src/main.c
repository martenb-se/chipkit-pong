#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"



int main()
{
        /*
	  This will set the peripheral bus clock to the same frequency
	  as the sysclock. That means 80 MHz, when the microcontroller
	  is running at 80 MHz. Changed 2017, as recommended by Axel.
	*/
	SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
	while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
	while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
	SYSKEY = 0x0;  /* Lock OSCCON */

	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;

	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;

	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);

	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
  SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;

	display_init();
	//display_image(96, icon);
	//display_string( 1, "AAAAAAAA" );
	//display_update();

	// Testing
	DISPLAY_CHANGE_TO_COMMAND_MODE;

																							// https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
	spi_send_recv(0x22);												// 0010 0010 // pg. 31 - Set Page Address
	spi_send_recv(0);														// Page start 0, 1, 2, 3 // 0000 0000

	//spi_send_recv(96 & 0xF);										// Page end // 0110 0000 & 0000 1111 = 0000 0000
	//spi_send_recv(0x10 | ((96 >> 4) & 0xF));		// Set Higher Column = 0110 (6) // 0001 0000 | ((0110 0000 >> 4) = 0000 0110) & 0000 1111 = 0001 0110

	spi_send_recv(0x0);
	spi_send_recv(0x10);

	DISPLAY_CHANGE_TO_DATA_MODE;

	/*spi_send_recv(0b10000000); // ~(255 = 1111 1111) = 0 (0000 0000)
	spi_send_recv(0b01000000);
	spi_send_recv(0b00100000);
	spi_send_recv(0b00010000);
	spi_send_recv(0b00001000);
	spi_send_recv(0b00000100);
	spi_send_recv(0b00000010);
	spi_send_recv(0b00000001);

	spi_send_recv(0b10000000);
	spi_send_recv(0b01000000);
	spi_send_recv(0b00100000);
	spi_send_recv(0b00010000);
	spi_send_recv(0b00001000);
	spi_send_recv(0b00000100);
	spi_send_recv(0b00000010);
	spi_send_recv(0b00000001);

	spi_send_recv(0b10000000);
	spi_send_recv(0b01000000);
	spi_send_recv(0b00100000);
	spi_send_recv(0b00010000);
	spi_send_recv(0b00001000);
	spi_send_recv(0b00000100);
	spi_send_recv(0b00000010);
	spi_send_recv(0b00000001);*/

	int i = 0;
	int j = 0;

	// Test XY
	screen_clear();
	playing_field_init();

	//for(i = 0; i < 31; i++)
	//	screen_xy(0, i);

	// Testing for input/output

	/*display_string(1, "Test");
	display_update();

	// Upper connectors
	// Set RF2 (0000 0000 0000 000X) to input
	TRISFSET = 1 << 2;

	// Set RF3 (0000 0000 0000 00X0) to output
	TRISFCLR = 1 << 3;

	// Lower
	// Set RE0 (0000 0000 0000 000X) to output
	TRISECLR = 1;*/


	int k;

	while(1) {
		/*for(j;j<4;j++) {

			DISPLAY_CHANGE_TO_COMMAND_MODE;
			spi_send_recv(0x22);
			spi_send_recv(j);	// row 0, 1, 2, 3
			spi_send_recv(0x0);
			spi_send_recv(0x10);
			DISPLAY_CHANGE_TO_DATA_MODE;

			i = 0;

			for(;i<128;i++) {
				spi_send_recv(~i & 0xFF);
				//quicksleep(100000);
			}

		}*/


		// Testing for input/output

		/*if ((PORTF >> 2) & 1) {
			display_string(1, "RF2");
			display_update();
		} else {
			display_string(1, "-----");
			display_update();
		}
		quicksleep(1000);*/


		//quicksleep(1000000);
		//PORTESET = 1;
		//quicksleep(1000000);
		//PORTECLR = 1;

	}
}

