


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include "data_storage.h"
#include "csv_file.h"
#include "xml.h"

/**
 * int init_data_storage(char *storage_type)
 * @brief Setup function pointers for the data storage
 * @param storage_type, string identifying the type of storage to init
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int init_data_storage(char storage_type){

	_INIT_DATA_STORAGE_FC = NULL;
	_COPY_DATA_IN = NULL;
	_TERMINATE_DATA_STORAGE_FC = NULL;

	storage_type = CSV_OUTPUT;
		
	if(storage_type == CSV_OUTPUT) {
		
		printf("Storage type: CSV\n");
		_INIT_DATA_STORAGE_FC = &csv_init_file;
		_COPY_DATA_IN = &csv_write_in_file;
		_TERMINATE_DATA_STORAGE_FC = &csv_close_file;
	}
	else{
		fprintf(stderr, "Unknown storage type\n");
		return EXIT_FAILURE;
	}

	return INIT_DATA_STORAGE_FC();
}
