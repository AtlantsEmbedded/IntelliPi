
#define LED_STRIP_ENABLED 0

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <wiringPi.h>

#include "rwalk_process.h"
#include "feature_processing.h"
#include "ipc_status_comm.h"
#include "feature_input.h"
#include "xml.h"
#include "main.h"

#if LED_STRIP_ENABLED
	#include "led_strip_lib.h"
#endif

appconfig_t* app_config;

#define CONFIG_NAME "config/application_config.xml"

#define PLAYER_1_ENABLED 1
#define PLAYER_2_ENABLED 1

unsigned char app_running = 0x01;

void* train_player(void* param);
void* get_sample(void* param);

double player_1_acc_value = 5;
double player_2_acc_value = 5;

/**
 * which_config(int argc, char **argv)
 * @brief return which config to use
 * @param argc
 * @param argv
 * @return string of config
 */
inline char *which_config(int argc, char **argv)
{

	if (argc == 2) {
		return argv[1];
	} else {
		return CONFIG_NAME;
	}
}

/**
 * main(int argc, char **argv)
 * @brief test the mindbx_lib
 */
int main(int argc, char **argv){
	
	int i;
	feature_input_t feature_input_1;
	feature_input_t feature_input_2;
	ipc_comm_t ipc_comm_1;
	ipc_comm_t ipc_comm_2;
	feat_proc_t feature_proc_1;
	feat_proc_t feature_proc_2;
	
	int player_1_intensity;
	int player_2_intensity;
	
	unsigned char game_over = 0x00;
	
	pthread_t thread[2];
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		
	/*read the xml*/
	app_config = xml_initialize(which_config(argc, argv));
	
	
	/*configure the feature input - muse 1*/
#if PLAYER_1_ENABLED
	feature_input_1.shm_key=7804;
	feature_input_1.sem_key=1234;
	init_feature_input(app_config->feature_source, &feature_input_1);
#endif

#if PLAYER_2_ENABLED
	/*configure the feature input - muse 2*/
	feature_input_2.shm_key=4786;
	feature_input_2.sem_key=1728;
	init_feature_input(app_config->feature_source, &feature_input_2);
#endif
	
	/*configure the inter-process communication channel - muse 1 & muse 2*/
#if PLAYER_1_ENABLED
	ipc_comm_1.sem_key=1234;
	ipc_comm_init(&ipc_comm_1);
#endif

#if PLAYER_2_ENABLED
	ipc_comm_2.sem_key=1728;
	ipc_comm_init(&ipc_comm_2);
#endif	
	/*if required, wait for eeg hardware to be present - muse 1 & muse 2*/
	if(app_config->eeg_hardware_required){
#if PLAYER_1_ENABLED
		ipc_wait_for_harware(&ipc_comm_1);
#endif		

#if PLAYER_2_ENABLED
		ipc_wait_for_harware(&ipc_comm_2);
#endif
	}
	
	/*init feature processing - muse 1 & muse 2*/
#if PLAYER_1_ENABLED
	feature_proc_1.nb_train_samples = 20;
	feature_proc_1.feature_input = &feature_input_1;
	init_feat_processing(&feature_proc_1);
#endif

#if PLAYER_2_ENABLED
	feature_proc_2.nb_train_samples = 20;
	feature_proc_2.feature_input = &feature_input_2;
	init_feat_processing(&feature_proc_2);
#endif

	printf("check\n");
	
	while(1){

		/*wait for the go*/
		
		/*wait 3 seconds*/
		sleep(3);
		
		/*build training set - muse 1 & muse 2 (start a thread)*/
	#if PLAYER_1_ENABLED
		pthread_create(&thread[0], &attr, train_player, (void*) &feature_proc_1); 
	#endif

	#if PLAYER_2_ENABLED
		pthread_create(&thread[1], &attr, train_player, (void*) &feature_proc_2); 
	#endif

		/*wait for training over - muse 1 & muse 2 (join with the thread)*/
	#if PLAYER_1_ENABLED
		pthread_join(thread[0],NULL); 
	#endif
	#if PLAYER_2_ENABLED
		pthread_join(thread[1],NULL); 
	#endif

		printf("Training completed!\n");
		fflush(stdout); 
		
#if LED_STRIP_ENABLED
		/*start the led strip*/
		init_led_strip();
#endif
		
		/*wait 3 seconds*/
		sleep(3);
		
		player_1_acc_value = 5;
		player_2_acc_value = 5;
		game_over = 0;

		/*run the test*/
		while(!game_over){
		
			/*get a normalized sample - muse 1 & muse 2*/
	#if PLAYER_1_ENABLED
			pthread_create(&thread[0], &attr, get_sample, (void*) &feature_proc_1); 
	#endif
	#if PLAYER_2_ENABLED
			pthread_create(&thread[1], &attr, get_sample, (void*) &feature_proc_2); 
	#endif
	#if PLAYER_1_ENABLED
			pthread_join(thread[0],NULL);
	#endif
	#if PLAYER_2_ENABLED
			pthread_join(thread[1],NULL);
	#endif
			
#if 0			
			/*show sample - muse 1 & muse 2*/
			printf("sample[%i]: ",i);
	#if PLAYER_1_ENABLED
			printf("\tP1:%.3f ",feature_proc_1.sample);
	#endif
	#if PLAYER_2_ENABLED
			printf("\tP2:%.3f",feature_proc_2.sample);
	#endif
#endif
			
			if(feature_proc_1.sample>0){
				player_1_acc_value += feature_proc_1.sample;
			}
			
			if(feature_proc_2.sample>0){
				player_2_acc_value += feature_proc_2.sample;
			}
			
			player_1_intensity = (10-feature_proc_1.sample/3*10 + 1);
			player_2_intensity = (10-feature_proc_2.sample/3*10 + 1);
			
			if(player_1_intensity>10){
				player_1_intensity=10;
			}else if(player_1_intensity<1){
				player_1_intensity=1;
			}
			
			if(player_2_intensity>10){
				player_2_intensity=10;
			}else if(player_2_intensity<1){
				player_2_intensity=1;
			}
			
#if LED_STRIP_ENABLED
			set_led_strip_values(player_1_intensity, player_2_intensity, player_1_acc_value/(player_1_acc_value+player_2_acc_value)*NB_LEDS);
#else
			printf("iteration[%i]: ",i);
			printf("\tP1:%i ",11-player_1_intensity);
			printf("\tP2:%i ",11-player_2_intensity);
			printf("\tloc:%.3f ",player_1_acc_value/(player_1_acc_value+player_2_acc_value));
#endif
			printf("\n");
			fflush(stdout); 
			
			/*compare accumulation*/
			if(player_1_acc_value/(player_1_acc_value+player_2_acc_value) < 0.1 || player_1_acc_value/(player_1_acc_value+player_2_acc_value) > 0.9){
				/*we have a winner*/
				game_over = 0x01;
				
				if(player_1_acc_value>player_2_acc_value){
					printf("Player 1 win!\n");
				}else{
					printf("Player 2 win!\n");
				}
			}
			
			if(player_1_acc_value>20){
				player_1_acc_value /=2;
				player_2_acc_value /=2;
			}
		}
	}
	
#if PLAYER_1_ENABLED
	ipc_comm_cleanup(&ipc_comm_1);
	clean_up_feat_processing(&feature_proc_1);
#endif
#if PLAYER_2_ENABLED
	ipc_comm_cleanup(&ipc_comm_2);
	clean_up_feat_processing(&feature_proc_2);
#endif
	pthread_attr_destroy(&attr);
	
	exit(0);
}

void* train_player(void* param){
	train_feat_processing((feat_proc_t*)param);	
}

void* get_sample(void* param){
	get_normalized_sample((feat_proc_t*)param);
}


void stop_application(void){
	app_running = 0x00;
}



