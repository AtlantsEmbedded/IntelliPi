/**
 * @file wrapper.c
 * @author Frédéric Simard
 * @date October, 2014
 * @brief This file implements a wrapper that reads the samples from an ASCII file.
 *		  It then offers a series of function to grant access to these samples.
 */

#include "wrapper.h"

/**
 * init_wrapper(struct SWrapperProp InitProp)
 * 
 * @brief Init the wrapper by reading and storing the infos from the data file
 * @param InitProp
 * @return NUll if error, non-NULL SWrapper struct if success
 */
t_wrapper *init_wrapper(t_wrapper_prop init_prop)
{

	register int line_count = 0;
	FILE *ptext_file = NULL;
	register int ii = 0;
	int ret __attribute__ ((unused));

	t_wrapper *pwrapper = (t_wrapper *)malloc(sizeof(t_wrapper));

	pwrapper->own_prop = init_prop;

	/* Read the file */
	ptext_file = fopen(init_prop.filename, "r");

	if (ptext_file) {
		while (line_count < pwrapper->own_prop.nb_trials) {
			ret = fscanf(ptext_file, "%i", &pwrapper->trial_list[line_count].label);

			for (ii = 0; ii < NB_OF_FEATURES; ii++) {
				ret = fscanf(ptext_file, "%f ", &(pwrapper->trial_list[line_count].features[ii]));
			}

			line_count++;
		}
		fclose(ptext_file);

	} else {
		printf("filename invalid");
		return NULL;
	}

	return pwrapper;

}

/**
 * kill_wrapper(struct Swrapper *pwrapper)
 * 
 * @brief cleans up pwrapper memory
 * @param pwrapper
 */
void kill_wrapper(t_wrapper *pwrapper)
{
	free(pwrapper);
}

/**
 * get_nb_samples(struct Swrapper *pwrapper)
 * 
 * @brief Get NB of samples
 * @param pwrapper
 * @return number of NB trials
 */
int get_nb_samples(t_wrapper *pwrapper)
{
	return pwrapper->own_prop.nb_trials;
}

/**
 * get_random_trial(struct Swrapper *pwrapper, float *input, float *output)
 * 
 * @brief Retrieve random trial
 * @param pwrapper
 * @param input
 * @param output
 */
void get_random_trial(t_wrapper *pwrapper, float *input, int *output)
{

	register int ii = 0;
	int trial_ID = rand() % pwrapper->own_prop.nb_trials;
	memcpy((void*)input, (void*)pwrapper->trial_list[trial_ID].features, sizeof(float) * NB_OF_FEATURES);

	*output = pwrapper->trial_list[trial_ID].label;
}

/**
 * get_trial(struct Swrapper *pwrapper, float *input, float *output, int trial_ID)
 * 
 * @brief Retrieve trial
 * @param pwrapper
 * @param input
 * @param output
 * @param trial_ID
 */
void get_trial(t_wrapper *pwrapper, float *input, int *output, int trial_ID)
{

	int ii = 0;
	
	memcpy((void*)input, (void*)pwrapper->trial_list[trial_ID].features, sizeof(float) * NB_OF_FEATURES);

	*output = pwrapper->trial_list[trial_ID].label;
}


