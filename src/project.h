#define MATH_ERRNO = 0;
#define math_errhandling = 0;
//#define errno = 0;
extern int errno;

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

extern const uint8_t const font[128*8];
extern const uint8_t const numbers_mini[11*4];
extern char textbuffer[4][16];

// Clear whole screen
void screen_clear(void);

// Main
extern uint8_t in_game;

// Input
extern uint8_t controller_input_a;
extern uint8_t controller_input_b;
extern uint8_t controller_input_a_buffer;
extern uint8_t controller_input_b_buffer;
void input_init(void);
int input_get_buttons(void);

// Frames
void frame_init(void);
void frame_update(void);

// Menu
extern char gametitle[16];
extern char oneplayer[2];
extern char twoplayer[2];
extern char options[3];
extern char credits[4];
extern char exit[4];
extern char paddlesize[9];
extern char medium[3];
extern char large[5];
extern char small[5];
extern char ballspeed[11];
extern char fast[4];
extern char slow[4];
extern char ballsize[10];
extern char twobytwo[3];
extern char fourbyfour[3];
extern char sixbysix[3];
extern char countdown[6];
extern char difficulty[5];
extern char normal[6];
extern char hard[4];
extern char god[3];
extern char easy[4];
void start_menu(void);
void options_menu_one(char arr[], int row, int len);
void options_menu_two(char arr1[], char arr2[], int row, int len1, int len2);
void game_countdown(void);
void start_menu(void);
void options_menu();
void menu_select_1p(void);
void menu_select_2p(void);
void menu_select_opt(void);
void select_option(void);
void check_buttons(void);


// Scores
extern char leftscorebuffer[2];
extern char rightscorebuffer[2];
extern int sc1;
extern int sc2;
void display_left_score_update(void);
void display_right_score_update(void);

// Playing
extern unsigned int rand_next;
unsigned int rand(void);
extern uint8_t play_mode_score;									// Scored play: Score goal
extern int play_mode_timed;											// Timed play: Time
extern int play_time_left;											// Timed play: Time left
extern int play_timestamp;											// Timed play: Timestamp
extern uint8_t player_cpu;											// CPU level
extern uint8_t player_cpu_hold;
extern uint8_t playbuffer[4][96];								// Buffer for playing field pixels
extern uint8_t player_indent;										// Gap between players and nextmost wall
extern uint8_t player_left;											// Y-position for left player
extern uint8_t player_right;										// Y-position for left player
extern uint8_t player_ball[2];									// XY-position for the ball
extern uint8_t player_ball_x[96];								// X-position for the upcoming movement
extern uint8_t player_ball_y[96];								// Y-position for the upcoming movement
extern double player_ball_movement_pointer;
extern double player_ball_direction;						// Angle for ball movement;
extern uint8_t play_game_over;
extern uint8_t player_left_pause;
extern uint8_t player_right_pause;
extern uint8_t play_quit_init;
extern uint8_t ball_speed;											// Ball speed
extern uint8_t ball_speed_temp;									// Temporary ball speed
extern uint8_t player_left_speed;
extern uint8_t player_right_speed;
extern uint8_t player_left_command;
extern uint8_t player_right_command;
extern uint8_t player_left_hold;
extern uint8_t player_right_hold;
extern uint8_t player_left_height;
extern uint8_t player_right_height;
extern uint8_t ball_width;
extern uint8_t ball_height;
void playbuffer_clear(void);										// Clear pixelbuffer for playing field
void playing_field_init(void);	 								// Initiate playing field with borders
void playing_field_update(void);								// Update playing field's pixelbuffer
void playing_field_paused(void);
void playing_field_game_over(void);
void playing_exit(void);
void playing_replay(void);
void play_xy(int x, int y); 										// Draw x, y pixel on playing field
void play_xy_clear(int x, int y);								// Erase x, y pixel on playing field
void move_player_left(uint8_t rel_y);						// Move left player relative Y-position
void move_player_right(uint8_t rel_y);					// Move right player relative Y-position
void move_ball();																// Move ball
void timer_countdown(void);
void draw_timer(int time);
void draw_pause(void);
void draw_players(void);												// Draw players on playing field
void draw_ball(void);														// Draw ball on playing field
void check_player_moves(void);									// Check input and move players
void init_ball(void);														// Initiate ball movement
double ball_bounce_calculation_x(double angle);	// Calculate bounces on x
double ball_bounce_calculation_y(double angle);	// Calculate bounces on y
void ball_collision_detection(void);						// Ball collision
double pow_simle(double base, int exp);
double acos_simple(double x);
