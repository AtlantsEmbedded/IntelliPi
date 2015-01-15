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
#include <ctype.h>

#include <wiringPi.h>
#include <mcp23017.h>
#include <lcd.h>
#include "main.h"

static const char *AM2302_CMD = "/usr/bin/dht22_interface";
static const char *DSPROBE_CMD = "cat /sys/devices/w1_bus_master1/28-*/w1_slave";

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
	float set_point = 0;

	struct tm *t = NULL;
	time_t tim_t;

	// Check arguments
	if (argc != 2) {
		return usage(argv[0]);
	}

	printf("MaplePI GPIO App\n");
	printf("==============================\n");

	b_state.colour = atoi(argv[1]);

	wiringPiSetupSys();
	mcp23017Setup(AF_BASE, 0x20);

	adafruitLCDSetup(b_state.colour);

	char am_buffer[17] = { 0 };
	char probe_buffer[1024] = { 0 };
	char bottom_buffer[17] = "c";	//"C123.45 S123.45f"
	int i = 0;
	int len = 0;

	for (;;) {
		// Retrieve out the AM2302 sensor data
		FILE *am_file = NULL;
		am_file = popen(AM2302_CMD, "r");
		fgets(am_buffer, 17, am_file);
		pclose(am_file);
		
		printf("AM2302 output:%s\n",am_buffer);

		// Output AM2302 data onto the top row of the LCD
		lcdPosition(lcdHandle, 0, 0);
		lcdPuts(lcdHandle, am_buffer);

		// Retrieve out DS probe sensor data
		FILE *probe_file = NULL;
		probe_file = popen(DSPROBE_CMD, "r");
		fgets(probe_buffer, 1024, probe_file);
		pclose(probe_file);
		
		printf("DS output:%s len:%d\n",probe_buffer,len);

		// Raw buffer will be a string - read until a '='
		len = strlen(probe_buffer);
		for (i = len; i > 0; i--) {
			if ((probe_buffer[i] == '=')) {
				break;
			}
		}

		printf("Found a = at:%d\n",i);
		// Convert the temperature string to a float
		char tmp_buffer[8] = { 0 };
		memcpy(tmp_buffer, *probe_buffer + i, (len - i));

		float actual_temp = atof(tmp_buffer) / 1000;

		printf("actual temp: %3.2f\n",actual_temp);
		// Build the final string (obviously not efficient)
		snprintf(bottom_buffer, 17,"c%3.2f s%3.2f", actual_temp, set_point);

		// Output DS data onto the bottom row of the LCD
		lcdPosition(lcdHandle, 0, 1);
		lcdPuts(lcdHandle, bottom_buffer);

		printf("%s\n", am_buffer);
		printf("%s\n", bottom_buffer);

	}

	return 0;
}
