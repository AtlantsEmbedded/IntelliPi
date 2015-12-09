
#ifndef DATA_INPUT_H
#define DATA_INPUT_H

#define INIT_DATA_INPUT_FC(param) \
		_INIT_DATA_INPUT_FC(param)
		
#define READ_DATA_FC(param) \
		_READ_DATA_FC(param)
		
#define TERMINATE_DATA_INPUT_FC() \
		_TERMINATE_DATA_INPUT_FC(NULL)
		
typedef int (*functionPtr_t) (void *);
functionPtr_t _INIT_DATA_INPUT_FC;
functionPtr_t _READ_DATA_FC;
functionPtr_t _TERMINATE_DATA_INPUT_FC;

typedef struct data_input_options_s{
	
	/*IPC keys*/
	int shm_key;
	int sem_key;
	
	/*input data properties*/
	int nb_channels;
	int window_width;
	
}data_input_options_t;

int init_data_input(char input_type, data_input_options_t options);


#endif
