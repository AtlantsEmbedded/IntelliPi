


#include "stdio.h"
#include "stdlib.h"

#include <train_set_acq.h>
#include <feature_input.h>


int train_system(train_options_t options, trained_params_t* trained_params){

	/*prepare for training set acquisition*/
	int i;
	double* trainset_array = (double*)malloc(options.nb_train_samples*options.nb_features*sizeof(double));
	feature_vect_t new_sample;
	
	/*acquire samples while target number attained*/
	for(i=0;i<options.nb_train_samples;i++){
		if(get_sample(&new_sample))
			memcpy(trainset_array[i],new_sample.feature_vect,options.nb_features*sizeof(double));
		else
			return EXIT_FAILURE;
	}
	
	/*extract the trained parameters:*/
	/* -compute the average*/
	/* -compute the standard deviation*/
	
	return EXIT_SUCCESS;
}


