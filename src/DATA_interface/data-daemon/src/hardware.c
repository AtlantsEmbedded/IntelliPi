/**
 * @file hardware.c
 * @brief Initializes generic hardware interface which will assign
 * functions to function pointers to provide a reusable interface 
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include "socket.h"
#include "hardware.h"
#include "debug.h"
#include "main.h"
#include "muse.h"
#include "openbci.h"

/**
 * init_hardware()
 * @brief Setup function pointers
 * @param hardware_type
 * @return -1 for unknown type, 0 for known/success
 */
int init_hardware(char *hardware_type)
{

	_INIT_HARDWARE_FC = NULL;
	_KEEP_ALIVE_FC = NULL;
	_SEND_PKT_FC = NULL;
	_RECV_PKT_FC = NULL;
	_TRANS_PKT_FC = NULL;

	printf("Hardware type: %s\n",hardware_type);
	if (strcmp(hardware_type, "MUSE") == 0) {
		
		_INIT_HARDWARE_FC = &muse_init_hardware;
		_KEEP_ALIVE_FC = &muse_send_keep_alive_pkt;
		_SEND_PKT_FC = &muse_send_pkt;
		_RECV_PKT_FC = &muse_read_pkt;
		_TRANS_PKT_FC = &muse_translate_pkt;

	} else if (strcmp(hardware_type, "OPENBCI") == 0) {
		// OpenBCI stuff (there are several versions though)
	} else {
		fprintf(stderr, "Unknown hardware type\n");
		return (-1);
	}

	return (0);
}
