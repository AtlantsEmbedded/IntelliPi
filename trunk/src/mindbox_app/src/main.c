

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>

#include "feature_processing.h"
#include "ipc_status_comm.h"
#include "feature_input.h"
#include "mindbx_lib.h"
#include "xml.h"

appconfig_t* app_config;


#define CONFIG_NAME "config/application_config.xml"

/**
 * main(int argc, char **argv)
 * @brief test the mindbx_lib
 */
int main(int argc, char **argv){
	
	int i;
	
	/*read the xml*/
	app_config = xml_initialize(CONFIG_NAME);
	
	/*configure the feature input*/
	init_feature_input(app_config->feature_source);
	
	/*configure the inter-process communication channel*/
	ipc_comm_init();
	
	/*configure the mind box*/
	//setup_mindbx();
	
	/*if required, wait for eeg hardware to be present*/
	if(app_config->eeg_hardware_required){
		/*set the LED signal*/
		printf("wait for harware\n");
		//set_led_strip_flash_state(BLUE,OFF,750);
		/*and wait*/
		ipc_wait_for_harware();
	}
	
	/*while application is alive*/
	while(1){
	
		/*wait for a coin*/
		//wait_for_coin_insertion();
		
		printf("wait for data in\n");
	
		/*build training set*/
		READ_FEAT_FC(NULL);
	
		sleep(5);
	
		/*run the test*/
	
	
		/*apply the result of the test*/
		
		
	}
	
	
	
	ipc_comm_cleanup();
	
	
	
	
	
	/*wait for test button*/
	wait_for_test_button();
	
	/*flash the LEDs GREEN and YELLOW faster*/
	set_led_strip_flash_state(GREEN,YELLOW,150);
	
	/*delay to prevent double detection of test button*/
	delay(3000);
	
	/*wait for coin acceptor*/
	wait_for_coin_insertion();
	
	/*turn off led flashing*/
	reset_led_strip_flash_state();
	
	/*turn off the LED strip*/
	set_led_strip_color(OFF);

	/*and open the door 5 times*/
	for(i=0;i<5;i++){
		open_door();
		delay(2000);
	}
	
	exit(0);
}
