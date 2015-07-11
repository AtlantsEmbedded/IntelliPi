


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include "data_output.h"
#include "csv_file.h"
#include "xml.h"

/**
 * int init_data_output(char *output_type)
 * @brief Setup function pointers for the data output
 * @param output_type, string identifying the type of output to init
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int init_data_output(char output_type){

	_INIT_DATA_OUTPUT_FC = NULL;
	_COPY_DATA_IN = NULL;
	_TERMINATE_DATA_OUTPUT_FC = NULL;

	output_type = CSV_OUTPUT;
		
	if(output_type == CSV_OUTPUT) {
		
		printf("Output type: CSV\n");
		_INIT_DATA_OUTPUT_FC = &csv_init_file;
		_COPY_DATA_IN = &csv_write_in_file;
		_TERMINATE_DATA_OUTPUT_FC = &csv_close_file;
	}
	else{
		fprintf(stderr, "Unknown output type\n");
		return EXIT_FAILURE;
	}

	return INIT_DATA_OUTPUT_FC();
}
