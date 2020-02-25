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
void select_option(void);
void check_buttons(void);
/*
extern char menu_gametitle[16];
extern char menu_oneplayer[2];
extern char menu_twoplayer[2];
extern char menu_options[3];
// extern char credits[4];
// extern char exit[4];
extern char paddle_size[9];
extern char pad_size_medium[3];
extern char pad_size_large[5];
extern char pad_size_small[5];
extern char opt_ball_speed[11];
extern char opt_ball_speed_fast[4];
extern char opt_ball_speed_slow[4];
extern char ball_size[10];
extern char ball_size_2x2[3];
extern char ball_size_4x4[3];
extern char ball_size_6x6[3];
extern char game_count_down[6];
extern char game_difficulty[5];
extern char diff_normal[6];
extern char diff_hard[4];
extern char diff_godmode[7];
extern char diff_easy[4];
*/


// Scores
extern uint8_t sc1;
extern uint8_t sc2;
void display_score(uint8_t sc, int on_right);

// Playing
extern uint8_t play_time_left;
extern uint8_t play_state;

extern uint8_t player_commands[2];
extern uint8_t player_holds[2];
extern uint8_t player_cpu_hold;
extern int play_timestamp;
extern uint8_t player_ball[2];
extern uint8_t player_coordinates[2];
extern double player_ball_movement_pointer;
extern uint8_t player_ball_movemestatic[2][96];
extern double player_ball_direction; 
extern uint8_t ball_speed_temp;

extern uint8_t ball_size;
extern uint8_t ball_speed;
extern uint8_t player_heights[2];
extern uint8_t player_speeds[2];
extern uint8_t player_cpu;
extern uint8_t play_mode_timed;

unsigned int rand(void);
void playbuffer_clear(void);
void play_xy_update(void);
void play_xy_set(int x, int y);
void move_player(uint8_t id_player, uint8_t rel_y);
void move_ball();
void player_input_movement_action(uint8_t id_player, uint8_t controller_data, uint8_t input, int8_t movement);
void player_input_pause(uint8_t id_player, uint8_t controller_data);
void cpu_movement();
void check_player_moves(void);
double ball_bounce_calculation(double angle, uint8_t use_y);
void ball_collision_detection(void);
void draw_playing_field(void);
void draw_players(void);
void draw_ball(void);
void draw_message(char *message);
void timer_countdown(void);
void draw_timer(int time);
void playing_field_update(void);
void playing_reset(int exit);
