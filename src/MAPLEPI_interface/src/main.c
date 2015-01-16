/**
 * @file main.c
 * @author Ronnie Brash (ron.brash@gmail.com)
 * @date Jan 16, 2014
 * 
 * @brief LCD driver code which will be used to control
 * a RaspberryPI (B+/A+) that monitors environmental data,
 * drives a relay which would open an electronic solenoid,
 * have a user settable set point and ON/OFF modes.
 * 
 * @note Depends on an ADAfruit LCD Plate,
 * AM2302 sensor, a DS18B20 sensor with 1w kernel support, 
 * a relay and several buttons driven by GPIO.
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

#define RUNNING 1
#define NOT_RUNNING 0

#define RED_COLOR 1
#define GREEN_COLOR 2
#define YELLOW_COLOR 3

// Commands to retrieve information from the sensors
static const char *AM2302_CMD = "/usr/bin/dht22_interface";
static const char *DSPROBE_CMD = "cat /sys/devices/w1_bus_master1/28-*/w1_slave | sed -n 2p";

static const char *OPEN_RELAY = "gpio write 0 1";
static const char *CLOSE_RELAY = "gpio write 0 0";

// Pins used (wiringPi numbers)
const int RELAY_PIN = 0;
const int UP_TMP_PIN = 6;
const int DN_TMP_PIN = 13;
const int MODE_PIN = 2;
const int BEEPER_PIN = 12;

// Default temperature to be used as a setpoint
const float DEFAULT_SETPOINT = 150.00;

// Device mode {RUNNING, NOT_RUNNING}
static int device_mode;

// LCD handle
static int lcdHandle;

static float set_point;
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

static inline void mode_button()
{
	if (digitalRead(MODE_PIN)) {
		if (device_mode == RUNNING) {
			printf("Mode changed to NOT-RUNNING!\n");
			device_mode = NOT_RUNNING;
		} else {
			printf("Mode changed to RUNNING!\n");
			device_mode = RUNNING;
		}
	}
}

static inline void up_temp_button()
{
	if (digitalRead(UP_TMP_PIN)) {
		set_point+=0.01;
	}
}

static inline void down_temp_button()
{
	if (digitalRead(DN_TMP_PIN)) {
		set_point-=0.01;
	}
}

static inline void open_relay()
{

}

static inline void close_relay()
{

}

/**
 * setup_buttons()
 * @brief Setup buttons to be used by the external 
 * user interface - to be manipulated by the user
 */
static inline void setup_buttons()
{
	/*
	 * For each button:
	 * 1.) Set button as INPUT
	 * 2.) Enable pull-up resistor on button
	 */
	pinMode(UP_TMP_PIN, INPUT);
	pullUpDnControl(UP_TMP_PIN, PUD_UP);

	pinMode(DN_TMP_PIN, INPUT);
	pullUpDnControl(DN_TMP_PIN, PUD_UP);

	pinMode(MODE_PIN, INPUT);
	pullUpDnControl(MODE_PIN, PUD_UP);

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
	b_state.colour = RED_COLOR;
	set_point = DEFAULT_SETPOINT;

	char am_buffer[17] = { 0 };
	char probe_buffer[1024] = { 0 };
	char bottom_buffer[17] = "c";	//"123.45-123.45 {Ou,No}"
	register int i = 0;
	register int len = 0;
	float actual_temp = 0;

	struct tm *t = NULL;
	time_t tim_t;

	// Check arguments
	if (argc != 2) {
		return usage(argv[0]);
	}

	printf("MaplePI GPIO/Monitoring App\n\n");
	printf("Author: Ronnie brash (ron.brash@gmail.com)\n");
	printf("------------------------------------------\n");

	b_state.colour = atoi(argv[1]);

	wiringPiSetupSys();
	mcp23017Setup(AF_BASE, 0x20);

	adafruitLCDSetup(b_state.colour);

	// Set device mode to not running
	device_mode = NOT_RUNNING;

	// Initialize the user interface buttons (not on plate)
	//setup_buttons();

	for (;;) {
		// Check GPIO/buttons first
		// MODE select button (yellow)
		mode_button();
		up_tmp_button();
		down_tmp_button();

		// Retrieve out the AM2302 sensor data
		FILE *am_file = NULL;
		am_file = popen(AM2302_CMD, "r");
		fgets(am_buffer, 17, am_file);
		pclose(am_file);

		printf("AM2302  output:%s\n", am_buffer);

		// Output AM2302 data onto the top row of the LCD
		lcdPosition(lcdHandle, 0, 0);
		lcdPuts(lcdHandle, am_buffer);

		// Retrieve out DS probe sensor data
		FILE *probe_file = NULL;
		probe_file = popen(DSPROBE_CMD, "r");
		fgets(probe_buffer, 1024, probe_file);
		pclose(probe_file);

		printf("DS18B20 output:%s\n", probe_buffer);

		// Raw buffer will be a string - read until a '='
		len = strlen(probe_buffer);
		for (i = len; i > 0; i--) {
			if ((probe_buffer[i] == '=')) {
				break;
			}
		}

		// Convert the temperature string to a float
		char tmp_buffer[8] = { 0 };
		char *buff_ptr = &probe_buffer;

		// Increment buffer to remove the '=' from the string
		buff_ptr += 1;
		memcpy(tmp_buffer, buff_ptr + i, (len - i));

		// Convert string to a float and divide by 1000
		actual_temp = (1.8 * (atof(tmp_buffer) / 1000)) + 32;

		// Build the final string (obviously not efficient)
		snprintf(bottom_buffer, 17, "%3.2f-%3.2f ", actual_temp, set_point);

		if (device_mode == RUNNING) {
			strcat(bottom_buffer, "Ou");
		} else {
			strcat(bottom_buffer, "No");
		}

		// Output DS data onto the bottom row of the LCD
		lcdPosition(lcdHandle, 0, 1);
		lcdPuts(lcdHandle, bottom_buffer);

		printf("\nDisplay will show -----------------------\n");
		printf("%s\n", am_buffer);
		printf("%s\n", bottom_buffer);
		printf("-----------------------------------------\n");

	}

	return 0;
}
