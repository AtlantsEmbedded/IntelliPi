/**
 * @file muse.c
 * @author Ron Brash (ron.brash@gmail.com)
 *         Frederic Simard (frederic.simard.1@outlook.com) || Atlants Embedded
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
#include "muse_pack_parser.h"
#include "data_output.h"

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
	int i,j;
	int delta_offset;
	muse_translt_pkt_t *muse_trslt_pkt_ptr = (muse_translt_pkt_t *) param;
	
	/*new samples might be relative to last sample, we keep the current*/
	/*eeg data in a persistent output, until it's being replaced.*/
	static int cur_eeg_values[MUSE_NB_CHANNELS];
	
	data_t data_struct;
	data_struct.type = INT32;
	data_struct.nb_data = MUSE_NB_CHANNELS;
	data_struct.ptr = (unsigned char*)cur_eeg_values;

	/*Check the type of eeg samples we are receiving*/
	switch(muse_trslt_pkt_ptr->type){
	
		case MUSE_UNCOMPRESS_PKT:
			/*It's an uncompressed packet, we just received the actual eeg values*/
			/*do nothing, the data is good*/
			for(i=0;i<MUSE_NB_CHANNELS;i++){
				cur_eeg_values[i] = muse_trslt_pkt_ptr->eeg_data[i];
			}
					
			/*Push the new sample in the output*/
			COPY_DATA_IN(&data_struct);
			
			break;

		case MUSE_COMPRESSED_PKT:
		
			
			/*It's a compressed packet, we just received the variation measured from previous sample*/
			/*go over all deltas*/
			for(i=0;i<MUSE_NB_DELTAS;i++){
				
				/*compute the new value from the previous value*/
				for(j=0;j<MUSE_NB_CHANNELS;j++){
					cur_eeg_values[j] = cur_eeg_values[j]+muse_trslt_pkt_ptr->eeg_data[j*MUSE_NB_DELTAS+i];		
				}
				
				/*Push the new sample in the output*/
				COPY_DATA_IN(&data_struct);
			}
		
			break;
		
	}
	
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
	int i=0;
	int nb_of_soft_packets;
	int soft_packets_headers[MAX_NB_SOFT_PACKETS];		
	int soft_packets_types[MAX_NB_SOFT_PACKETS];	
	
	/*This buffer will temporaly keep the decoded eeg data, 
	  while it is being translated and put in a permanent
	  container. Make sure the content stays valid until the 
	  translation process has returned.*/
	int eeg_data_buffer[MUSE_NB_CHANNELS*MUSE_NB_DELTAS] = { 0 };
	muse_translt_pkt_t param_translate_pkt;
	param_translate_pkt.eeg_data = eeg_data_buffer;
	param_translate_pkt.nb_samples = MUSE_NB_CHANNELS;
	
	// Uncompressed or raw at this point
	param_t *param_ptr = (param_t *) param;
	
	int nb_bits = 0;
	
	if (param_ptr->len >= 6) {
		
		/*pre-parse the bluetooth packet to know how many soft packets*/
		/*are present*/	
		nb_of_soft_packets = preparse_packet((unsigned char *)param_ptr->ptr, param_ptr->len, soft_packets_headers, soft_packets_types);
	
		/*process each individual packet*/
		for(i=0;i<nb_of_soft_packets;i++){
			       
			switch(soft_packets_types[i]){
				case MUSE_UNCOMPRESS_PKT:

					/*Extract EEG values*/
					parse_uncompressed_packet(&(param_ptr->ptr[soft_packets_headers[i]+1]), eeg_data_buffer);
					
						/*Send them to the translator*/
						param_translate_pkt.type = MUSE_UNCOMPRESS_PKT;
						TRANS_PKT_FC(&param_translate_pkt);

				break;
		
				case MUSE_COMPRESSED_PKT:	

					/*Extract delta values values*/
					parse_compressed_packet(&(param_ptr->ptr[soft_packets_headers[i]]), eeg_data_buffer);

					/*Send them to the translator*/
					param_translate_pkt.type = MUSE_COMPRESSED_PKT;
					TRANS_PKT_FC(&param_translate_pkt);


				break;
			}
			
		}
		
		
		
	} else {
		printf("Invalid packet - too small\n");
	}

	return (0);
}

/**
 * muse_read_pkt()
 * @brief Reads incoming packets from the socket
 */
int muse_read_pkt(void *param __attribute__ ((ubufnused)))
{
	int bytes_read = 0;
	char buf[BUFSIZE] = { 0 };
	param_t param_start_transmission = { MUSE_START_TRANSMISSION, 3 };
	param_t param_request_transmission = { MUSE_VERSION, 5};
	param_t param_preset_transmission = { MUSE_PRESET, 6};
	param_t param_host_transmission = { MUSE_SET_HOST_PLATFORM, 5};
	param_t param_process_pkt = { 0 };

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

		/*build the param structure containing the hard packet*/
		param_process_pkt.ptr = buf;
		param_process_pkt.len = bytes_read;
		/*send the packet for processing*/
		PROCESS_PKT_FC(&param_process_pkt);
		memset(buf, 0, bytes_read);

	} while (1);
	return (0);
}

