/**
 * @file fake_data_generator.c
 * @author Frederic Simard, Atlants Embedded (fred.simard@atlantsembedded.com)
 * @brief This file implements a fake data generator in the context of EEG signal
 * 		  processing.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "fake_data_generator.h"
#include "data_structure.h"
#include "data_input.h"


/**
 * int fake_data_gen_init()
 * @brief Initialize the fake data generator.
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int fake_data_gen_init(){
	
	/*nothing to do*/
	return EXIT_SUCCESS;
}

/**
 * int fake_data_gen_init()
 * @brief Initialize the fake data generator.
 * @param param, (data_t *)buffer to which fake data need to be written
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int fake_data_gen_get_sample(void *param){
	
	int i,j;
	/*recast the array*/
	double* data_buf = (double*)((data_t *)param)->ptr;
	
	/*generate fake data and copy them to the buffer*/
	for(i=0;i<NB_SAMPLES;i++){
		for(j=0;j<NB_CHANNELS;j++){
			/*read and convert*/;
			data_buf[i*NB_CHANNELS+j] = (0.5*(double)rand()/((double)RAND_MAX)+0.5)*sin(2*M_PI*100*i/SAMPLING_FREQ);
		}
	}
	
	return EXIT_SUCCESS;
}

/**
 * int fake_data_gen_cleanup()
 * @brief Cleanup the fake data generator.
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int fake_data_gen_cleanup(){
	
	/*nothing to do*/
	return EXIT_SUCCESS;
}

