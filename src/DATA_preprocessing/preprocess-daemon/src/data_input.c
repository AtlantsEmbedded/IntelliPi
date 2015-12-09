/**
 * @file data_input.c
 * @author Frederic Simard (fred.simard@atlantsembedded.com), Ron Brash (ron.brash@gmail.com) | Atlants Embedded
 * @brief Data input interface initialization.
 * 		  Links the virtual definition with the configured interface
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_input.h"
#include "fake_data_generator.h"
#include "shm_rd_buf.h"
#include "xml.h"

/**
 * int init_data_input(char input_type)
 * @brief Setup function pointers for the data input
 * @param input_type, string identifying the type of input to init
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int init_data_input(char input_type, data_input_options_t options){

	/*init the virtual interface with default values*/
	_INIT_DATA_INPUT_FC = NULL;
	_READ_DATA_FC = NULL;
	_TERMINATE_DATA_INPUT_FC = NULL;
		
	/*shared memory input*/
	if(input_type == SHM_INPUT) {
		_INIT_DATA_INPUT_FC = &shm_rd_init;
		_READ_DATA_FC = &shm_rd_read_from_buf;
		_TERMINATE_DATA_INPUT_FC = &shm_rd_cleanup;
	}
	/*fake signal input*/
	else if(input_type == FAKE_INPUT){
		_INIT_DATA_INPUT_FC = &fake_data_gen_init;
		_READ_DATA_FC = &fake_data_gen_get_sample;
		_TERMINATE_DATA_INPUT_FC = &fake_data_gen_cleanup;
	}
	else{
		fprintf(stderr, "Unknown intput type\n");
		return EXIT_FAILURE;
	}

	/*inint the interface*/
	return INIT_DATA_INPUT_FC(&options);
}
