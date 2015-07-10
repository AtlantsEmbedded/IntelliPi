
#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H

#define INIT_DATA_STORAGE_FC() \
		_INIT_DATA_STORAGE_FC(NULL)
		
#define COPY_DATA_IN(param) \
		_COPY_DATA_IN(param)
		
#define TERMINATE_DATA_STORAGE_FC() \
		_TERMINATE_DATA_STORAGE_FC(NULL)
		

typedef enum { INT32, UINT32, UINT8 } data_type_t;
		

typedef int (*functionPtr_t) (void *);
functionPtr_t _INIT_DATA_STORAGE_FC;
functionPtr_t _COPY_DATA_IN;
functionPtr_t _TERMINATE_DATA_STORAGE_FC;

typedef struct data_s {
	data_type_t type;
	unsigned char *ptr;
	int nb_data;
} data_t;

int init_data_storage(char storage_type);


#endif
