/**
 * @file feature_output.c
 * @author Frederic Simard (fred.simard@atlantsembedded.com), Ron Brash (ron.brash@gmail.com) | Atlants Embedded
 * @brief Feature output initialization.
 * 		  Links the virtual definition with the configured interface. A pointer
 * 		  To the interface is returned
 * 		  
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_structure.h"
#include "feature_output.h"
#include "csv_file.h"
#include "shm_wrt_buf.h"
#include "xml.h"

/**
 * int init_data_input(char input_type)
 * @brief Setup function pointers for the feature output
 * @param output_type, integer identifying the output mode to be configured 
 * @param options, integer identifying the output mode to be configured 
 * @return NULL for unknown type, pointer to interface if success
 */
void* init_feature_output(char output_type, void* options){

	/*init virutal interface with default values*/
	_INIT_FEATURE_OUTPUT_FC = NULL;
	_WRITE_FEATURE_FC = NULL;
	_TERMINATE_FEATURE_OUTPUT_FC = NULL;
		
	/*check output interface type*/
	/*shared memory*/
	if(output_type == SHM_OUTPUT) {
		
		_INIT_FEATURE_OUTPUT_FC = &shm_wrt_init;
		_WRITE_FEATURE_FC = &shm_wrt_write_to_buf;
		_TERMINATE_FEATURE_OUTPUT_FC = &shm_wrt_cleanup;
	}
	/*CSV file*/
	else if(output_type == CSV_OUTPUT) {
		
		_INIT_FEATURE_OUTPUT_FC = &csv_init_file;
		_WRITE_FEATURE_FC = &csv_write_in_file;
		_TERMINATE_FEATURE_OUTPUT_FC = &csv_close_file;
	}
	else{
		fprintf(stderr, "Unknown feature output mode\n");
		return NULL;
	}

	/*init the feature output and return pointer*/
	return INIT_FEATURE_OUTPUT_FC(options);
}
