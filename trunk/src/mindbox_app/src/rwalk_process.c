
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "rwalk_process.h"

double v_decision_value = 0.0;
rwalk_options_t v_rwalk_options;

double randn();

int init_rwalk_process(rwalk_options_t options){
	v_rwalk_options = options;
	
	return EXIT_SUCCESS;
}

int reset_rwalk_process(){
	v_decision_value = 0.0;
	
	return EXIT_SUCCESS;
}

double iterate_rwalk_process(double drift_rate){
	
	/*add a step to the decision value by*/
	/*getting a normaly distributed random number*/
	/*with average equal to drift rate and deviation*/
	/*specified by the options*/
	
	printf("drift_rate:%f\n",drift_rate);
	printf("DR_std:%f\n",v_rwalk_options.drift_rate_std);
	printf("dt:%f\n",v_rwalk_options.dt);
	
	v_decision_value += (randn()*v_rwalk_options.drift_rate_std+drift_rate)*v_rwalk_options.dt;
	
	printf("v_decision_value:%f\n",v_decision_value);
	
	sleep(1);
	return v_decision_value;		
	
}


/**
 * double randn()
 * @brief Utility function to generate a normally distributed number
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




