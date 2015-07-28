
#ifndef DATA_INPUT_H
#define DATA_INPUT_H

#define INIT_DATA_INPUT_FC() \
		_INIT_DATA_INPUT_FC(NULL)
		
#define READ_DATA(param) \
		_READ_DATA(param)
		
#define TERMINATE_DATA_INPUT_FC() \
		_TERMINATE_DATA_INPUT_FC(NULL)
		

typedef enum { INT32, UINT32, UINT8 } data_type_t;
		

typedef int (*functionPtr_t) (void *);
functionPtr_t _INIT_DATA_INPUT_FC;
functionPtr_t _READ_DATA;
functionPtr_t _TERMINATE_DATA_INPUT_FC;

typedef struct data_s {
	data_type_t type;
	unsigned char *ptr;
	int nb_data;
} data_t;

int init_data_input(char input_type);


#endif
