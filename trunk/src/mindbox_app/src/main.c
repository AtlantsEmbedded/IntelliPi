

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>

#include "rwalk_process.h"
#include "feature_processing.h"
#include "ipc_status_comm.h"
#include "feature_input.h"
#include "mindbx_lib.h"
#include "xml.h"

appconfig_t* app_config;


#define CONFIG_NAME "config/application_config.xml"

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
	char winner = 0x00;
	double decision_var_value = 0.0;
	double threshold = 0.0;
	char test_running = 0x00;
	feat_proc_options_t feat_proc_opts;
	rwalk_options_t rwalk_opts;
	feature_t feature_vect;
	double* feature_array;
	
	/*read the xml*/
	app_config = xml_initialize(which_config(argc, argv));
	
	threshold = app_config->threshold;
	
	/*configure the feature input*/
	init_feature_input(app_config->feature_source);
	
	/*configure the inter-process communication channel*/
	ipc_comm_init();
	
	/*configure the mind box*/
	setup_mindbx();
	
	/*set LED strip to pairing mode*/
	set_led_strip_flash_state(WHITE, OFF, 1000);
	
	/*if required, wait for eeg hardware to be present*/
	if(app_config->eeg_hardware_required){
		ipc_wait_for_harware();
	}
	
	feat_proc_opts.nb_train_samples = app_config->training_set_size;
	feat_proc_opts.nb_features = app_config->nb_features;
	init_feat_processing(feat_proc_opts);
	
	rwalk_opts.drift_rate_std = app_config->noise_std_dev;
	rwalk_opts.dt = app_config->time_period;
	init_rwalk_process(rwalk_opts);
	
	feature_vect.nb_features = app_config->nb_features;
	feature_vect.ptr = (unsigned char*)malloc(sizeof(double)*feature_vect.nb_features);
	
	feature_array = (double*)feature_vect.ptr;
	
	/*while application is alive*/
	while(1){
	
		
		printf("check\n");
	
		/*set LED strip to wait mode*/
		set_led_strip_flash_state(PINK, OFF, 1000);
	
		/*wait for a coin*/
		wait_for_coin_insertion();
		
		/*set LED strip to train mode*/
		set_led_strip_flash_state(GREEN, OFF, 1000);
		
		/*wait 3 seconds*/
		sleep(3);
		
		/*build training set*/
		train_feat_processing();
		
		/*reset random walk process*/
		reset_rwalk_process();
	
		test_running = 0x01;
		
		/*set LED strip to test mode*/
		set_led_strip_flash_state(BLUE, RED, 1000);

		/*wait 3 seconds*/
		sleep(3);
		
		/*run the test*/
		while(test_running){
		
			/*get a normalized sample*/
			get_normalized_sample(&feature_vect);
			
			
			printf("Drift rate:%f\n",feature_array[0]);
			
			/*push it to to noisy integrator*/
			decision_var_value = iterate_rwalk_process(feature_array[0]);
			
			printf("DV:%f\n",decision_var_value);
			
			/*update flash frequency*/
			set_led_strip_flash_state(BLUE, RED, ((threshold-decision_var_value)/threshold)*1000);
		
			/*check if one of the stop conditions is met*/
			if(decision_var_value>threshold){
				test_running = 0x00;
			}
		}
		
		if(decision_var_value>threshold){
			/*if we have a winner*/
			/*open the door*/
			open_door();
		}
		
	}
	
	printf("check\n");
	
	ipc_comm_cleanup();
	reset_led_strip_flash_state();
	set_led_strip_color(OFF);
	
	free(feature_vect.ptr);
	
	exit(0);
}
