
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include "feature_structure.h"
#include "fake_feature_generator.h"
#include "feature_input.h"


extern double randn();

int fake_feat_gen_init(void *param){
	
	feature_input_t* pfeature_input = param;
	
	pfeature_input->shm_buf = malloc(sizeof(double)*220);
	
	/*nothing to do*/
	return EXIT_SUCCESS;
}

#include <unistd.h>
int fake_feat_gen_request(void *param __attribute__((unused))){
	
	return EXIT_SUCCESS;
}

int fake_feat_gen_wait_for_request_completed(void *param){
	
	int i;
	feature_input_t* pfeature_input = param;
	double* feature_array = (double*)pfeature_input->shm_buf;
	
	for(i=0;i<220;i++){
		feature_array[i] = (double)rand()/(double)RAND_MAX;
	}
	
	usleep(500000);
	
	return EXIT_SUCCESS;
}

int fake_feat_gen_cleanup(void *param){
	
	feature_input_t* pfeature_input = param;
	
	free(pfeature_input->shm_buf);
	
	return EXIT_SUCCESS;
}



