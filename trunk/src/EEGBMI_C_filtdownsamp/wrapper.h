#ifndef WRAPPER_H
#define WRAPPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sample_props.h"

#define MAX_NB_TRIALS 400
#define NB_OF_FEATURES NATIV_NB_SAMPLES 
#define MAX_LINE_SIZE 13000

typedef struct Swrapper_prop 
{

	char filename[200];
	int nb_trials;

}t_wrapper_prop;

typedef struct Strial {

	int label;
	float features[NB_OF_FEATURES];

}t_trial;

typedef struct Swrapper {
	
	struct Swrapper_prop own_prop;
	struct Strial trial_list[MAX_NB_TRIALS];

}t_wrapper;

t_wrapper *init_wrapper(t_wrapper_prop init_prop);

void kill_wrapper(t_wrapper *pwrapper);

int get_nb_samples(t_wrapper *pwrapper);

void get_random_trial(t_wrapper *pwrapper, float *input, int *output);

void get_trial(t_wrapper *pwrapper, float *input, int *output, int trial_ID);

#endif
