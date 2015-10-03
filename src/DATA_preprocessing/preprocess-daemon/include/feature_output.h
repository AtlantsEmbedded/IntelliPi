
#ifndef FEATURE_OUTPUT_H
#define FEATURE_OUTPUT_H

#define INIT_FEATURE_OUTPUT_FC(param) \
		_INIT_FEATURE_OUTPUT_FC(param)
		
#define WRITE_DATA_FC(param) \
		_WRITE_DATA_FC(param)
		
#define TERMINATE_FEATURE_OUTPUT_FC() \
		_TERMINATE_FEATURE_OUTPUT_FC(NULL)
		
typedef int (*functionPtr_t) (void *);
functionPtr_t _INIT_FEATURE_OUTPUT_FC;
functionPtr_t _WRITE_DATA_FC;
functionPtr_t _TERMINATE_FEATURE_OUTPUT_FC;


typedef struct feature_output_options_s{
	
	/*IPC keys*/
	int shm_key;
	int sem_key;
	
	/*feature vect optional content*/
	unsigned char timeseries;
	unsigned char fft;
	unsigned char power_alpha;
	unsigned char power_beta;
	unsigned char power_gamma;
	
}feature_output_options_t;

int init_feature_output(char input_type, feature_output_options_t options);


#endif
