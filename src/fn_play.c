#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "project.h"

uint8_t playbuffer[4][96];
uint8_t player_left = 16;

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

void playbuffer_clear(void) {
	int i;
	int j;

	for(j = 0; j < 4; j++) {

		for(i = 0; i < 96; i++) {
			playbuffer[j][i] = 0;

		}

	}

}

void play_xy_update(void) {
	int i;
	int j;

	for(j = 0; j < 4; j++) {

		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(j);	// row 0, 1, 2, 3
		spi_send_recv(0x0);
		spi_send_recv(0x11);
		//spi_send_recv(0x21);
		//spi_send_recv(16);
		//spi_send_recv(111);
		DISPLAY_CHANGE_TO_DATA_MODE;

		for(i = 0; i < 96; i++) {
			spi_send_recv(playbuffer[j][i]);

		}

	}

}

// Translate X Y to page and colum
void play_xy(int x, int y) {

	playbuffer[y/8][x] = playbuffer[y/8][x] | (1 << y%8);

}

void playing_field_init(void) {
	int i;
	for(i=0;i<96;i++) {
		play_xy(i,0);
		play_xy(i,31);
	}
	play_xy_update();
}
