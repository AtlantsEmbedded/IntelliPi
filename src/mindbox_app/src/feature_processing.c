


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feature_processing.h"
#include "feature_input.h"

feat_proc_options_t v_options;
double* v_mean;
double* v_std_dev;


int init_feat_processing(feat_proc_options_t init_struct){
	
	v_options = init_struct;
	v_mean = (double*)malloc(sizeof(double)*v_options.nb_features);
	v_std_dev = (double*)malloc(sizeof(double)*v_options.nb_features);
	
	return EXIT_SUCCESS;
}

int train_feat_processing(){

	/*prepare for training set acquisition*/
	
	
	/*start acquisition*/
	
	
	/*drop first few samples*/
	
	
	/*log the next sequence of samples*/
	
	
	/*extract the training set parameters:*/
	/* -compute the average*/
	/* -compute the standard deviation*/
	
	return EXIT_SUCCESS;
}


int get_normalized_sample(feature_t* feature_vect){
	
	int i;
	double* features = (double*)feature_vect->ptr;
	
	/*wait for a sample*/
	
	
	/*normalize each features*/
	for(i=0;i<v_options.nb_features;i++){
		features[i] = (features[i]-v_mean[i])/v_std_dev[i];
	}
	
	return EXIT_SUCCESS;
}


int clean_up_feat_processing(){
	
	free(v_mean);
	free(v_std_dev);
	return EXIT_SUCCESS;
}
