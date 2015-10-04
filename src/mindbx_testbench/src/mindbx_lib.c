/**
 * @file cerebral_app_lib.c
 * @author Frederic Simard, Billy Labelle atlants embedded
 * @brief */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "wiringPi.h"

#include "pthread.h"

#define	LED_RED_P1  0
#define	LED_GREEN,P1  1
#define	LED_RED_P2	2
#define	LED_GREEN_P2  		3
#define	TEST_BUTTON		4

#define FIRST_UNUSED_GPIO 5
#define TOT_NB_GPIO 8


static char ls_flashing_latch = 0x00;
static int ls_flashing_half_period = 0;
static char ls_flashing_color_on = 0x00;
static char ls_flashing_color_off = 0x00;

void *led_strip_flash(void *param);

/**
 * setup_mindbx(void)
 * @brief setups wiringpi and the gpios associated with the mindbx
 */
void setup_mindbx(void)
{
	int i;

	/*check if root */
	if (geteuid() != 0) {
		fprintf(stderr, "mindbx need to be root to run\n");
		exit(0);
	}

	/*setup the wiring pi */
	if (wiringPiSetup() == -1)
		exit(1);

	/*inform user */
	printf("Wiringpi setup proceeding\n");
	fflush(stdout);

	/*define the pins functions */
	pinMode(LED_RED_P1, OUTPUT);
	pinMode(LED_GREEN_P1, OUTPUT);
	pinMode(LED_RED_P2, OUTPUT);
	pinMode(LED_GREEN_P2, OUTPUT);
	pinMode(TEST_BUTTON, INPUT);
	

	/*set default output values */
	set_led_strip_color(OFF);

	/*set other pins as output */
	for (i = FIRST_UNUSED_GPIO; i < TOT_NB_GPIO; i++) {
		pinMode(i, OUTPUT);
		digitalWrite(i, LOW);
	}

	/*inform user */
	printf("Wiringpi setup completed\n");
}
/*
/**
 * wait_for_coin_insertion(void)
 * @brief blocking call that waits for coin insertion
 */
/*void wait_for_coin_insertion(void)
{
	printf("Waiting for a coin... ");
	fflush(stdout);

	/*wait for coin acceptor to be activated */
	/*while (digitalRead(COIN_ACCEPTOR) == HIGH && digitalRead(TEST_BUTTON) == HIGH)
		delay(50);

	/*inform user */
	/*printf("Coin accepted!\n");
}
*/
/**
 * wait_for_test_button(void)
 * @brief blocking call that waits for the test button
 */
void wait_for_test_button(void)
{
	printf("Waiting for a test button... ");
	fflush(stdout);

	/*wait for test button to be activated */
	while (digitalRead(TEST_BUTTON) == HIGH)
		delay(100);

	/*inform user */
	printf("Test button pressed!\n");
}

void set_player1_green()
{
	digitalWrite(LED_GREEN_P1, 1);
	
}
void reset_player1_green()
{
	digitalWrite(LED_GREEN_P1,0);
	
}
void set_player2_green()
{
	digitalWrite(LED_GREEN_P2,1);
	
}
void reset_player2_green()
{
	digitalWrite(LED_GREEN_P2,0);
	
}
void set_player1_red()
{
	digitalWrite(LED_RED_P1,1);
	
}
void reset_player1_red()
{
	digitalWrite(LED_RED_P1,0);
	
}
void set_player2_red()
{
	digitalWrite(LED_RED_P2,1);
	
}
void reset_player2_red()
{
	digitalWrite(LED_RED_P2,0);
	
}

/**
 * open_door(void)
 * @brief push open the door of the mindbx. Solenoid stays activated for 5 seconds
 */
 
/*void open_door(void)
{

	/*open the door */
	/*digitalWrite(SOLENOID, HIGH);
	printf("Door opened!\n");
	delay(5000);
	digitalWrite(SOLENOID, LOW);
	printf("Can close door now\n");

}
*/
/**
 * set_led_strip_flash_state(void)
 * @brief 
 * @param 
 */
void set_led_strip_flash_state(char rgb_on, char rgb_off, int half_period)
{

	pthread_t thread;
	pthread_attr_t attr;

	ls_flashing_half_period = half_period;
	ls_flashing_color_on = rgb_on;
	ls_flashing_color_off = rgb_off;

	if (!ls_flashing_latch) {
		ls_flashing_latch = 0x01;

		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		pthread_create(&thread, &attr, led_strip_flash, (void *)NULL);
		pthread_attr_destroy(&attr);
	}
}

/**
 * set_led_strip_flash_state(void)
 * @brief 
 * @param 
 */
void *led_strip_flash(void *param)
{

	printf("Starts LED flashing\n");

	while (ls_flashing_latch) {
		set_led_strip_color(ls_flashing_color_on);
		delay(ls_flashing_half_period);
		set_led_strip_color(ls_flashing_color_off);
		delay(ls_flashing_half_period);
	}

	return NULL;
}

/**
 * reset_led_strip_flash_state(void)
 * @brief 
 */
void reset_led_strip_flash_state(void)
{
	printf("Stops LED flashing\n");
	ls_flashing_latch = 0x00;
}

/***********************************************************************/
void set_player1_flash_red(){
	
	while(ls_flashing_latch){
		digitalWrite(LED_RED_P1,HIGH);
		delay(ls_flashing_half_period);
		digitalWrite(LED_RED_P1,LOW);
		delay(ls_flashing_half_period);		
		}
	
	}
/***********************************************************************/
/***********************************************************************/
void set_player1_flash_green(){
	
	while(ls_flashing_latch){
		digitalWrite(LED_GREEN_P1,HIGH);
		delay(ls_flashing_half_period);
		digitalWrite(LED_GREEN_P1,LOW);
		delay(ls_flashing_half_period);		
		}
	
	}
/***********************************************************************/
/***********************************************************************/
void set_player2_flash_red(){
	
	while(ls_flashing_latch){
		digitalWrite(LED_RED_P2,HIGH);
		delay(ls_flashing_half_period);
		digitalWrite(LED_RED_P2,LOW);
		delay(ls_flashing_half_period);		
		}
	
	}
/***********************************************************************/
/***********************************************************************/
void set_player2_flash_green(){
	
	while(ls_flashing_latch){
		digitalWrite(LED_GREEN_P1,HIGH);
		delay(ls_flashing_half_period);
		digitalWrite(LED_GREEN_P1,LOW);
		delay(ls_flashing_half_period);		
		}
	
	}
/***********************************************************************/
