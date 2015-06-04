/**
 * @file main.c
 * @author Ron Brash (ron.brash@gmail.com) | Atom Embedded
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
#include "app_signal.h"
#include "hardware.h"
#include "xml.h"
#include "debug.h"

#define CONFIG_NAME "config/data_config.xml"

int main(int argc __attribute__ ((unused)), char **argv __attribute__ ((unused)))
{
	(void)signal(SIGINT, ctrl_c_handler);

	pthread_t readT, writeT;
	int iret1 __attribute__ ((unused)), iret2 __attribute__ ((unused));

	uint8_t attempts = CONN_ATTEMPTS;
	int ret = 0;

	appconfig_t *config = (appconfig_t *) xml_initialize(CONFIG_NAME);
	if (config == NULL) {
		return (-1);
	}

	if (init_hardware(config->device) < 0) {
		return (-1);
	}

	printf("Remote target: %s\n", config->remote_addr);
	for (attempts = 1; attempts >= config->conn_attempts; attempts++) {
		printf("Connection attempt: %u\n", attempts);
		ret = setup_socket(config->remote_addr);
		if (ret == 0) {
			break;
		}
		sleep(1);
	}
	if (attempts >= config->conn_attempts) {
		printf("Unable to connect to the device\n");
		return (-1);
	}

	if (ret == 0) {
		printf("Connected to device\n");
		
		iret1 = pthread_create(&readT, NULL, (void *)_RECV_PKT_FC, NULL);
		

		if (get_appconfig()->keep_alive) {
			printf("fuck off");
			iret2 = pthread_create(&writeT, NULL, (void *)_KEEP_ALIVE_FC, NULL);
			pthread_join(writeT, NULL);
			exit(10);
		}
		pthread_join(readT, NULL);

	} else {
		printf("Unable to connect to hardware\n");
	}

	close_sockets();
	return 0;
}
