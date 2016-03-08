/**
 * @file data_output.c
 * @author Frederic Simard, Atlants Embedded (frederic.simard.1@outlook.com)
 * @brief Configures the data output interface. Sets the function pointers according
 *        to user options.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_output.h"
#include "csv_file.h"
#include "shm_wrt_buf.h"
#include "xml.h"

/**
 * int init_data_output(char *output_type)
 * @brief Setup function pointers for the data output
 * @param output_type, identifies the type of output to init
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int init_data_output(char output_type, data_out_options_t options){

	_INIT_DATA_OUTPUT_FC = NULL;
	_COPY_DATA_IN = NULL;
	_TERMINATE_DATA_OUTPUT_FC = NULL;
		
	/*output to colum separated values file (only for debug purpose for now)*/
	if(output_type == CSV_OUTPUT) {
		
		printf("Output type: CSV\n");
		_INIT_DATA_OUTPUT_FC = &csv_init_file;
		_COPY_DATA_IN = &csv_write_in_file;
		_TERMINATE_DATA_OUTPUT_FC = &csv_close_file;
	}
	/*output to shared memory*/
	else if(output_type == SHM_OUTPUT) {
		
		printf("Output type: SHM\n");
		_INIT_DATA_OUTPUT_FC = &shm_wrt_init;
		_COPY_DATA_IN = &shm_wrt_write_in_buf;
		_TERMINATE_DATA_OUTPUT_FC = &shm_wrt_cleanup;
	}
	/*Error, wrong type of output*/
	else{
		fprintf(stderr, "Unknown output type\n");
		return EXIT_FAILURE;
	}

	return INIT_DATA_OUTPUT_FC((void*)&options);
}
