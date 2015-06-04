/**
 * @file muse.c
 * @author Ron Brash (ron.brash@gmail.com)
 * @brief Handles all MUSE related function pointers, however, currently
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

#include "socket.h"
#include "hardware.h"
#include "debug.h"
#include "main.h"
#include "muse.h"
#include "xml.h"

#define BUFSIZE 1024

static int seconds = 0;

/**
 * muse_init_hardware()
 * @brief Initializes muse hardware related variables
 */
int muse_init_hardware(void *param __attribute__ ((unused)))
{
	seconds = get_appconfig()->keep_time;
	return (0);
}

/** 
 * muse_translate_pkt
 * @brief translate MUSE packet
 */
int muse_translate_pkt(void *param)
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
int muse_send_keep_alive_pkt(void *param __attribute__ ((unused)))
{
	const char *msg = MUSE_KEEP_ALIVE;
	int status;

	do {
		status = send(get_socket_fd(), msg, 3, 0);
		sleep(seconds);
	} while (status > 0);
	return (0);
}

/**
 * muse_send_pkt()
 * @brief Sends a keep alive repeatedly while sleeping for the required 
 * time-out period.
 * @param param
 */
int muse_send_pkt(void *param)
{
	param_t *param_ptr = (param_t *) param;

	send(get_socket_fd(), param_ptr->ptr, param_ptr->len, 0);
	return (0);
}

/**
 * muse_process_pkt()
 * @brief Processes the packet
 * @param param
 */
static void muse_process_pkt(param_t * param)
{
	// Uncompressed or raw at this point
	fprintf(stdout, "Bytes read = %d\n", param->len);
	hexdump((unsigned char *)param->ptr, param->len);

	if (_TRANS_PKT_FC)
		TRANS_PKT_FC(param);

}

/**
 * muse_read_pkt()
 * @brief Reads incoming packets from the socket
 */
int muse_read_pkt(void *param __attribute__ ((unused)))
{
	int bytes_read = 0;
	char buf[BUFSIZE] = { 0 };
	param_t param_start_transmission = { MUSE_START_TRANSMISSION, 3 };
	param_t *param_translate_pkt = NULL;

	muse_send_pkt(&param_start_transmission);
	do {
		memset(buf, 0, BUFSIZE);
		bytes_read = recv(get_socket_fd(), buf, sizeof(buf), 0);

		if (bytes_read <= 0)
			fprintf(stdout, "Error reading socket: %d\n", bytes_read);
		continue;

		param_translate_pkt->ptr = buf;
		param_translate_pkt->len = bytes_read;
		muse_process_pkt(param_translate_pkt);

	} while (1);
	return (0);
}
