
#include "stdio.h"
#include "stdlib.h"

#include <rwalk_process.h>

double v_decision_value = 0.0;
rwalk_options_t v_options;


int init_rwalk_process(rwalk_options_t options){
	v_options = options;
}

int reset_rwalk_process(){
	decision_value = 0.0;
}

char integrate_rwalk_process(double drift_rate){
	
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
}




