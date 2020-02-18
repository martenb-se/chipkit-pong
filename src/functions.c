#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"

void display_init(void) {
  DISPLAY_CHANGE_TO_COMMAND_MODE;
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD;
	quicksleep(1000000);
	
	spi_send_recv(0xAE);
	DISPLAY_ACTIVATE_RESET;
	quicksleep(10);
	DISPLAY_DO_NOT_RESET;
	quicksleep(10);
	
	spi_send_recv(0x8D);
	spi_send_recv(0x14);
	
	spi_send_recv(0xD9);
	spi_send_recv(0xF1);
	
	DISPLAY_ACTIVATE_VBAT;
	quicksleep(10000000);
	
	spi_send_recv(0xA1);
	spi_send_recv(0xC8);
	
	spi_send_recv(0xDA);
	spi_send_recv(0x20);
	
	spi_send_recv(0xAF);
}

void screen_clear(void) {
	int i;
	int j = 0;

	// Clear screen
	for(j;j<4;j++) {
		
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22); 
		spi_send_recv(j);	// row 0, 1, 2, 3
		spi_send_recv(0x0);
		spi_send_recv(0x10);
		DISPLAY_CHANGE_TO_DATA_MODE;
		
		i = 0;
			
		for(;i<128;i++) {
			spi_send_recv(0);
		}
	
	}	
}

void pixelbuffer_clear(void) {	
	int i;
	int j;
	
	for(j = 0; j < 4; j++) {
		
		for(i = 0; i < 128; i++) {
			pixelbuffer[j][i] = 0;
			
		}
		
	}
	
}

// Translate X Y to page and colum
void screen_xy(int x, int y) {

	pixelbuffer[y/8][x] = pixelbuffer[y/8][x] | (1 << y%8);
	
}

uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));									// 0000 0000 0000 1000 = SPITBE (while SPITBE == 0)
																						  // Waiting until transmitter is ready
	SPI2BUF = data;
	while(!(SPI2STAT & 1));											// 0000 0000 0000 0001 = SPIRBF (while SPITBF == 0)
																					    // Waiting until byte is recieved
	return SPI2BUF;
}

void display_string(int line, char *s) {
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

void screen_xy_update() {	
	int i;
	int j;
	
	for(j = 0; j < 4; j++) {
		
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(j);	// row 0, 1, 2, 3
		spi_send_recv(0x0);
		spi_send_recv(0x10);
		DISPLAY_CHANGE_TO_DATA_MODE;
		
		for(i = 0; i < 128; i++) {
			spi_send_recv(pixelbuffer[j][i]);
			
		}
		
	}
	
}

void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

void user_isr(void) {
	
}