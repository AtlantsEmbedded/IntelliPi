/**
 * @file rwalk_process.c
 * @author Frederic Simard (fred.simard@atlantsembedded.com) | Atlants Embedded
 * @brief Random walk process
 * 		  This file implements a random walk process service. This type of system is used to
 * 		  generate variability in the responses, because it is a noisy integrator, while allowing
 *        an easy way to modulate the integration rate.
 * 
 * 		  Randomness and difficulty level can be adjusted on initialization
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "rwalk_process.h"

double randn();

/**
 * int init_rwalk_process(rwalk_t *rwalk)
 * @brief initialize the random walk process
 * @param rwalk, reference to the random walk process
 * @return EXIT_SUCCESS
 */
int init_rwalk_process(rwalk_t *rwalk){
	
	rwalk->decision_value = 0.0;
	return EXIT_SUCCESS;
}

/**
 * int reset_rwalk_process(rwalk_t *rwalk)
 * @brief reset the random walk process
 * @param rwalk, reference to the random walk process
 * @return EXIT_SUCCESS
 */
int reset_rwalk_process(rwalk_t *rwalk){
	
	rwalk->decision_value = 0.0;
	return EXIT_SUCCESS;
}

/**
 * int iterate_rwalk_process(rwalk_t *rwalk, double drift_rate)
 * @brief This implements a brownian motion that randomly drift in the
 * 		  direction and at the rate defined by the drift rate
 * 	      noise is set as an option of the rwalk process
 * @param rwalk, reference to the random walk process
 * @param drift_rate, the average drift rate that should be applied to the process
 * @return EXIT_SUCCESS
 */
int iterate_rwalk_process(rwalk_t *rwalk, double drift_rate){
	
	/*function itself*/
	rwalk->decision_value += (randn()*rwalk->drift_rate_std+drift_rate)*rwalk->dt;
	
	/*keep value positive, since we don't want to get randomly too far from the goal*/
	if(rwalk->decision_value<0.0){
		rwalk->decision_value = 0;
	}
	return EXIT_SUCCESS;	
}


/**
 * double randn()
 * @brief Utility function to generate a normally distributed number
 * @return a standard normally distributed number
 */ 
double randn(){
	double Twopi = (6.2831853071795864769252867665590057683943387987502); /* 2 * pi */
	double randmax = RAND_MAX;
	double random = rand();
	double randnorm;
	
	/*Calculates a normally distributed random number*/
	random/=randmax;
	randnorm= (sqrt(-2.0*log(random)));
	randnorm*=cos(Twopi*random);
	return randnorm;
}




