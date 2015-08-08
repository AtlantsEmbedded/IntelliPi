
#ifndef FEATURE_INPUT_H
#define FEATURE_INPUT_H

#define INIT_FEAT_INPUT_FC() \
		_INIT_FEAT_INPUT_FC(NULL)
		
#define READ_FEAT_FC(param) \
		_READ_FEAT_FC(param)
		
#define TERMINATE_FEAT_INPUT_FC() \
		_TERMINATE_FEAT_INPUT_FC(NULL)
		
typedef int (*functionPtr_t) (void *);
functionPtr_t _INIT_FEAT_INPUT_FC;
functionPtr_t _READ_FEAT_FC;
functionPtr_t _TERMINATE_FEAT_INPUT_FC;

int init_feature_input(char input_type);


#endif
