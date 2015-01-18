/**
 * @file main.c
 * @author Ronnie Brash (ron.brash@gmail.com)
 * @date Jan SIZE_OF_LCD, 2014
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

#define SIZE_OF_LCD 16

#define DEFAULT_SETUPOINT 255.00

// Commands to retrieve information from the sensors
static const char *AM2302_CMD = "/usr/bin/dht22_interface";
static const char *DSPROBE_CMD = "cat /sys/devices/w1_bus_master1/28-*/w1_slave | sed -n 2p";

// Pins used (wiringPi numbers)
const int RELAY_PIN = 0;
const int UP_TMP_PIN = 6;
const int DN_TMP_PIN = 13;
const int MODE_PIN = 2;
const int BEEPER_PIN = 12;

// Default temperature to be used as a setpoint
static float set_point;

// Device mode {RUNNING, NOT_RUNNING}
static int device_mode;

// LCD handle
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
	lcdHandle = lcdInit(2, SIZE_OF_LCD, 4, AF_RS, AF_E, AF_DB4, AF_DB5, AF_DB6, AF_DB7, 0, 0, 0, 0);

	if (lcdHandle < 0) {
		fprintf(stderr, "lcdInit failed\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * mode_button()
 * @brief Handles mode button functionality
 */
static inline void mode_button()
{
	if (digitalRead(MODE_PIN) == HIGH) {
		return;
	}

	if (device_mode == RUNNING) {
		setBacklightColour(RED_COLOR);
		printf("Mode changed to NOT-RUNNING!\n");
		device_mode = NOT_RUNNING;
	} else {
		printf("Mode changed to RUNNING!\n");
		setBacklightColour(GREEN_COLOR);
		device_mode = RUNNING;
	}

	while (digitalRead(MODE_PIN) == LOW) {	// Wait for release
		delay(1);
	}

}

/**
 * up_temp_button()
 * @brief Handles tmp up button functionality
 */
static inline void up_temp_button()
{
	if (digitalRead(UP_TMP_PIN) == HIGH) {
		return;
	}
	while (digitalRead(UP_TMP_PIN) == LOW) {
		set_point += 0.01;
		delay(1);
	}
}

/**
 * down_temp_button()
 * @brief Handles tmp down button functionality
 */
static inline void down_temp_button()
{
	if (digitalRead(DN_TMP_PIN) == HIGH) {
		return;
	}

	while (digitalRead(DN_TMP_PIN) == LOW) {
		set_point -= 0.01;
		delay(1);
	}
}

/**
 * open_relay()
 * @brief Opens relay at RELAY_PIN
 */ 
static inline void open_relay()
{
	digitalWrite(RELAY_PIN, HIGH);
}

/**
 * close_relay()
 * @brief Closes relay at RELAY_PIN
 */ 
static inline void close_relay()
{
	digitalWrite(RELAY_PIN, LOW);
}

/**
 * manage_relay(float actual_temp)
 * @brief manages relay
 * @param actual_temp
 */ 
static inline void manage_relay(float actual_temp) {
	
	if (actual_temp >= set_point) {
		open_relay();
	} else {
		close_relay();
	}
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
 * get_am2302_data(char *top_buffer)
 * @brief Retrieves DS data
 * @param top_buffer
 */
static inline void get_am2302_data(char *top_buffer)
{
	// Retrieve out the AM2302 sensor data
	FILE *am_file = NULL;
	am_file = popen(AM2302_CMD, "r");

	fgets(top_buffer, SIZE_OF_LCD, am_file);
	pclose(am_file);

	printf("AM2302  output:%s\n", top_buffer);
}

/**
 * get_ds_data(char *bottom_buffer, float *actual_temp)
 * @brief Retrieves DS data
 * @param bottom_buffer
 * @param actual_temp
 */
static inline void get_ds_data(char *bottom_buffer, float *actual_temp)
{
	// Retrieve out DS probe sensor data
	char probe_buffer[1024] = { 0 };
	int len = 0;
	int i = 0;
	
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
	(*actual_temp) = (1.8 * (atof(tmp_buffer) / 1000)) + 32;
}

/**
 *  build_bottom_string(char *top_buffer, char *bottom_buffer, float actual_temp)
 *  @brief Builds bottom string to be displayed on LCD
 *  @param top_buffer
 *  @param bottom_buffer
 *  @param actual_temp
 */
static inline void build_bottom_string(char *top_buffer, char *bottom_buffer, float actual_temp)
{
	snprintf(bottom_buffer, SIZE_OF_LCD, "%3.2f-%3.2f ", actual_temp, set_point);

	if (device_mode == RUNNING) {
		strcat(bottom_buffer, "Ou");
	} else {
		strcat(bottom_buffer, "No");
	}

}

/**
 * check_time(time_t old_time)
 * @brief Checks for time
 */ 
static int check_time(time_t *old_time) {
	
	// If this is the first time running, time will be 0;
	if(old_time == 0) {
		return (1);
	}
	
	// Get the current time
	time_t cur_time = time(0);
	
	// Compare if difference in time is greater than 1 second
    if ((cur_time - old_time) > 1) {
		(*old_time) = cur_time;
		return (1);
	}
	
	return (0);
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
	int cols = SIZE_OF_LCD;
	button_s b_state = { 0 };
	b_state.waitForRelease = FALSE;
	b_state.colour = RED_COLOR;
	set_point = DEFAULT_SETPOINT;
	float actual_temp = 0;
	time_t cur_time = 0;
	
	char top_buffer[SIZE_OF_LCD] = { 0 };
	char bottom_buffer[SIZE_OF_LCD] = { 0 };	//"123.45-123.45 {Ou,No}"

	printf("\nMaplePI GPIO/Monitoring App\n\n");
	printf("Author: Ronnie brash (ron.brash@gmail.com)\n");
	printf("------------------------------------------\n");

	// Check arguments
	if (argc != 2) {
		return usage(argv[0]);
	}
	
	b_state.colour = atoi(argv[1]);

	wiringPiSetup();
	
	// Setup LCD
	mcp23017Setup(AF_BASE, 0x20);
	adafruitLCDSetup(b_state.colour);

	// Set device mode to not running
	device_mode = NOT_RUNNING;

	// Initialize the user interface buttons (not on plate)
	setup_buttons();

	for (;;) {
		// Check GPIO/buttons for input
		mode_button();
		up_temp_button();
		down_temp_button();
		
		// Open/Close Relay
		if(device_mode == RUNNING) {
			manage_relay(actual_temp);
		}
		
		// Check time
		if(check_time(&last_time) > 0) {
			// Retrieve sensor data through pipes
			get_am2302_data(&top_buffer);
			get_ds_data(&bottom_buffer, &actual_temp);
				
		} 
		
		// Build the final string (obviously not efficient)
		build_bottom_string(&top_buffer, &bottom_buffer, actual_temp);
		
		// Output AM2302 data onto the top row of the LCD
		lcdPosition(lcdHandle, 0, 0);
		lcdPuts(lcdHandle, top_buffer);

		// Output DS data & set_point onto the bottom row of the LCD
		lcdPosition(lcdHandle, 0, 1);
		lcdPuts(lcdHandle, bottom_buffer);

		printf("\nDisplay will show -----------------------\n");
		printf("%s", top_buffer);
		printf("%s", bottom_buffer);
		printf("\n-----------------------------------------\n");

	}

	return 0;
}
