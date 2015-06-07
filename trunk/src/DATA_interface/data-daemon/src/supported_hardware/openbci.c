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
#include "xml.h"

#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>

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

	return setup_serial((unsigned char *)param_ptr->ptr);
}

/**
 * openbci_cleanup()
 * @brief Openbci cleanup function
 */
int openbci_cleanup(void *param __attribute__ ((unused)))
{
	param_t param_stop_transmission = { OPENBCI_HALT_TRANSMISSION, 1 };
	openbci_send_pkt(&param_stop_transmission);

	close_serial();
	return (0);
}

/** 
 * openbci_translate_pkt
 * @brief translate MUSE packet
 */
int openbci_translate_pkt(void *param __attribute__ ((unused)))
{
	// Unused for now
	param_t *param_ptr __attribute__ ((unused)) = (param_t *) param;

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
	// Not used / Not required
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
	int ret __attribute__ ((unused));

	ret = write(get_serial_fd(), param_ptr->ptr, param_ptr->len);

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

	param_t param_start_transmission = { OPENBCI_START_TRANSMISSION, 1 };
	param_t param_stop_transmission = { OPENBCI_HALT_TRANSMISSION, 1 };
	param_t param_reset_transmission = { OPENBCI_RESET, 1 };
	param_t param_translate_pkt = { 0 };
	char buf[255] = { 0 };

	int fd = get_serial_fd();
	int check = 0;
	int num, offset = 0, bytes_expected = 130;

	// If running lets stop the transmission and reset the device
	openbci_send_pkt(&param_stop_transmission);
	openbci_send_pkt(&param_reset_transmission);
	sleep(2);
	// If you care about the $$$ prompt
	do {
		num = read(fd, buf + offset, 255);

		if (buf[num - 1] == '$') {
			check++;
			break;
		}

		offset += num;
		param_translate_pkt.ptr = buf;
		param_translate_pkt.len = num;
		PROCESS_PKT_FC(&param_translate_pkt);

	} while (check > 0);

	sleep(1);

	// Now restart the communication
	memset(buf, 0, 255);
	openbci_send_pkt(&param_start_transmission);

	int samples = 0;
	bytes_expected = DATA_PACKET_LENGTH;
	do {
		offset = 0;

		do {
			offset += read(fd, buf + offset, 1);

		} while (offset < bytes_expected);

		if (bytes_read < 0) {
			fprintf(stdout, "Error reading socket: %d\n", bytes_read);
			continue;
		}

		param_translate_pkt.ptr = buf;
		param_translate_pkt.len = DATA_PACKET_LENGTH;

		PROCESS_PKT_FC(&param_translate_pkt);

		samples++;

	} while (samples < get_appconfig()->samples);
	openbci_send_pkt(&param_stop_transmission);
	printf("Samples taken: %d\n", samples);
	return (0);
}
