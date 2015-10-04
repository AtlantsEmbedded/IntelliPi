

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>

#include "mindbx_lib.h"

/**
 * main(int argc, char **argv)
 * @brief test the mindbx_lib
 */
int main(int argc, char **argv){
	
	int i;
	
	/*configure the mind box*/
	setup_mindbx();
	 
	/*Test the outputs of the LEDs*/
	 
	 /*Player 1 LEDs*/
	 set_player1_green();
	 delay(1000);
	 reset_player1_green();
	 set_player1_red();
	  delay(1000);
	 reset_player1_red();
	 
	 
	 /*Player 2 LEDs*/
	 set_player2_green();
	  delay(1000);
	 reset_player2_green();
	 set_player2_red();
	  delay(1000);
	 reset_player2_red();
	 
	 
	 /*Test the flashing leds for player 1*/
	 set_player1_flash_green();
	 set_player1_flash_red();
	 
	 /*Test the flashing leds for player 2*/
	 set_player2_flash_green();
	 set_player2_flash_red();
	 
	 /*Test the start button*/
	 
	 wait_for_test_button();
	 
	 
	 
	 
	/*flash the LEDs RED and BLUE*/
	//set_led_strip_flash_state(RED,BLUE,500);
	
	/*wait for test button*/
	//wait_for_test_button();
	
	/*flash the LEDs GREEN and YELLOW faster*/
	//set_led_strip_flash_state(GREEN,YELLOW,150);
	
	/*wait for coin acceptor*/
	//wait_for_coin_insertion();
	
	/*turn off led flashing*/
	//reset_led_strip_flash_state();
	
	/*turn off the LED strip*/
	//set_led_strip_color(OFF);

	/*and open the door 5 times*/
	/*for(i=0;i<5;i++){
		open_door();
		delay(2000);
	}
*/
	exit(0);
}
