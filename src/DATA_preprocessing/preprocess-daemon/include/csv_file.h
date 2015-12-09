#ifndef CSV_FILE_H
#define CSV_FILE_H
/**
 * @file csv_file.h
 * @author Frederic Simard, Atlants Embedded (frederic.simard.1@outlook.com)
 * @brief Handles the CSV file function pointers
 */
 
/*Define the csv options*/
typedef struct csv_output_options_s{
	char* filename;
}csv_output_options_t;

void* csv_init_file(void *options);
int csv_write_in_file(void *feature_buf, void *csv_interface);
int csv_close_file(void *csv_interface);


#endif
