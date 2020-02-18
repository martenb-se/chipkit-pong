#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)			// OLED data(1)/command(0) select 	- 0000 0000 0001 0000 (RF4 - PIN 39)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)					// OLED data(1)/command(0) select   -

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)							// OLED reset 											- 0000 0010 0000 0000 (RG9 - PIN 10)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)								// OLED no reset 										- 0000 0010 0000 0000 (RG9 - PIN 10)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)								// OLED VDD enable 									- 0000 0000 0100 0000 (RF6 - PIN 38)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)								// OLED VBAT enable 								- 0000 0000 0010 0000 (RF5 - PIN 40)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)								// OLED VDD disable 								- 0000 0000 0100 0000 (RF6 - PIN 38)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)								// OLED VBAT disable 								- 0000 0000 0010 0000 (RF5 - PIN 40)

void display_image(int x, const uint8_t *data);
void display_init(void);
void display_string(int line, char *s);
void display_update(void);
uint8_t spi_send_recv(uint8_t data);
void quicksleep(int cyc);

/* Declare bitmap array containing font */
extern const uint8_t const font[128*8];
/* Declare text buffer for display output */
extern char textbuffer[4][16];

void screen_clear(void);
void pixelbuffer_clear(void);
void screen_xy(int x, int y);

extern uint8_t pixelbuffer[4][127];