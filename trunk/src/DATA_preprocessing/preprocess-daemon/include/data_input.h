
#ifndef DATA_INPUT_H
#define DATA_INPUT_H

#define INIT_DATA_INPUT_FC() \
		_INIT_DATA_INPUT_FC(NULL)
		
#define READ_DATA_FC(param) \
		_READ_DATA_FC(param)
		
#define TERMINATE_DATA_INPUT_FC() \
		_TERMINATE_DATA_INPUT_FC(NULL)
		
typedef int (*functionPtr_t) (void *);
functionPtr_t _INIT_DATA_INPUT_FC;
functionPtr_t _READ_DATA_FC;
functionPtr_t _TERMINATE_DATA_INPUT_FC;

int init_data_input(char input_type);


#endif
