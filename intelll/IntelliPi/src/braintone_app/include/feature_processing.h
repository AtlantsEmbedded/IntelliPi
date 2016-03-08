#ifndef TRAIN_SET_ACQ_H
#define TRAIN_SET_ACQ_H

#include "feature_structure.h"
#include "feature_input.h"


typedef struct feat_proc_s{
	
	/*to be set before init*/
	int nb_train_samples;
	feature_input_t* feature_input;
	
	/*set during training*/
	double mean[2];
	double std_dev[2];
	
	/*current sample value, set during get_normalized_sample*/
	double sample;
		
}feat_proc_t; 

int init_feat_processing(feat_proc_t* feature_proc);
void train_feat_processing(feat_proc_t* feature_proc);
int get_normalized_sample(feat_proc_t* feature_proc);
int clean_up_feat_processing(feat_proc_t* feature_proc);

#endif
