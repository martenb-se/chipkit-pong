#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)			// OLED data(1)/command(0) select 	- 0000 0000 0001 0000 (RF4 - PIN 39)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)					// OLED data(1)/command(0) select   -

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)							// OLED reset 											- 0000 0010 0000 0000 (RG9 - PIN 10)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)								// OLED no reset 										- 0000 0010 0000 0000 (RG9 - PIN 10)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)								// OLED VDD enable 									- 0000 0000 0100 0000 (RF6 - PIN 38)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)								// OLED VBAT enable 								- 0000 0000 0010 0000 (RF5 - PIN 40)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)								// OLED VDD disable 								- 0000 0000 0100 0000 (RF6 - PIN 38)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)								// OLED VBAT disable 								- 0000 0000 0010 0000 (RF5 - PIN 40)

#define PI 3.14159265

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

// Clear whole screen
void screen_clear(void);

// Input
void input_init(void);
int input_get_buttons(void);

// Frames
void frame_init(void);
void frame_update(void);

// Menu
void start_menu(void);
extern char gametitle[16];
extern char oneplayer[2];
extern char twoplayer[2];
extern char options[3];
extern char credits[4];
extern char exit[4];

// Scores
extern char leftscorebuffer[2];
extern char rightscorebuffer[2];
extern int sc1;
extern int sc2;
void display_left_score_update(void);
void display_right_score_update(void);

// Playing
extern uint8_t playbuffer[4][96];								// Buffer for playing field pixels
extern uint8_t player_left;											// Y-position for left player
extern uint8_t player_right;										// Y-position for left player
extern uint8_t player_ball[2];									// XY-position for the ball
extern uint8_t player_ball_x[96];								// X-position for the upcoming movement
extern uint8_t player_ball_y[96];								// Y-position for the upcoming movement
extern uint8_t player_ball_movement_pointer;
double player_ball_direction;										// Angle for ball movement
extern uint8_t player_ball_speed;								// Ball speed
extern uint8_t player_height;
extern uint8_t ball_width;
extern uint8_t ball_height;
void playbuffer_clear(void);										// Clear pixelbuffer for playing field
void playing_field_init(void);	 								// Initiate playing field with borders
void playing_field_update(void);								// Update playing field's pixelbuffer
void play_xy(int x, int y); 										// Draw x, y pixel on playing field
void move_player_left(uint8_t rel_y);						// Move left player relative Y-position
void move_player_right(uint8_t rel_y);					// Move right player relative Y-position
void move_ball();																// Move ball
void draw_players(void);												// Draw players on playing field
void draw_ball(void);														// Draw ball on playing field
void check_player_moves(void);									// Check input and move players
void init_ball(void);														// Initiate ball movement
void ball_collision_detection(void);						// Ball collision detection