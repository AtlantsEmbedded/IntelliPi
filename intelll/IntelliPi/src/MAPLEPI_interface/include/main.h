/**
 * @file main.h
 * @brief Header for the main file
 * 
 */
#ifndef	TRUE
#define	TRUE	(1==1)
#define	FALSE	(1==2)
#endif

// Defines for the Adafruit Pi LCD interface board

#define	AF_BASE		100
#define	AF_RED		(AF_BASE + 6)
#define	AF_GREEN	(AF_BASE + 7)
#define	AF_BLUE		(AF_BASE + 8)

#define	AF_E		(AF_BASE + 13)
#define	AF_RW		(AF_BASE + 14)
#define	AF_RS		(AF_BASE + 15)

#define	AF_DB4		(AF_BASE + 12)
#define	AF_DB5		(AF_BASE + 11)
#define	AF_DB6		(AF_BASE + 10)
#define	AF_DB7		(AF_BASE +  9)

#define	AF_SELECT	(AF_BASE +  0)
#define	AF_RIGHT	(AF_BASE +  1)
#define	AF_DOWN		(AF_BASE +  2)
#define	AF_UP		(AF_BASE +  3)
#define	AF_LEFT		(AF_BASE +  4)

#define RUNNING 1
#define NOT_RUNNING 0

#define RED_COLOR 1
#define GREEN_COLOR 2
#define YELLOW_COLOR 3

#define SIZE_OF_LCD 16

#define DEFAULT_SETPOINT 219.00
#define AMOUNT_TO_CHANGE 0.5

#define OPEN 1
#define CLOSED 0

typedef struct {
	int waitForRelease;
	int colour;
} button_s;

static int usage(const char *progName);
static inline void print_banner();
static inline void setBacklightColour(int colour);
static void adafruitLCDSetup(int colour);
static inline void mode_button(button_s * state);
static void up_temp_button(button_s * state);
static void down_temp_button(button_s * state);
static void open_relay();
static void close_relay();
static void turn_off_beeper();
static void turn_on_beeper();
static void manage_relay(float actual_temp);
static inline void setup_buttons();
static void get_am2302_data(char *top_buffer[]);
static void get_ds_data(float *actual_temp);
static inline void build_bottom_string(char *bottom_buffer[], float actual_temp);
static int check_time(time_t * old_time);

