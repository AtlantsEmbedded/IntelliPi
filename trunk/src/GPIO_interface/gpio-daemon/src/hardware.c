/**
 * @file hardware.c
 * @author Ronnie Brash (ron.brash@gmail.com)
 * @brief File to contain specific hardware related functions
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#ifdef RASPI
#include <wiringPi.h>
#include <mcp23017.h>
#include <softTone.h>
#include <lcd.h>
#endif

#include <include/main.h>
#include <include/menu_structure.h>
#include <include/menu_item.h>
#include <include/user_input.h>
#include <include/socket.h>
#include <include/event.h>
#include <include/hardware.h>

const char accepted_input_chars[] = { 's', 'b', 'c', 'q', 'a', 'd' };

#define SELECT_CHAR accepted_input_chars[0]
#define BACK_CHAR accepted_input_chars[1]
#define CANCEL_CHAR accepted_input_chars[2]
#define QUIT_CHAR accepted_input_chars[3]
#define PREV_CHAR accepted_input_chars[4]
#define NEXT_CHAR accepted_input_chars[5]

/// Console related functions for testing from the command line

#ifdef RASPI
/// LCD handle
static int lcdHandle;
#endif

/**
 * setup_hardware(void)
 * @brief setup hardware based on defines passed to the compiler
 * @note will default to else if not set (which is X86)
 */
int setup_hardware(void)
{
	_INIT_HARDWARE = NULL;
	_INIT_DISPLAY = NULL;
	_SET_DISPLAY_COLOR = NULL;
	_OUTPUT_TO_DISPLAY = NULL;
	_CLEAR_DISPLAY = NULL;
	_WAIT_FOR_INPUT_SELECT = NULL;
	_WAIT_FOR_INPUT_ARROWS = NULL;
	_WAIT_FOR_INPUT = NULL;

	/// Based on hardware, setup function pointers
#ifdef RASPI
	_INIT_HARDWARE = &raspi_setup;
	_INIT_DISPLAY = &raspi_adafruit_LCD_Setup;
	_SET_DISPLAY_COLOR = &raspi_set_disp_colour;
	_CLEAR_DISPLAY = &raspi_clear_display;
	_OUTPUT_TO_DISPLAY = &raspi_print_to_display;
	_WAIT_FOR_INPUT_SELECT = &raspi_wait_for_select;
	_WAIT_FOR_INPUT_ARROWS = &raspi_wait_for_arrows;
	_WAIT_FOR_INPUT = &raspi_wait_for_button_input;
	return (0);
#elif X86
	_INIT_HARDWARE = &cmd_not_used;
	_INIT_DISPLAY = &cmd_clear_console;
	_SET_DISPLAY_COLOR = &cmd_not_used;
	_CLEAR_DISPLAY = &cmd_clear_console;
	_OUTPUT_TO_DISPLAY = &cmd_print_to_display;
	_WAIT_FOR_INPUT_SELECT = &cmd_wait_for_select_keys;
	_WAIT_FOR_INPUT_ARROWS = &cmd_wait_for_arrows;
	_WAIT_FOR_INPUT = &cmd_wait_for_console_input_keys;
	return (0);
#endif

	return (-1);
}

#ifdef RASPI
/**
 * raspi_set_disp_colour(int colour)
 * @brief The colour outputs are inverted.
 * @param color
 */
int raspi_set_disp_colour(void *param)
{
	int colour = (intptr_t) param;
	colour &= 7;

	digitalWrite(AF_RED, !(colour & 1));
	digitalWrite(AF_GREEN, !(colour & 2));
	digitalWrite(AF_BLUE, !(colour & 4));
	return (0);
}

/**
 * raspi_adafruit_LCD_Setup(int colour)
 * @param color
 * @brief Setup the Adafruit board by making sure the additional pins are
 *	set to the correct modes, etc.
 */
int raspi_adafruit_LCD_Setup(void *param)
{
	int colour = (intptr_t) param;
	int i = 0;
	lcdHandle = 0;

	// Backlight LEDs
	pinMode(AF_RED, OUTPUT);
	pinMode(AF_GREEN, OUTPUT);
	pinMode(AF_BLUE, OUTPUT);
	raspi_set_disp_colour(&colour);

	// Input buttons
	for (i = 0; i <= 4; ++i) {
		pinMode(AF_BASE + i, INPUT);
		pullUpDnControl(AF_BASE + i, PUD_UP);	// Enable pull-ups, switches close to 0v
	}

	// Control signals
	pinMode(AF_RW, OUTPUT);
	digitalWrite(AF_RW, LOW);	// Not used with wiringPi - always in write mode

	// The other control pins are initialised with lcdInit ()
	lcdHandle = lcdInit(2, 16, 4, AF_RS, AF_E, AF_DB4, AF_DB5, AF_DB6, AF_DB7, 0, 0, 0, 0);

	if (lcdHandle < 0) {
		debug_msg("lcdInit failed\n");
		exit(EXIT_FAILURE);
	}
	return (0);
}

/**
 * raspi_setup(void)
 * @brief initialize RASPI hardware & Adafruit LCD
 * @return 0
 */
int raspi_setup(void *param __attribute__ ((unused)))
{

	/// Setup wiringPi Library
	wiringPiSetup();

	/// Setup LCD
	mcp23017Setup(AF_BASE, 0x20);

	INIT_DISPLAY(BLUE_COLOR);

	return (0);
}

/**
 * raspi_wait_for_select (void)
 * @brief RASPI specific version for wait for select keys (SEL)
 * @return TRUE or 0 if completed
 */
int raspi_wait_for_select(void *param __attribute__ ((unused)))
{
	debug_msg("Press SELECT to continue: \n");

	while (digitalRead(AF_SELECT) == HIGH)	// Wait for push
		delay(1);

	while (digitalRead(AF_SELECT) == LOW)	// Wait for release
		delay(1);

	return (0);
}

/**
 * raspi_wait_for_arrows (void)
 * @brief RASPI specific version for wait for arrow keys (left and right)
 * @return TRUE or 1 if completed
 */
int raspi_wait_for_arrows(void *param __attribute__ ((unused)))
{
	debug_msg("Press ARROWS (left/Right) to continue: \n");

	int res = 0;
	for (;;) {
		if (res >= 1) {
			if ((digitalRead(AF_UP) == LOW) || (digitalRead(AF_DOWN) == LOW)
			    || (digitalRead(AF_RIGHT) == LOW) || (digitalRead(AF_LEFT) == LOW))
				continue;
			else
				return (0);
		}

		if (digitalRead(AF_LEFT) == LOW) {
			res = PREV_EVENT;
			debug_msg("Prev event\n");
		}
		if (digitalRead(AF_RIGHT) == LOW) {
			res = NEXT_EVENT;
			debug_msg("Next event\n");
		}
	}

	return (0);
}

/**
 * raspi_clear_display()
 * @brief clear LCD display
 * @return 0 on success, -1 for error
 */
int raspi_clear_display(void *param __attribute__ ((unused)))
{
	lcdClear(lcdHandle);
	return (0);
}

/**
 * raspi_print_to_display(char *topline, char *bottomline)
 * @brief clear LCD display
 * @param param
 * @return 0 on success, -1 for error
 */
int raspi_print_to_display(void *param)
{
	message_str_t *msg = (message_str_t *) param;

	CLEAR_DISPLAY();
	lcdPosition(lcdHandle, 0, 0);
	lcdPuts(lcdHandle, msg->top);
	lcdPosition(lcdHandle, 0, 1);
	lcdPuts(lcdHandle, msg->bottom);

	return (0);
}

/**
 * raspi_wait_for_button_input(void) 
 * @brief Wait for button input
 * @return 0 for success, -1 for error
 */
int raspi_wait_for_button_input(void *param __attribute__ ((unused)))
{
	debug_msg("Waiting for button input:\n");
	int res = -1;
	for (;;) {
		if (res >= 1) {
			if ((digitalRead(AF_UP) == LOW) || (digitalRead(AF_DOWN) == LOW)
			    || (digitalRead(AF_RIGHT) == LOW) || (digitalRead(AF_LEFT) == LOW))
				continue;
			else
				return res;
		}

		if (digitalRead(AF_UP) == LOW) {
			res = BACK_EVENT;
			debug_msg("Back event\n");
		}
		if (digitalRead(AF_DOWN) == LOW) {
			res = CANCEL_CHAR;
			debug_msg("Cancel event\n");
		}
		if (digitalRead(AF_LEFT) == LOW) {
			res = PREV_EVENT;
			debug_msg("Prev event\n");
		}
		if (digitalRead(AF_RIGHT) == LOW) {
			res = NEXT_EVENT;
			debug_msg("Next event\n");
		}
		if (digitalRead(AF_SELECT) == LOW) {
			res = SELECT_EVENT;
			debug_msg("Select event\n");
		}

	}

	return res;
}
#endif

/**
 * cmd_clear_console(void *param)
 * @brief Clear console
 * @param param
 * @return 0
 */
int cmd_clear_console(void *param __attribute__ ((unused)))
{
	printf("\e[1;1H\e[2J");
	return (0);
}

/**
 * cmd_wait_for_arrows()
 * @brief Waits for errors
 * @return {NEXT_EVENT, or PREV_EVENT}
 */
int cmd_wait_for_arrows(void *param __attribute__ ((unused)))
{

	set_current_menu_item(g_node_first_child(return_root_node()));

	cmd_clear_console(NULL);
	print_item_text(get_current_node());
	print_item_menu(get_current_node());

	int res = 0;
	res = cmd_wait_for_arrow_keys(NULL);
	if (res == NEXT_EVENT) {
		/// Set node to first node of the main menu
		set_current_menu_item(g_node_first_child(get_current_node()));
	} else if (res == PREV_EVENT) {
		/// Set node to first node of the main menu
		set_current_menu_item(g_node_last_child(get_current_node()));
	}
	return res;
}

/**
 * wait_for_console_input()
 * @brief Waits for user input on console
 * @return -1 for error (should never happen), 
 */
int cmd_wait_for_console_input_keys(void *param __attribute__ ((unused)))
{

	char buff[2] = { 0 };

	debug_msg("Enter input:\n");
	int res = 0;
	for (;;) {

		memset(buff, 0, sizeof(buff));
		buff[1] = '\0';

		if (fgets(buff, sizeof(buff), stdin) == NULL) {
			if (puts("EOF or read error\n") == EOF) {
				continue;
			}
		} else {
			errno = 0;
			if (buff[0] == '\0' || buff[0] == '\n') {
				continue;
			}
			if (isalpha(buff[0]) != 0) {
				if (buff[0] == QUIT_CHAR) {
					debug_msg("Quitting...\n");
					res = QUIT_EVENT;
					break;
				} else if (buff[0] == SELECT_CHAR) {
					debug_msg("Selecting option\n");
					res = SELECT_EVENT;
					break;
				} else if (buff[0] == BACK_CHAR) {
					debug_msg("Returning to previous menu\n");
					res = BACK_EVENT;
					break;
				} else if (buff[0] == CANCEL_CHAR) {
					debug_msg("Canceling task\n");
					res = CANCEL_EVENT;
					break;
				} else if (buff[0] == NEXT_CHAR) {
					debug_msg("Next task\n");
					res = NEXT_EVENT;
					break;
				} else if (buff[0] == PREV_CHAR) {
					debug_msg("Previous task\n");
					res = PREV_EVENT;
					break;
				} else {
					INPUT_ERROR_MSG;
					continue;
				}
			} else {
				INPUT_ERROR_MSG;
				continue;
			}

		}

	}
	return res;
}

/**
 * cmd_not_used(void *param)
 * @brief used when not required
 * @param param
 * @return 0
 */
int cmd_not_used(void *param __attribute__ ((unused)))
{
	return (0);
}

/**
 * cmd_print_to_display(void *param) 
 * @brief print_out_to_display
 * @param param
 * @return 0
 */
int cmd_print_to_display(void *param)
{
	char *str = (char *)param;
	CLEAR_DISPLAY();
	debug_msg("%.16s\n", str);
	debug_msg("%.16s\n", str + 16);
	return (0);
}

/**
 * cmd_wait_for_select_keys()
 * @brief waits for select key
 * @returns {QUIT_EVENT, SELECT_EVENT }
 */
int cmd_wait_for_select_keys(void *param __attribute__ ((unused)))
{

	char buff[2] = { 0 };

	debug_msg("Enter input {s,q}:\n");
	int res = 0;
	for (;;) {

		memset(buff, 0, sizeof(buff));
		buff[1] = '\0';

		if (fgets(buff, sizeof(buff), stdin) == NULL) {
			if (puts("EOF or read error\n") == EOF) {
				continue;
			}
		} else {
			errno = 0;
			if (buff[0] == '\0' || buff[0] == '\n') {
				continue;
			}
			if (isalpha(buff[0]) != 0) {
				if (buff[0] == QUIT_CHAR) {
					debug_msg("Quitting...\n");
					res = QUIT_EVENT;
					break;
				} else if (buff[0] == SELECT_CHAR) {
					debug_msg("Selecting event\n");
					res = SELECT_EVENT;
					break;
				} else {
					continue;

				}

			}
		}
	}
	return res;

}

/**
 * cmd_wait_for_arrow_keys()
 * @brief wait for arrow keys to be pressed
 * @return {QUIT_EVENT, PREV_EVENT, NEXT_EVENT }
 */
int cmd_wait_for_arrow_keys(void *param __attribute__ ((unused)))
{

	char buff[2] = { 0 };

	debug_msg("Enter input {a,d,q}:\n");
	int res = 0;
	for (;;) {

		memset(buff, 0, sizeof(buff));
		buff[1] = '\0';

		if (fgets(buff, sizeof(buff), stdin) == NULL) {
			if (puts("EOF or read error\n") == EOF) {
				continue;
			}
		} else {
			errno = 0;
			if (buff[0] == '\0' || buff[0] == '\n') {
				continue;
			}
			if (isalpha(buff[0]) != 0) {
				if (buff[0] == QUIT_CHAR) {
					debug_msg("Quitting...\n");
					res = QUIT_EVENT;
					break;
				} else if (buff[0] == NEXT_CHAR) {
					debug_msg("Next task\n");
					res = NEXT_EVENT;
					break;
				} else if (buff[0] == PREV_CHAR) {
					debug_msg("Previous task\n");
					res = PREV_EVENT;
					break;
				} else {
					continue;

				}

			}
		}
	}
	return res;

}
