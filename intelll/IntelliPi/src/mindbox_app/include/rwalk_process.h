#ifndef RWALK_PROCESS_H
#define RWALK_PROCESS_H

#define DONE 0x01
#define GOING 0x00

/*Random walk process structure*/
typedef struct rwalk_s{
	double drift_rate_std; /*noise in the integrator, to be set before init*/
	double dt; /*time period, to be set before init*/
	double decision_value; /*current integrator value*/
}rwalk_t;

int init_rwalk_process(rwalk_t *rwalk);
int reset_rwalk_process(rwalk_t *rwalk);
int iterate_rwalk_process(rwalk_t *rwalk, double drift_rate);


#endif
