
#ifndef FEATURE_OUTPUT_H
#define FEATURE_OUTPUT_H

#define INIT_FEATURE_OUTPUT_FC() \
		_INIT_FEATURE_OUTPUT_FC(NULL)
		
#define WRITE_DATA_FC(param) \
		_WRITE_DATA_FC(param)
		
#define TERMINATE_FEATURE_OUTPUT_FC() \
		_TERMINATE_FEATURE_OUTPUT_FC(NULL)
		
typedef int (*functionPtr_t) (void *);
functionPtr_t _INIT_FEATURE_OUTPUT_FC;
functionPtr_t _WRITE_DATA_FC;
functionPtr_t _TERMINATE_FEATURE_OUTPUT_FC;

int init_feature_output(char input_type);


#endif
