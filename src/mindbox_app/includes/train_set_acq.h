#ifndef TRAIN_SET_ACQ_H
#define TRAIN_SET_ACQ_H


typedef struct train_options_s{
	int nb_train_samples;
	int nb_features;
}train_options_t; 

typedef struct trained_params_s{
	mean;
	std_dev;
}trained_params_t; 

int train_system(train_options_t options, trained_params_t* trained_params);


#endif
