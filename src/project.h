#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)			// OLED data(1)/command(0) select 	- 0000 0000 0001 0000 (RF4 - PIN 39)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)					// OLED data(1)/command(0) select   -

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)							// OLED reset 											- 0000 0010 0000 0000 (RG9 - PIN 10)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)								// OLED no reset 										- 0000 0010 0000 0000 (RG9 - PIN 10)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)								// OLED VDD enable 									- 0000 0000 0100 0000 (RF6 - PIN 38)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)								// OLED VBAT enable 								- 0000 0000 0010 0000 (RF5 - PIN 40)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)								// OLED VDD disable 								- 0000 0000 0100 0000 (RF6 - PIN 38)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)								// OLED VBAT disable 								- 0000 0000 0010 0000 (RF5 - PIN 40)

#define PI 3.14159265

#define BIT_BUTTON_LEFT								0
#define BIT_BUTTON_RIGHT							7
#define BIT_BUTTON_UP									2
#define BIT_BUTTON_DOWN								1
#define BIT_BUTTON_START							3
#define BIT_BUTTON_SELECT							4
#define BIT_BUTTON_A									6
#define BIT_BUTTON_B									5

#define DATA_BUTTON_LEFT(ctrl_data)   ((ctrl_data >> BIT_BUTTON_LEFT) & 1)
#define DATA_BUTTON_RIGHT(ctrl_data)  ((ctrl_data >> BIT_BUTTON_RIGHT) & 1)
#define DATA_BUTTON_UP(ctrl_data)     ((ctrl_data >> BIT_BUTTON_UP) & 1)
#define DATA_BUTTON_DOWN(ctrl_data)   ((ctrl_data >> BIT_BUTTON_DOWN) & 1)
#define DATA_BUTTON_START(ctrl_data)  ((ctrl_data >> BIT_BUTTON_START) & 1)
#define DATA_BUTTON_SELECT(ctrl_data) ((ctrl_data >> BIT_BUTTON_SELECT) & 1)
#define DATA_BUTTON_A(ctrl_data)      ((ctrl_data >> BIT_BUTTON_A) & 1)
#define DATA_BUTTON_B(ctrl_data)      ((ctrl_data >> BIT_BUTTON_B) & 1)

// Error handling
extern int errno;

//
void display_init(void);
uint8_t spi_send_recv(uint8_t data);
void quicksleep(int cyc);

//
extern const uint8_t const font[128*8];
extern const uint8_t const numbers_mini[11*4];

// Clear whole screen
void screen_clear(void);

// Main
extern uint8_t in_game;
extern uint8_t sound_on;
unsigned int rand(void);

// Sound
extern uint8_t sound_status;
void sound_init(void);
void play_sound(uint16_t frequency, uint16_t duration, uint16_t delay);

extern uint8_t music_play_got;
extern uint8_t music_iteration;
void music_got_playing();

// Input
extern uint8_t controller_input_a;
extern uint8_t controller_input_b;
extern uint8_t controller_input_a_buffer;
extern uint8_t controller_input_b_buffer;

// Timers
void timer_init(void);
void frame_update(void);

// Menu
void select_option(void);
void check_buttons(void);
extern uint8_t in_options;
extern uint8_t in_credits;
extern uint8_t in_menu;
extern char menu_gametitle[16];
extern int credits_counter;
void write_to_display(char arr[], uint8_t len, uint8_t row, uint8_t col);

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
extern double ball_speed_temp;

extern uint8_t ball_size;
extern uint8_t ball_speed;
extern uint8_t player_heights[2];
extern uint8_t player_speeds[2];
extern uint8_t player_cpu;
extern uint8_t play_mode_timed;
extern uint8_t sc1;
extern uint8_t sc2;

void display_score(uint8_t sc, int on_right);
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
