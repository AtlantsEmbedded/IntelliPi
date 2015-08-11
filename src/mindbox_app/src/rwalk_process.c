
#include "stdio.h"
#include "stdlib.h"

#include <rwalk_process.h>

double v_decision_value = 0.0;
rwalk_options_t v_options;


int init_rwalk_process(rwalk_options_t options){
	v_options = options;
	
	return EXIT_SUCCESS;
}

int reset_rwalk_process(){
	v_decision_value = 0.0;
	
	return EXIT_SUCCESS;
}

char iterate_rwalk_process(double drift_rate){
	
#if 0
	/*add a step to the decision value by*/
	/*getting a normaly distributed random number*/
	/*with average equal to drift rate and deviation*/
	/*specified by the options*/
	v_decision_value = randn()*(v_options.drift_rate_std+drift_rate)*v_options.dt;
	
	/*check if threshold is reached*/
	if(v_options.decision_threshold < v_decision_value)
		/*yes, we're done*/
		return DONE;
	else
		/*no, not done yet*/
		return GOING;
#endif
	if(rand()<drift_rate)
		return DONE;
	else
		return GOING;
		
	
}




