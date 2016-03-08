

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
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int init_feature_output(char input_type, feature_output_options_t options){

	_INIT_FEATURE_OUTPUT_FC = NULL;
	_WRITE_DATA_FC = NULL;
	_TERMINATE_FEATURE_OUTPUT_FC = NULL;
	
		
	printf("%i\n",input_type);
	
		
	if(input_type == SHM_OUTPUT) {
		
		printf("Feature output mode: SHM\n");
		_INIT_FEATURE_OUTPUT_FC = &shm_wrt_init;
		_WRITE_DATA_FC = &shm_wrt_write_to_buf;
		_TERMINATE_FEATURE_OUTPUT_FC = &shm_wrt_cleanup;
	}
	else if(input_type == CSV_OUTPUT) {
		
		printf("Feature output mode: CSV\n");
		_INIT_FEATURE_OUTPUT_FC = &csv_init_file;
		_WRITE_DATA_FC = &csv_write_in_file;
		_TERMINATE_FEATURE_OUTPUT_FC = &csv_close_file;
	}
	else{
		fprintf(stderr, "Unknown feature output mode\n");
		return EXIT_FAILURE;
	}

	return INIT_FEATURE_OUTPUT_FC(&options);
}
