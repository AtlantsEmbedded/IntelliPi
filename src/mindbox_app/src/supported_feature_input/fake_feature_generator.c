
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "feature_structure.h"
#include "fake_feature_generator.h"
#include "feature_input.h"


extern double randn();

int fake_feat_gen_init(){
	
	/*nothing to do*/
	return EXIT_SUCCESS;
}

int fake_feat_gen_get_sample(void *param){
	
	int i,j;
	/*fill in the buffer with a fake signal*/
	double* feat_buf = (double*)((feature_t *)param)->ptr;
	
	for(i=0;i<((feature_t *)param)->nb_features;i++){
		feat_buf[i] = randn();
	}
	
	return EXIT_SUCCESS;
}

int fake_feat_gen_cleanup(){
	
	/*nothing to do*/
	return EXIT_SUCCESS;
}



