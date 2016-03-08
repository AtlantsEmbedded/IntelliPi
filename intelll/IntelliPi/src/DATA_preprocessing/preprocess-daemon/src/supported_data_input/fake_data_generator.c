
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "fake_data_generator.h"
#include "data_structure.h"
#include "data_input.h"


int fake_data_gen_init(){
	
	/*nothing to do*/
	return EXIT_SUCCESS;
}

int fake_data_gen_get_sample(void *param){
	
	int i,j;
	/*fill in the buffer with a fake signal*/
	double* data_buf = (double*)((data_t *)param)->ptr;
	
	/*read all data in the page*/
	for(i=0;i<NB_SAMPLES;i++){
		for(j=0;j<NB_CHANNELS;j++){
			/*read and convert*/;
			data_buf[i*NB_CHANNELS+j] = (0.5*(double)rand()/((double)RAND_MAX)+0.5)*sin(2*M_PI*100*i/SAMPLING_FREQ);
			
			//+
			//							(0.5*(double)rand()/((double)RAND_MAX)+0.5)*sin(2*M_PI*0*i/SAMPLING_FREQ);
		}
	}
	
	return EXIT_SUCCESS;
}

int fake_data_gen_cleanup(){
	
	/*nothing to do*/
	
	return EXIT_SUCCESS;
}

