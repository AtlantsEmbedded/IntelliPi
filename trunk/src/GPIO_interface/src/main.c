/**
 * @file lcd-adafruit.c:
 * @brief Text-based LCD driver test code
 *	This is designed to drive the Adafruit RGB LCD Plate
 *	with the additional 5 buttons for the Raspberry Pi
 *  based loosely on Gordon Henderson's original driver code
 * @author Gordon Henderson (original author)
 * @author Ronnie Brash (ron.brash@gmail.com)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#include <wiringPi.h>
#include <mcp23017.h>
#include <lcd.h>
#include "main.h"

static const char *message = " GPIO interface ";

// Global lcd handle:

static int lcdHandle;

/**
 * usage(const char *progName)
 * @brief program usage
 * @param progName
 * @return -1 for failure
 */
int usage(const char *progName)
{
	fprintf(stderr, "Usage: %s colour (0-7 where 0 is no backlight)\n", progName);
	return EXIT_FAILURE;
}

/**
 * setBacklightColour(int colour)
 * @brief The colour outputs are inverted.
 * @param color
 */
static inline void setBacklightColour(int colour)
{
	colour &= 7;

	digitalWrite(AF_RED, !(colour & 1));
	digitalWrite(AF_GREEN, !(colour & 2));
	digitalWrite(AF_BLUE, !(colour & 4));
}

/**
 * adafruitLCDSetup(int colour)
 * @param color
 * @brief Setup the Adafruit board by making sure the additional pins are
 *	set to the correct modes, etc.
 *********************************************************************************
 */

static void adafruitLCDSetup(int colour)
{
	int i = 0;
	lcdHandle = 0;

	// Backlight LEDs

	pinMode(AF_RED, OUTPUT);
	pinMode(AF_GREEN, OUTPUT);
	pinMode(AF_BLUE, OUTPUT);
	setBacklightColour(colour);

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
		fprintf(stderr, "lcdInit failed\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * waitForEnter(void)
 * @brief waits for select to be pressed
 */
static void waitForEnter(void)
{
	printf("Press SELECT to continue: ");
	fflush(stdout);

	while (digitalRead(AF_SELECT) == HIGH) {	// Wait for push
		delay(1);
	}

	while (digitalRead(AF_SELECT) == LOW) {	// Wait for release
		delay(1);
	}

	printf("OK\n");
}

/**
 * waitForDown(button_s *state)
 * @brief waits for Down button to be pressed
 * @param state
 */
static void waitForDown(button_s * state)
{
	if (digitalRead(AF_DOWN) == LOW)	// Pushed
	{
		printf("down was pressed\n");
		state->colour = state->colour - 1;
		if (state->colour == -1) {
			state->colour = 7;
		}
		setBacklightColour(state->colour);
		state->waitForRelease = TRUE;
	}

}

/**
 * waitForUp(button_s *state)
 * @brief waits for Up button to be pressed
 * @param state
 */
static void waitForUp(button_s * state)
{

	if (digitalRead(AF_UP) == LOW)	// Pushed
	{
		printf("up was pressed\n");
		state->colour = state->colour + 1;
		if (state->colour == 8) {
			state->colour = 0;
		}
		setBacklightColour(state->colour);
		state->waitForRelease = TRUE;
	}

}

/**
 * waitForLeft(button_s *state)
 * @brief waits for Left button to be pressed
 * @param state
 */
static void waitForLeft(button_s * state)
{
	if (digitalRead(AF_LEFT) == LOW)	// Pushed
	{
		printf("left was pressed\n");
		state->colour = state->colour + 1;
		if (state->colour == 8) {
			state->colour = 0;
		}
		setBacklightColour(state->colour);
		state->waitForRelease = TRUE;
	}
}

/**
 * waitForRight(button_s *state)
 * @brief waits for Right button to be pressed
 * @param state
 */
static void waitForRight(button_s * state)
{
	if (digitalRead(AF_RIGHT) == LOW)	// Pushed
	{
		printf("right was pressed\n");
		state->colour = state->colour + 1;
		if (state->colour == 8) {
			state->colour = 0;
		}
		setBacklightColour(state->colour);
		state->waitForRelease = TRUE;
	}
}

/**
 * main(int argc, char *argv[])
 * @brief main function
 * @param argc
 * @param argv
 * @return 0 for success, -1 for error
 */
int main(int argc, char *argv[])
{
	int cols = 16;
	button_s b_state = { 0 };
	b_state.waitForRelease = FALSE;
	b_state.colour = 1;

	struct tm *t = NULL;
	time_t tim_t;

	char buf[32] = { 0 };

	// Check arguments
	if (argc != 2) {
		return usage(argv[0]);
	}

	printf("Raspberry Pi Adafruit GPIO App\n");
	printf("==============================\n");

	b_state.colour = atoi(argv[1]);

	wiringPiSetupSys();
	mcp23017Setup(AF_BASE, 0x20);

	adafruitLCDSetup(b_state.colour);

	lcdPosition(lcdHandle, 0, 0);
	lcdPuts(lcdHandle, "Adafruit RGB LCD");

	lcdPosition(lcdHandle, 0, 1);
	lcdPuts(lcdHandle, "Press Sel to cont");

	waitForEnter();

	lcdClear(lcdHandle);

	for (;;) {
		lcdPosition(lcdHandle, 0, 0);
		lcdPuts(lcdHandle, message);

		tim_t = time(NULL);
		t = localtime(&tim_t);

		sprintf(buf, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);

		lcdPosition(lcdHandle, (cols - 8) / 2, 1);
		lcdPuts(lcdHandle, buf);

		// Check buttons to cycle colour
		// If Up or Down are still pushed, then skip
		if (b_state.waitForRelease) {
			if ((digitalRead(AF_UP) == LOW) || (digitalRead(AF_DOWN) == LOW)
			    || (digitalRead(AF_LEFT) == LOW) || (digitalRead(AF_RIGHT) == LOW)) {
				continue;
			} else {
				b_state.waitForRelease = FALSE;
			}
		}
		// Check for button presses
		waitForUp(&b_state);
		waitForDown(&b_state);
		waitForRight(&b_state);
		waitForLeft(&b_state);

	}

	return 0;
}
