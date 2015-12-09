/**
 * @file csv_file.c
 * @author Frederic Simard, Atlants Embedded (frederic.simard.1@outlook.com)
 * @brief feature output interface, initialize a CSV file for feature output.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feature_output.h"
#include "data_structure.h"
#include "csv_file.h"

/*Define the csv structure*/
typedef struct csv_output_s{
	
	csv_output_options_t options;
	FILE* fp;

}csv_output_t;


/**
 * void* csv_init_file(void *options)
 * @brief Initialize the CSV output interface
 * @param options, options defining the interface
 * @return a pointer to the interface or NULL if invalid
 */
void* csv_init_file(void *options){
	
	/*allocate memory*/
	csv_output_t* this_csv = (csv_output_t*)malloc(sizeof(csv_output_t));
	
	/*copy the options*/
	memcpy(&(this_csv->options),options,sizeof(csv_output_options_t));
	
	/*open a read/write file*/
	this_csv->fp = fopen(this_csv->options.filename,"w+");
	
	/*check if opened correctly*/
	if(this_csv->fp==NULL)
		return NULL;
		
	/*return the pointer*/
	return this_csv;	
}

/**
 * int csv_write_in_file(void *feature_buf, void *csv_interface)
 * @brief Writes the data received in a csv file, skipping a line at the end
 * @param feature_buf, (data_t*) feature buffer to be written
 * @param csv_interface, (csv_output_t*) pointer to the csv interface
 * @return if successful, EXIT_SUCCESS, otherwise, EXIT_FAILURE
 */
int csv_write_in_file(void *feature_buf, void *csv_interface){
	
	int i;
	/*recast pointers*/
	data_t *data = (data_t *) feature_buf;
	csv_output_t* this_csv = (csv_output_t*)csv_interface;
	double* double_buf = (double*)data->ptr;
	
	/*check if file valid*/
	if(this_csv->fp==NULL)
		return EXIT_FAILURE;
	
	/*write feature vector to csv file*/
	for(i=0;i<data->nb_data;i++){
		fprintf(this_csv->fp,"%f;",double_buf[i]);
	}
	
	/*skip a line*/
	fprintf(this_csv->fp,"\n");
	
	return EXIT_SUCCESS;
}

/**
 * int csv_close_file(void *csv_interface)
 * @brief Close the csv file
 * @param param (unused)
 * @return if successful, EXIT_SUCCESS, otherwise, EXIT_FAILURE
 */
int csv_close_file(void *csv_interface){
	
	/*recast the pointer*/
	csv_output_t* this_csv = (csv_output_t*)csv_interface;
	
	/*check if file is valid*/
	if(this_csv->fp==NULL)
		return EXIT_FAILURE;
	
	/*close and free the interface memory*/
	fclose(this_csv->fp);
	free(this_csv);
	
	/*done*/
	return EXIT_SUCCESS;
}
