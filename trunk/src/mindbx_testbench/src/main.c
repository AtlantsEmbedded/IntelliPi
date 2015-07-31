

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
	
	/*open the door 5 times*/
	for(i=0;i<5;i++){
		open_door();
		delay(2000);
	}
	
	/*wait for test button*/
	wait_for_test_button();
	
	/*wait for coin acceptor*/
	wait_for_coin_insertion();
	
	/*play with led strip*/
	for(i=0;i<1024;i++){
		set_led_strip_intensity(i);
		delay(125);
	}
	
	exit(0);
}
