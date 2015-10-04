#ifndef FEATURE_STRUCTURE_H
#define FEATURE_STRUCTURE_H

typedef enum { INT32, UINT32, UINT8, DOUBLE } feature_type_t;

typedef struct feature_s {
	feature_type_t type;
	unsigned char *ptr;
	int nb_features;
} feature_t;


#endif
