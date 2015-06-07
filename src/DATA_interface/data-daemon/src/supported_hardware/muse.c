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

static int seconds = 0;

/**
 * muse_init_hardware()
 * @brief Initializes muse hardware related variables
 */
int muse_connect_dev(void *param)
{
	param_t *param_ptr = (param_t *) param;
	return setup_socket((unsigned char *)param_ptr->ptr);
}

/**
 * muse_cleanup()
 * @brief Muse cleanup function
 */
int muse_cleanup(void *param __attribute__ ((unused)))
{
	close_sockets();
	return (0);
}

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
	param_t *param_pt __attribute__ ((unused)) = (param_t *) param;

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
	int status = 0;

	status = send(get_socket_fd(), param_ptr->ptr, param_ptr->len, 0);

	if (status < 0) {
		printf("error sending pkt\n");
	}
	return (0);
}

/**
 * muse_process_pkt()
 * @brief Processes the packet
 * @param param
 */
int muse_process_pkt(void *param)
{
	// Uncompressed or raw at this point
	param_t *param_ptr = (param_t *) param;
	muse_raw_pkt_t *muse_raw_pkt_ptr = NULL;

	uint8_t shim = 1;	// Header - type + flag == one byte
	fprintf(stdout, "Bytes read = %d\n", param_ptr->len);
	hexdump((unsigned char *)param_ptr->ptr, param_ptr->len);
	if (param_ptr->len >= 6) {
		muse_raw_pkt_ptr = (muse_raw_pkt_t *) param_ptr->ptr;
		if ((muse_raw_pkt_ptr->flag >> 4) == 0x8) {
			shim += 2;
		}
		printf("type: %01x\n", muse_raw_pkt_ptr->type & 0x0F);
		switch ( muse_raw_pkt_ptr->type & 0x0F) {
		case MUSE_UNCOMPRESS_PKT:
			//~ muse_raw_pkt_ptr->ch1 = muse_raw_pkt_ptr + shim;
			//~ muse_raw_pkt_ptr->ch2 = muse_raw_pkt_ptr + shim + 1;
			//~ muse_raw_pkt_ptr->ch3 = muse_raw_pkt_ptr + shim + 2;
			//~ muse_raw_pkt_ptr->ch4 = muse_raw_pkt_ptr + shim + 3;
			//~ printf("ch1: %x\n", muse_raw_pkt_ptr->ch1);
			//~ printf("ch2: %x\n", muse_raw_pkt_ptr->ch2);
			//~ printf("ch3: %x\n", muse_raw_pkt_ptr->ch3);
			//~ printf("ch4: %x\n", muse_raw_pkt_ptr->ch4);

			printf("got a eeg pkt\n");
			break;
		case MUSE_COMPRESSED_PKT:
			printf("got a compressed eeg pkt\n");
			break;
		default:
			
			return (0);
			break;
		}

	} else {
		printf("Invalid packet - too small\n");
	}

	//~ param_ob.ptr = muse_raw_pkt_ptr;



	//~ if (_TRANS_PKT_FC) {
	//~ TRANS_PKT_FC(&param_ob);
	//~ }

	return (0);
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
	param_t param_request_transmission = { MUSE_VERSION, 5};
	param_t param_preset_transmission = { MUSE_PRESET, 6};
	param_t param_host_transmission = { MUSE_SET_HOST_PLATFORM, 5};
	param_t param_translate_pkt = { 0 };

	muse_send_pkt((void *)&param_request_transmission);
	muse_send_pkt((void *)&param_host_transmission);
	muse_send_pkt((void *)&param_preset_transmission);
	
	muse_send_pkt((void *)&param_start_transmission);

	do {

		bytes_read = recv(get_socket_fd(), buf, BUFSIZE, 0);

		if (bytes_read <= 0) {
			fprintf(stdout, "Error reading socket: %d\n", bytes_read);
			continue;
		}

		param_translate_pkt.ptr = buf;
		param_translate_pkt.len = bytes_read;
		PROCESS_PKT_FC(&param_translate_pkt);
		memset(buf, 0, bytes_read);

	} while (1);
	return (0);
}
