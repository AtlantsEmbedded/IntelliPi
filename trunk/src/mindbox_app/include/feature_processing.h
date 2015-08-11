#ifndef TRAIN_SET_ACQ_H
#define TRAIN_SET_ACQ_H

#include "feature_structure.h"

typedef struct feat_proc_options_s{
	int nb_dropped_samples;
	int nb_train_samples;
	int nb_features;
}feat_proc_options_t; 

int init_feat_processing(feat_proc_options_t init_struct);
int train_feat_processing();
int get_normalized_sample(feature_t* feature_vect);
int clean_up_feat_processing();

#endif
