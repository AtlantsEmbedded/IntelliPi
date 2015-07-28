#ifndef DATA_OUTPUT_H
#define DATA_OUTPUT_H
/**
 * @file data_output.c
 * @author Frederic Simard, Atlants Embedded (frederic.simard.1@outlook.com)
 * @brief Configures the data output interface. Sets the function pointers according
 *        to user options.
 */

#define INIT_DATA_OUTPUT_FC() \
		_INIT_DATA_OUTPUT_FC(NULL)
		
#define COPY_DATA_IN(param) \
		_COPY_DATA_IN(param)
		
#define TERMINATE_DATA_OUTPUT_FC() \
		_TERMINATE_DATA_OUTPUT_FC(NULL)
		
/*data types that can be writte*/
typedef enum { INT32, UINT32, UINT8 } data_type_t;
		

/*defines the function pointers associated with the data output*/
typedef int (*functionPtr_t) (void *);
functionPtr_t _INIT_DATA_OUTPUT_FC;
functionPtr_t _COPY_DATA_IN;
functionPtr_t _TERMINATE_DATA_OUTPUT_FC;

/*defines the data structure that contains the data to be sent*/
typedef struct data_s {
	data_type_t type;
	unsigned char *ptr;
	int nb_data;
} data_t;

/*init function to setup funciton pointers*/
int init_data_output(char output_type);


#endif
