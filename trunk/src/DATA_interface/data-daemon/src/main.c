/**
 * @file main.c
 * @author Ron Brash (ron.brash@gmail.com) | Atom Embedded
 * @brief Handles the data interface layer which acts as an abstraction
 * interface for BLE/Bluetooth data and converts data into a usable format
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

int main(int argc, char **argv)
{
	(void)signal(SIGINT, ctrl_c_handler);

	pthread_t readT, writeT;
	int iret1 __attribute__ ((unused)), iret2 __attribute__ ((unused));

	uint8_t attempts = CONN_ATTEMPTS;
	int ret = 0;
	char *remote_addr = NULL;

	if (argc == 1) {
		remote_addr = REFERENCE_MUSE_ADDR;
	} else {
		remote_addr = argv[1];
	}

	printf("Remote target: %s\n",remote_addr);
	for (attempts = 3; attempts > 0; attempts--) {
		printf("Connection attempt: %u\n", attempts);
		ret = setup_socket(remote_addr);
		if (ret == 0) {
			break;
		}
	}
	if (attempts == 0) {
		printf("Unable to connect to the device\n");
		return (-1);
	}

	if (ret == 0) {
		printf("Connected to device\n");

		iret1 = pthread_create(&readT, NULL, (void *)_RECV_PKT_FC, NULL);
		iret2 = pthread_create(&writeT, NULL, (void *)_SEND_PKT_FC, NULL);

		pthread_join(readT, NULL);
		pthread_join(writeT, NULL);

	} else {
		printf("Unable to connect to hardware\n");
	}

	close_sockets();
	return 0;
}
