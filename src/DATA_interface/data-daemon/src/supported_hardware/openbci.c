/**
 * @file muse.c
 * @author Ron Brash (ron.brash@gmail.com)
 * @brief Handles all OpenBCI related function pointers, however, currently
 * only supports the non-research edition 
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include "serial.h"
#include "hardware.h"
#include "debug.h"
#include "main.h"
#include "openbci.h"

/**
 * openbci_init_hardware()
 * @brief Initializes muse hardware related variables
 */
int openbci_init_hardware(void *param __attribute__ ((unused)))
{
	return (0);
}

/**
 * openbci_init_hardware()
 * @brief Initializes muse hardware related variables
 */
int openbci_connect_dev(void *param)
{
	param_t *param_ptr = (param_t *) param;
	setup_serial((unsigned char *)param_ptr->ptr);
	return (0);
}

/**
 * openbci_cleanup()
 * @brief Openbci cleanup function
 */
int openbci_cleanup(void *param __attribute__ ((unused)))
{
	close_serial();
	return (0);
}

/** 
 * openbci_translate_pkt
 * @brief translate MUSE packet
 */
int openbci_translate_pkt(void *param)
{
	param_t *param_ptr = (param_t *) param;

	// Do some things to print out the packet
	return (0);
}

/**
 * send_keep_alive_pkt(void)
 * @brief Sends a keep alive repeatedly while sleeping for the required 
 * time-out period.
 */
int openbci_send_keep_alive_pkt(void *param __attribute__ ((unused)))
{
	// Not used
	return (0);
}

/**
 * openbci_send_pkt()
 * @brief Sends a keep alive repeatedly while sleeping for the required 
 * time-out period.
 * @param param
 */
int openbci_send_pkt(void *param)
{
	param_t *param_ptr = (param_t *) param;

	write(get_serial_fd(), param_ptr->ptr, param_ptr->len);

	return (0);
}

/**
 * openbci_process_pkt()
 * @brief Processes the packet
 * @param param
 */
int openbci_process_pkt(void *param)
{

	// Uncompressed or raw at this point
	param_t *param_ptr = (param_t *) param;

	fprintf(stdout, "Bytes read = %d\n", param_ptr->len);

	hexdump((unsigned char *)param_ptr->ptr, param_ptr->len);

	if (_TRANS_PKT_FC) {
		TRANS_PKT_FC(param);
	}

	return (0);
}

/**
 * openbci_read_pkt()
 * @brief Reads incoming packets from the socket
 */
int openbci_read_pkt(void *param __attribute__ ((unused)))
{
	int bytes_read = 0;
	char buf[BUFSIZE] = { 0 };
	param_t param_start_transmission = { OPENBCI_START_TRANSMISSION, 1 };
	param_t param_translate_pkt = { 0 };

	openbci_send_pkt(&param_start_transmission);

	do {
		memset(buf, 0, BUFSIZE);
		bytes_read = read(get_serial_fd(), buf, 33);

		if (bytes_read <= 0) {
			fprintf(stdout, "Error reading socket: %d\n", bytes_read);
			continue;
		}

		param_translate_pkt.ptr = buf;
		param_translate_pkt.len = 33;
		PROCESS_PKT_FC(&param_translate_pkt);

	} while (1);
	return (0);
}
