/**
 * @file parallelbatch.h
 * @author Frédéric Simard
 * @date November, 2014
 * @brief This file implements the parallel batch training strategy. Each thread posses
 *        a copy of the network and runs its part of the batch on its own. When all threads
 *        are done, the main thread gather the results and update the trunk network.
 */ 
#define _GNU_SOURCE
#include <sched.h>		//cpu_set_t , CPU_SET
#include <pthread.h>		//pthread_t
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#include <include/wrapper.h>
#include <include/network.h>

#define NB_WORK_THREADS 1	/* min=1, max=4 */
#define NB_MASTER_THREADS 1	/* do not change */
#define WORK_THREADS_OFFSET 1

/**
 * Thread params
 */
struct Sthread_params {

	char is_alive;
	int nb_iterations;
	
	struct SWrapper *ptrain_dataset;
	
	struct SNNetwork *pnnetwork_trunk_copy;
	
	struct SNNetworkProp network_init_prop;
	struct SNNetwork *pnnetwork_own_copy;
	int nb_weights;
	float *weight_update;
};


/**
 * Thread pool structure
 */
struct Sthread_pool{
	
	pthread_t worker_threads[NB_WORK_THREADS];
	struct Sthread_params thread_params[NB_WORK_THREADS + NB_MASTER_THREADS];
};


/**
 * Thread pool init structure
 */
struct Spool_init_struct{
	
	int nb_iterations;

	struct SWrapper ptrain_dataset;
		
	struct SNNetwork *pnnetwork_trunk_copy;
	
	struct SNNetworkProp network_init_prop;

	int nb_weights;
};

sem_t go_sem;
pthread_barrier_t done_barrier;

/*init thread pool*/
struct Sthread_pool *init_thread_pool(struct Spool_init_struct pool_init_struct);

/*Execute batch iteration*/
void exec_batch_iteration(struct Sthread_pool *pthread_pool);

/*kill thread pool*/
void kill_thread_pool(struct Sthread_pool *pthread_pool);
