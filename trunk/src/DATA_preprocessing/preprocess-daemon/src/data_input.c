


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_input.h"
#include "shm_rd_buf.h"
#include "xml.h"

/**
 * int init_data_input(char input_type)
 * @brief Setup function pointers for the data input
 * @param input_type, string identifying the type of input to init
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int init_data_input(char input_type){

	_INIT_DATA_INPUT_FC = NULL;
	_READ_DATA = NULL;
	_TERMINATE_DATA_INPUT_FC = NULL;

	input_type = SHM_INPUT;
		
	if(input_type == SHM_INPUT) {
		
		printf("Input type: SHM\n");
		_INIT_DATA_INPUT_FC = &shm_rd_init;
		_READ_DATA = &shm_rd_read_from_buf;
		_TERMINATE_DATA_INPUT_FC = &shm_rd_cleanup;
	}
	else{
		fprintf(stderr, "Unknown output type\n");
		return EXIT_FAILURE;
	}

	return INIT_DATA_INPUT_FC();
}
