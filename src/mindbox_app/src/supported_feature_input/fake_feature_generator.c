/**
 * @file fake_feature_generator.c
 * @author Frederic Simard (fred.simard@atlantsembedded.com) | Atlants Embedded
 * @brief Fake feature generator, can be used to stub a real feature input.
 *        For debug purpose.
 * 
 * 		  Fake features are uniform data range 0-1
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "feature_structure.h"
#include "fake_feature_generator.h"
#include "feature_input.h"

#define NB_FEATURES 220
#define SLEEP_PERIOD 500000

extern double randn(); // defined in random walk process (to be fixed)

/**
 * int fake_feat_gen_init(void *param)
 * @brief initialize a fake feature generator
 * @param param, pointer to a feature_input struct
 * @return EXIT_SUCCESS
 */ 
int fake_feat_gen_init(void *param){
	
	/*re-cast for simplicity*/
	feature_input_t* pfeature_input = param;
	
	/*allocate memory for the feature vector*/
	pfeature_input->shm_buf = malloc(sizeof(double)*NB_FEATURES);
	
	/*done*/
	return EXIT_SUCCESS;
}

/**
 * int fake_feat_gen_request(void *param)
 * @brief stub for request (do nothing)
 * @param param, pointer to a feature_input struct
 * @return EXIT_SUCCESS
 */ 
int fake_feat_gen_request(void *param){
	
	/*nothing to do*/
	return EXIT_SUCCESS;
}

/**
 * int fake_feat_gen_wait_for_request_completed(void *param)
 * @brief Fills the feature vector with random data, add a wait to simulate
 *        data acquisition delay
 * @param param, pointer to a feature_input struct
 * @return EXIT_SUCCESS
 */ 
int fake_feat_gen_wait_for_request_completed(void *param){
	
	int i;
	/*re-cast for simplicity*/
	feature_input_t* pfeature_input = param;
	/*re-cast for simplicity*/
	double* feature_array = (double*)pfeature_input->shm_buf;
	
	/*fill the feature vector with random values range 0-1*/
	for(i=0;i<NB_FEATURES;i++){
		feature_array[i] = (double)rand()/(double)RAND_MAX;
	}
	
	/*wait to simulate delay of acquisition*/
	usleep(SLEEP_PERIOD);
	
	/*done*/
	return EXIT_SUCCESS;
}

/**
 * int fake_feat_gen_cleanup(void *param)
 * @brief Cleanup the fake feature generator
 * @param param, pointer to a feature_input struct
 * @return EXIT_SUCCESS
 */ 
int fake_feat_gen_cleanup(void *param){
	
	/*re-cast for simplicity*/
	feature_input_t* pfeature_input = param;
	/*free the memory buffer*/
	free(pfeature_input->shm_buf);
	
	return EXIT_SUCCESS;
}

