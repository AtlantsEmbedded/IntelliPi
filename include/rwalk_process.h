#ifndef RWALK_PROCESS_H
#define RWALK_PROCESS_H

#define DONE 0x01
#define GOING 0x00

typedef struct rwalk_options_s{
	double drift_rate_std;
	double dt;
}rwalk_options_t;

int init_rwalk_process(rwalk_options_t options);

int reset_rwalk_process();

double iterate_rwalk_process(double drift_rate);


#endif
