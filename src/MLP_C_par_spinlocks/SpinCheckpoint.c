//
//
// SpinCheckpoint.h
//
// This file implements a barrier/semaphore like mecanism, but based on the spinlock.
// The advantage of this mecanism over barriers, conditions and semaphore, is that it 
// doesn't put the threads to sleep. It allows for a lower latency on the 
// detection of events. It should be noted, though, that this mecanism will
// impair the execution of other processes running on the system.
//
// 
// This mecanism needs to be used the following way. 
// waiting side:
// If a thread reach a checkpoint, it needs to pay a toll to get through. If the
// checkpoint hasn't enough of opening to let it pass, the thread keep on spinning.
// As soon as the checkpoint as enough opening, the thread pay its toll and move on.
//
// opening side:
// If a thread want to open a checkpoint, it can increment it by either 1 or more.
// This will allow threads that can pay the toll through.
//
// Ex: a worker thread might have to pay only 1 toll to pass, but a master may have to
// pay a toll equal to the number of thread it commands to pass.
//
// Similarly, a worker might only allow 1 opening at a time, while a master might allow
// an opening equal to the number of threads to pass.
//
// To use it, you need to:
// - call init_checkpoints, to initialize the number of desired checkpoints.
// - use wait_on_checkpoints, to wait on a specific check point until it has enough opening to pay its toll.
// - use open_checkpoints, allow a certain amount of opening on the checkpoint.
// - call to kill, when you are done with it.
//
// NOTE: This mecanism leads to a higher variance in the speed of execution of programs using it.
// So far, it has reached its peak performance when the threads were given high priority
// and the program low (-20) niceness. Even then, we still find it to overshoot considerably
// execution time of sequential solutions, once in a while. It is still in development.
//
// Frédéric Simard, 2014
//
#include "SpinCheckpoint.h"

/*definition of the checkpoints*/
struct Scheckpoints
{
	int nb_of_checkpoints;
	int* opening;
	pthread_spinlock_t* spinlock;
	
};

pthread_spinlock_t test;

/*function that inits the checkpoints*/
/*it allow the memory and init all the spinlocks*/
struct Scheckpoints* init_checkpoints(int nb_of_checkpoints)
{
	int i=0;
	/*allow memory*/
	struct Scheckpoints* pcheckpoints = (struct Scheckpoints*)malloc(sizeof(struct Scheckpoints));
	pcheckpoints->nb_of_checkpoints = nb_of_checkpoints;
	pcheckpoints->opening = (int*)malloc(nb_of_checkpoints*sizeof(int));
	pcheckpoints->spinlock = (pthread_spinlock_t*)malloc(nb_of_checkpoints*sizeof(pthread_spinlock_t));
	
	pthread_spin_init(&test, PTHREAD_PROCESS_PRIVATE);
	
	/*init all checkpoints*/
	for(i=0;i<nb_of_checkpoints;i++)
	{
		/*init opening*/
		pcheckpoints->opening[i] = 0;
	
		/*init spinlock*/
		if(pthread_spin_init(&(pcheckpoints->spinlock[i]), PTHREAD_PROCESS_PRIVATE))
		{
			/*if a problem occurs, inform user*/
			printf("could not init checkpoints");
			/*free memory*/
			kill_checkpoints(pcheckpoints);
			/*quit*/		
		    return NULL;
		} 	
	}
	
	/*return the initialized checkpoint*/
	return pcheckpoints;
}

/*Frees the memory used by the checkpoints*/
void kill_checkpoints(struct Scheckpoints* pcheckpoints)
{
	free((void*)pcheckpoints->opening);
	free((void*)pcheckpoints->spinlock);
	free((void*)pcheckpoints);	
}

/*this function blocks and only allow a thread if the checkpoint has enough*/
/*opening for it to pay the toll. Otherwise, it spins on it.*/
void wait_on_checkpoint(struct Scheckpoints* pcheckpoints, int checkpoints_id, int Tool)
{

	/*flag to know when the toll was paid*/
	char tmp_done_waiting=0;
	
	/*wait until the toll is paid*/
	while(!tmp_done_waiting)
	{
		/*acquire the spinlock*/
		if(!pthread_spin_lock(&(pcheckpoints->spinlock[checkpoints_id])))
		{

			/*if there is enough opening*/
			if(pcheckpoints->opening[checkpoints_id] >= Tool)
			{
				/*pay the toll*/
				/*done waiting*/
				tmp_done_waiting = 1;
				pcheckpoints->opening[checkpoints_id] -= Tool;
			}
			/*free the lock*/
			pthread_spin_unlock(&(pcheckpoints->spinlock[checkpoints_id]));
		}
		else
		{
			/*spinlock not free, yield to someone else (rather than sleep)*/
			sched_yield();
		}

	}  

}

/*this call is blocking until the opening has been given*/
void open_checkpoint(struct Scheckpoints* pcheckpoints, int checkpoints_id, int Opening)
{
	/*get the lock*/
	while(pthread_spin_lock(&(pcheckpoints->spinlock[checkpoints_id]))){}
		/*give the checkpoint some opening*/
		pcheckpoints->opening[checkpoints_id] += Opening;
	/*free the lock*/
	pthread_spin_unlock(&(pcheckpoints->spinlock[checkpoints_id]));
	sched_yield();
}
