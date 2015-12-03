/**
 * @file mindbx_lib.c
 * @author Frederic Simard (frederic.simard.1@outlook.com) | atlants embedded
 * @brief Libraries of functions to interact with the mindbx hardware.
 *        Functions allow for the monitoring of the coin acceptor and test button, 
 *        and the control over LED strip and the solenoid that push the door open.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <pthread.h>

#include "mindbx_lib.h"

#define	LED_STRIP_RED	0
#define	LED_STRIP_GREEN	1
#define	LED_STRIP_BLUE	2
#define	SOLENOID		3
#define	COIN_ACCEPTOR	4
#define	TEST_BUTTON		5
#define	DEMO_SELECTOR	6

static char ls_flashing_latch = 0x00; 
static int ls_flashing_half_period = 0;
static char ls_flashing_color_on = 0x00;
static char ls_flashing_color_off = 0x00; 

void* led_strip_flash(void* param);

/**
 * setup_mindbx(void)
 * @brief setups wiringpi and the gpios associated with the mindbx
 */
void setup_mindbx(void)
{
	  int i ;

	  /*check if root*/
	  if (geteuid () != 0)
	  {
			fprintf (stderr, "mindbx need to be root to run\n") ;
			exit (0) ;
	  }

	  /*setup the wiring pi*/
	  if (wiringPiSetup () == -1)
			exit (1) ;
	  
	  /*define the pins functions*/
	  pinMode(LED_STRIP_RED, OUTPUT);
	  pinMode(LED_STRIP_GREEN, OUTPUT);
	  pinMode(LED_STRIP_BLUE, OUTPUT);
	  pinMode(SOLENOID, OUTPUT);
	  pinMode(TEST_BUTTON, INPUT);
	  pinMode(COIN_ACCEPTOR, INPUT);
	  pinMode(DEMO_SELECTOR, INPUT);
	  
	  /*set default output values*/
	  digitalWrite (SOLENOID, 0);
	  set_led_strip_color(OFF);
}

/**
 * wait_for_coin_insertion(void)
 * @brief blocking call that waits for coin insertion
 */
void wait_for_coin_insertion(void)
{
	  printf("Waiting for a coin... "); 
	  fflush(stdout);
	  
	  /*wait for coin acceptor or test button to be activated*/
	  while (digitalRead(COIN_ACCEPTOR)==HIGH && digitalRead(TEST_BUTTON)==HIGH)
		delay(50);
	  
	  /*inform user*/
	  printf("Coin accepted!\n");
}

/**
 * wait_for_test_button(void)
 * @brief blocking call that waits for the test button
 */
void wait_for_test_button(void)
{
	  printf("Waiting for a test button... "); 
	  fflush(stdout);
	  
	  /*wait for test button to be activated*/
	  while (digitalRead(TEST_BUTTON)==HIGH)
		delay(50);
	  
	  /*inform user*/
	  printf("Test button pressed!\n");
}

char get_selector_state(void){
	if(digitalRead(DEMO_SELECTOR)==HIGH){
		return 0x01;
	}else{
		return 0x00;
	}	
}


/**
 * set_led_strip_color(void)
 * @brief sets the value of the PWM driving the led strip
 * @param intensity, {0,1024}
 */
void set_led_strip_color(char rgb)
{
  digitalWrite(LED_STRIP_RED, rgb & RED ? 1 : 0);
  digitalWrite(LED_STRIP_GREEN, rgb & GREEN ? 1 : 0);
  digitalWrite(LED_STRIP_BLUE, rgb & BLUE ? 1 : 0);
}

/**
 * open_door(void)
 * @brief push open the door of the mindbx. Solenoid stays activated for 5 seconds
 */
void open_door(void)
{

  /*open the door*/
  digitalWrite(SOLENOID, 1);
  printf ("Door opened!\n");
  delay(5000);
  digitalWrite(SOLENOID, 0);

}

/**
 * set_led_strip_flash_state(void)
 * @brief 
 * @param 
 */
void set_led_strip_flash_state(char rgb_on, char rgb_off, int half_period){

	pthread_t thread;
	pthread_attr_t attr;
	
	ls_flashing_half_period = half_period;
	ls_flashing_color_on = rgb_on;
	ls_flashing_color_off = rgb_off;
	
	if(!ls_flashing_latch){
		ls_flashing_latch = 0x01;
		
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		pthread_create(&thread, &attr, led_strip_flash, (void*) NULL); 
		pthread_attr_destroy(&attr);
	}
}

/**
 * set_led_strip_flash_state(void)
 * @brief 
 * @param 
 */
void* led_strip_flash(void* param __attribute__ ((unused))){

	printf ("Starts LED flashing\n");
  
	while(ls_flashing_latch){ 
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
void reset_led_strip_flash_state(void){
	printf ("Stops LED flashing\n");
	ls_flashing_latch = 0x00;
}
