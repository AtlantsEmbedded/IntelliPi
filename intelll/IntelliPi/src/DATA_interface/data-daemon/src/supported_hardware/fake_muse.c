/**
 * @file fake_muse.c
 * @author Frederic Simard (frederic.simard.1@outlook.com) || Atlants Embedded
 * @brief Handles all MUSE related function pointers, by implementing a 
 *        fake hardware, for offline testing and debugging.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "hardware.h"
#include "main.h"
#include "muse.h"
#include "fake_muse.h"
#include "xml.h"
#include "data_output.h"

static int seconds = 0;

/**
 * fake_muse_init_hardware()
 * @brief Initializes muse hardware related variables
 */
int fake_muse_connect_dev(void *param)
{
	return 0x00;
}

/**
 * fake_muse_cleanup()
 * @brief Muse cleanup function
 */
int fake_muse_cleanup(void *param __attribute__ ((unused)))
{
	return 0x00;
}

/**
 * muse_init_hardware()
 * @brief Initializes muse hardware related variables
 */
int fake_muse_init_hardware(void *param __attribute__ ((unused)))
{
	return 0x00;
}

/** 
 * fake_muse_translate_pkt
 * @brief translate MUSE packet
 */
int fake_muse_translate_pkt(void *param)
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
	
	
	printf("check translate pkt\n");

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
				delta_offset = i*MUSE_NB_CHANNELS;	
				
				/*compute the new value from the previous value*/	
				for(j=0;j<MUSE_NB_CHANNELS;j++){
					cur_eeg_values[j] = cur_eeg_values[j]+muse_trslt_pkt_ptr->eeg_data[delta_offset+j];
				}
				
				/*Push the new sample in the output*/
				COPY_DATA_IN(&data_struct);
				
			}
			break;
		
	}
	
	return 0x00;
}

/**
 * fake_muse_send_keep_alive_pkt(void)
 * @brief Sends a keep alive repeatedly while sleeping for the required 
 * time-out period.
 */
int fake_muse_send_keep_alive_pkt(void *param __attribute__ ((unused)))
{
	int status;

	do {
		sleep(seconds);
	} while (status > 0);

	return (0);
}

/**
 * fake_muse_send_pkt()
 * @brief Sends a keep alive repeatedly while sleeping for the required 
 * time-out period.
 * @param param
 */
int fake_muse_send_pkt(void *param)
{
	param_t *param_ptr = (param_t *) param;
	int status = 0;

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
int fake_muse_process_pkt(void *param)
{
	int i=0;
	
	printf("check process pkt\n");
	
	/*This buffer will temporaly keep the decoded eeg data, 
	  while it is being translated and put in a permanent
	  container. Make sure the content stays valid until the 
	  translation process has returned.*/
	int eeg_data_buffer[MUSE_NB_CHANNELS*MUSE_NB_DELTAS] = { 0 };
	muse_translt_pkt_t param_translate_pkt;
	param_translate_pkt.eeg_data = eeg_data_buffer;
	param_translate_pkt.nb_samples = MUSE_NB_CHANNELS;
	param_translate_pkt.type = MUSE_UNCOMPRESS_PKT;
	
	/*fill with random values*/
	for(i=0;i<MUSE_NB_CHANNELS;i++){
		//eeg_data_buffer[i] = rand();
		eeg_data_buffer[i] = i;
	}
	
	/*one packet*/
	TRANS_PKT_FC(&param_translate_pkt);

	return (0);
}

/**
 * fake_muse_read_pkt()
 * @brief Reads incoming packets from the socket
 */
int fake_muse_read_pkt(void *param __attribute__ ((ubufnused)))
{
    struct timespec interpacket_time;
    
    interpacket_time.tv_sec = 0;
    interpacket_time.tv_nsec = 10000000; /*sleep for 4 milliseconds*/
	
	do {

		nanosleep(&interpacket_time,NULL);
		printf("check read pkt %i\n",rand());
		
		/*call for packet processing*/
		PROCESS_PKT_FC(NULL);

	} while (1);
	return (0);
}

