
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <pthread.h>
#include <ctype.h>
#include <math.h>

#include <wiringPi.h>
#include <softTone.h>
#include <buzzer_lib.h>

#define BASE_FREQ 500

/*Pins used (wiringPi numbers)*/
const int BUZZER_PIN = 1;

/*defines the frequency scale*/
/*settings are for a log range between 500 and 1000Hz*/
static float exp_base = 1.065;
static float baseline_freq = 500;

static char beep_mode_latch = 0x00; 
static int beep_mode_half_period = 0;
static unsigned char beep_mode_state_a = 0x00;
static unsigned char beep_mode_state_b = 0x00; 

void* beep_mode(void* param);

/**
 * int setup_buzzer_lib(void)
 * @brief sets up the buzzer
 * @return 0 for success, -1 for error
 */
int setup_buzzer_lib(void){
	
	/*setup wiringPi*/
	wiringPiSetup();
	
	/*create a buzzer and set it off*/
	softToneCreate(BUZZER_PIN);
	turn_off_beeper();
	
	/*Set scheduling priority*/
	piHiPri(99);
	
	return EXIT_SUCCESS;
}

/**
 * int set_buzzer_state(int state)
 * @brief Sets the buzzer to a level between 0 and 100
 */
int set_buzzer_state(int state){

	if(state>100){
		state = 100;
		softToneWrite(BUZZER_PIN, state*5+baseline_freq);
	}else if(state<=0){
		softToneWrite(BUZZER_PIN, 0);
	}else{
		softToneWrite(BUZZER_PIN, state*5+baseline_freq);
	}
	return EXIT_SUCCESS;
}

/**
 * set_beep_mode()
 * @brief Sets the beeper on an intermittent mode
 */
int set_beep_mode(unsigned char state_a, unsigned char state_b, int half_period){
	
	pthread_t thread;
	pthread_attr_t attr;
	
	beep_mode_half_period = half_period;
	beep_mode_state_a = state_a;
	beep_mode_state_b = state_b;
	
	if(!beep_mode_latch){
		beep_mode_latch = 0x01;
		
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		pthread_create(&thread, &attr, beep_mode, (void*) NULL); 
		pthread_attr_destroy(&attr);
	}
	
	return EXIT_SUCCESS;
}


/**
 * set_led_strip_flash_state(void)
 * @brief 
 * @param 
 */
void* beep_mode(void* param __attribute__ ((unused))){

	printf ("Starts LED flashing\n");
  
	while(beep_mode_latch){ 
		set_buzzer_state(beep_mode_state_a); 
		delay(beep_mode_half_period);
		set_buzzer_state(beep_mode_state_b); 
		delay(beep_mode_half_period);
	}
	
	return NULL;
}

/**
 * turn_off_beep()
 * @brief Turn off beeper
 */
void turn_off_beeper(){
	beep_mode_latch = 0x00;
	softToneWrite(BUZZER_PIN, 0);
}
