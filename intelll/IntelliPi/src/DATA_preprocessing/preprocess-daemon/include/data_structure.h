#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

typedef enum { INT32, UINT32, UINT8, DOUBLE } data_type_t;

typedef struct data_s {
	data_type_t type;
	unsigned char *ptr;
	int nb_data;
} data_t;


#endif
