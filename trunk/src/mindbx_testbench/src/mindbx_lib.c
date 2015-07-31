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

#include "mindbx_lib.h"

#define	SOLENOID		0
#define	LED_STRIP		1
#define	TEST_BUTTON		2
#define	COIN_ACCEPTOR	3

#define FIRST_UNUSED_GPIO 4
#define TOT_NB_GPIO 8


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

	  /*inform user*/
	  printf("Wiringpi setup proceeding"); 
	  fflush(stdout);
	  
	  /*define the pins functions*/
	  pinMode(SOLENOID, OUTPUT);
	  pinMode(LED_STRIP, PWM_OUTPUT);
	  pinMode(TEST_BUTTON, INPUT);
	  pinMode(COIN_ACCEPTOR, INPUT);
	  
	  /*set default output values*/
	  digitalWrite (SOLENOID, 0);
	  set_led_strip_intensity(0);

	  /*set other pins as output*/
	  for (i=FIRST_UNUSED_GPIO ; i<TOT_NB_GPIO ; i++)
	  {
			pinMode (i, OUTPUT);
			digitalWrite (i, 0);
	  }

	  /*inform user*/
	  printf ("Wiringpi setup completed\n");
}

/**
 * wait_for_coin_insertion(void)
 * @brief blocking call that waits for coin insertion
 */
void wait_for_coin_insertion(void)
{
	  printf("Waiting for a coin... "); 
	  fflush(stdout);
	  
	  /*wait for coin acceptor to be activated*/
	  while (digitalRead(COIN_ACCEPTOR)==LOW)
		delay(100);
	  
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
	  while (digitalRead(TEST_BUTTON)==LOW)
		delay(100);
	  
	  /*inform user*/
	  printf("Test button pressed!\n");
}

/**
 * set_led_strip_intensity(void)
 * @brief sets the value of the PWM driving the led strip
 * @param intensity, {0,1024}
 */
void set_led_strip_intensity(int intensity)
{
  printf ("Set led PWM to: %i\n",intensity); 
  fflush (stdout);

  pwmWrite(LED_STRIP, intensity);
}

/**
 * open_door(void)
 * @brief push open the door of the mindbx. Solenoid stays activated for 5 seconds
 */
void open_door(void)
{

  /*open the door*/
  digitalWrite(SOLENOID, 1);
  printf ("Door opened!\n") ;
  delay(5000);
  digitalWrite(SOLENOID, 0);
  printf ("Can close door now\n") ;

}
