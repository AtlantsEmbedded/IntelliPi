/**
 * @file hardware.h
 * @author Ronnie Brash (ron.brash@gmail.com)
 * @brief Hardware specific header - has some macros to handle functions
 */
//#define RASPI 0
//#define X86 0

#define NO_COLOR 0
#define RED_COLOR 1
#define GREEN_COLOR 2
#define YELLOW_COLOR 3
#define BLUE_COLOR 4

#ifdef RASPI

#include <wiringPi.h>
#include <mcp23017.h>
#include <softTone.h>
#include <lcd.h>

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



int raspi_setup(void *param);
int raspi_set_disp_colour(void *param);
int raspi_adafruit_LCD_Setup(void *param);
int raspi_wait_for_button_input(void *param);
int raspi_wait_for_select(void *param);
int raspi_wait_for_arrows(void *param);
int raspi_clear_display(void *param);
int raspi_print_to_display(void *param);

#endif

#ifndef	TRUE
#define	TRUE	(1==1)
#define	FALSE	(1==2)
#endif

// Defines for the Adafruit Pi LCD interface board

#define SIZE_OF_LCD 16		
#define CLEAR_DISPLAY() \
		_CLEAR_DISPLAY(NULL)
		
#define INIT_HARDWARE() \
		_INIT_HARDWARE(NULL)			
		
#define INIT_DISPLAY(param) \
		_INIT_DISPLAY(param)

#define WAIT_FOR_INPUT_SELECT() \
		_WAIT_FOR_INPUT_SELECT(NULL)
		
#define WAIT_FOR_INPUT_ARROWS() \
		_WAIT_FOR_INPUT_ARROWS(NULL)		
		
#define WAIT_FOR_INPUT() \
		_WAIT_FOR_INPUT(NULL)
		
#define OUTPUT_TO_DISPLAY(param) \
		_OUTPUT_TO_DISPLAY(param)	
		
typedef int (*functionPtr_t) (void *);
functionPtr_t _INIT_HARDWARE;
functionPtr_t _INIT_DISPLAY;
functionPtr_t _CLEAR_DISPLAY;
functionPtr_t _OUTPUT_TO_DISPLAY;
functionPtr_t _SET_DISPLAY_COLOR;
functionPtr_t _WAIT_FOR_INPUT_SELECT;
functionPtr_t _WAIT_FOR_INPUT_ARROWS;
functionPtr_t _WAIT_FOR_INPUT;

typedef struct message_str_s {
	char top[16];
	char bottom[16];
} message_str_t;	

int setup_hardware(void);

int cmd_clear_console(void *param);
int cmd_not_used(void *param);
int cmd_print_to_display(void *);
int cmd_wait_for_arrows(void *param);
int cmd_wait_for_select_keys(void *param);
int cmd_wait_for_arrow_keys(void *param);
int cmd_wait_for_console_input_keys(void *param);
