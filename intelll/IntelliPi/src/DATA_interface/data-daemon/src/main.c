/**
 * @file main.c
 * @author Ron Brash (ron.brash@gmail.com), Frederic Simard (frederic.simard.1@outlook.com) | Atlants Embedded 2015
 * @brief Handles the data interface layer which acts as an abstraction
 * interface for BLE/Bluetooth (or other) data and converts data into a usable format
 * for other applications. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include "main.h"
#include "socket.h"
#include "serial.h"
#include "app_signal.h"
#include "hardware.h"
#include "data_output.h"
#include "xml.h"
#include "ipc_status_comm.h"
#include "debug.h"

#define CONFIG_NAME "config/data_config.xml"

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
 * main()
 * @brief Application main running loop
 */
int main(int argc __attribute__ ((unused)), char **argv __attribute__ ((unused)))
{
	param_t param_ptr = { 0 };
	pthread_t readT, writeT;
	int iret1 __attribute__ ((unused)), iret2 __attribute__ ((unused)), ret = 0, attempts = 0;

	data_out_options_t data_output_options;

	// Set up ctrl c signal handler
	(void)signal(SIGINT, ctrl_c_handler);

	/*read the config from the xml*/
	appconfig_t *config = (appconfig_t *) xml_initialize(which_config(argc, argv));
	if (config == NULL) {
		printf("Error initializing XML configuration\n");
		return (-1);
	}
	
	/*init inter-process status communication channel*/
	ipc_comm_init();

	/*init the hardware*/
	if (init_hardware((char *)config->device) < 0) {
		printf("Error initializing hardware");
		return (-1);
	}

	printf("Remote target: %s \n", get_appconfig()->remote_addr);
	param_ptr.ptr = (char *)get_appconfig()->remote_addr;

	printf("Attempts: %d\n", get_appconfig()->conn_attempts);
	printf("Keep alive: %d seconds: %d\n", get_appconfig()->keep_alive, get_appconfig()->keep_time);
	
	/*Copy info from xml to dataoutput options*/
	data_output_options.shm_key = config->shm_key;
	data_output_options.sem_key = config->sem_key;
	data_output_options.nb_data_channels = config->nb_data_channels*sizeof(float); /*adjusted for data type (patch)*/
	data_output_options.window_size = config->window_size;
	data_output_options.nb_pages = config->nb_pages;
	data_output_options.page_size = data_output_options.window_size*data_output_options.nb_data_channels;
	data_output_options.buffer_size = data_output_options.page_size*data_output_options.nb_pages;
	
	
	/*init the data output*/
	if (init_data_output((char)config->output_format,data_output_options)==EXIT_FAILURE){
		printf("Error initializing data output");
		return (-1);
	}
	
	printf("Data output initialized\n");

	for (;;) {
		attempts++;
		printf("Connection attempt: %u\n", attempts);
		
		if ((ret = DEVICE_CONNECTION_FC(&param_ptr)) == 0){
			break;
		}
		sleep(1);
	}
	
	/*tell app that hardware is present*/
	ipc_tell_harware_is_on();
	
	if (ret == 0) {
		printf("Connected to device\n");

		/*init the function that picks up the bluetooth packets*/
		iret1 = pthread_create(&readT, NULL, (void *)_RECV_PKT_FC, NULL);

		/*if keep_alive*/
		if (get_appconfig()->keep_alive) {
			/*init the function that implements the watchdog*/
			iret2 = pthread_create(&writeT, NULL, (void *)_KEEP_ALIVE_FC, NULL);
			pthread_join(writeT, NULL);
		}
		pthread_join(readT, NULL);

	} else {
		printf("Unable to connect to hardware\n");
	}

	ipc_comm_cleanup();
	DEVICE_CLEANUP_FC();
	TERMINATE_DATA_OUTPUT_FC();
	return 0;
}
