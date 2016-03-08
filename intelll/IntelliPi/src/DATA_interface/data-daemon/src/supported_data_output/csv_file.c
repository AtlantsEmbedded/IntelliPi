/**
 * @file csv_file.c
 * @author Frederic Simard, Atlants Embedded (frederic.simard.1@outlook.com)
 * @brief Handles the CSV file function pointers
 */

#include <stdio.h>
#include <stdlib.h>

#include "data_output.h"
#include "csv_file.h"

static FILE* fp;

/**
 * int csv_init_file(void *param)
 * @brief Sends a keep alive repeatedly while sleeping for the required 
 * time-out period.
 * @param param (unused)
 * @return if successful, EXIT_SUCCESS, otherwise, EXIT_FAILURE
 */
int csv_init_file(void *param __attribute__ ((unused))){
	
	/*open a read/write file*/
	fp = fopen("eeg_data.csv","w+");
	
	/*check if opened correctly*/
	if(fp==NULL)
		return EXIT_FAILURE;
		
	return EXIT_SUCCESS;	
}

/**
 * int csv_write_in_file(void *param)
 * @brief Writes the data received in a csv file, skipping a line at the end
 * @param param, must be pointing to a data_t
 * @return if successful, EXIT_SUCCESS, otherwise, EXIT_FAILURE
 */
int csv_write_in_file(void *param){
	
	int i;
	data_t *data = (data_t *) param;
	int* uint32_buf;
	int line_offset;
	
	if(fp==NULL)
		return EXIT_FAILURE;
	
	/*check which type of data is coming in*/
	switch(data->type){
		/*32 bits unsigned integer*/
		case INT32:
			/*recast the pointer*/
			uint32_buf = (int*)data->ptr;
			for(i=0;i<data->nb_data;i++){
				fprintf(fp,"%i;",uint32_buf[i]);
			}
		break;
		
		/*8 bits integer (unsigned char) not used*/
		case UINT8:
			/*good to go*/
			for(i=0;i<data->nb_data;i++){
				fprintf(fp,"%i;",data->ptr[i]);
			}
		break;
	}
	
	/*skip a line*/
	fprintf(fp,"\n");
	
	return EXIT_SUCCESS;
}

/**
 * int csv_close_file(void *param)
 * @brief Close the csv file
 * @param param (unused)
 * @return if successful, EXIT_SUCCESS, otherwise, EXIT_FAILURE
 */
int csv_close_file(void *param __attribute__ ((unused))){

	if(fp==NULL)
		return EXIT_FAILURE;
		
	fclose(fp);
	return EXIT_SUCCESS;
}
