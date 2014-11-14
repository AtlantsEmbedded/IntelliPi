/**
 * @file parallelbatch.c
 * @author Frédéric Simard
 * @date November, 2014
 * @brief This file implements the parallel batch training strategy. Each thread posses
 *        a copy of the network and runs its part of the batch on its own. When all threads
 *        are done, the main thread gather the results and update the trunk network.
 */ 

#include <include/parallelbatch.h>

/**
 * init_thread_pool(struct pool_init_struct)
 * 
 * @brief This function initialize the thread pool that performs the 
 *        batch processing training algo.
 * @param pool_init_struct, initialization parameters of the thread pool
 * @return Sthread_pool, the thread pool
 */
struct Sthread_pool *init_thread_pool(struct Spool_init_struct pool_init_struct)
{
	/*init the pool struct*/
	struct Sthread_pool *pthread_pool = (Sthread_pool*)malloc(sizeof(Sthread_pool));
	pthread_barrierattr_t barrier_attributes;
	pthread_attr_t thread_attr;
	
	/*Init the go semaphore*/
	if(sem_init(&go_sem, 0, 0)){
		printf("Go semaphore couldn't be initialized\n");
	}
	
	/*Init the done barrier*/
	pthread_barrierattr_init(&barrier_attributes);
	if(pthread_barrier_init(&done_barrier, &barrier_attributes, NB_WORK_THREADS + NB_MASTER_THREADS)){
		printf("Done barrier couldn't be initialized\n");
	}
	pthread_barrierattr_destroy(&barrier_attributes);
	
	/*Init threads attribute */
	pthread_attr_init(&thread_attr);
	pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED); /*set them to be detached*/
	
	/*Init the thread params*/
	for(ii=0;ii<(NB_WORK_THREADS + NB_MASTER_THREADS);ii++){
		pthread_pool.thread_params[ii].is_alive = 1;
		pthread_pool.thread_params[ii].nb_iterations = pool_init_struct.nb_iterations;

		pthread_pool.thread_params[ii].ptrain_dataset = pool_init_struct.ptrain_dataset;
		
		pthread_pool.thread_params[ii].pnnetwork_trunk_copy = pool_init_struct.pnnetwork_trunk_copy;
		pthread_pool.thread_params[ii].network_init_prop = pool_init_struct.network_init_prop;
		pthread_pool.thread_params[ii].pnnetwork_own_copy = InitNetwork(pool_init_struct.network_init_prop);;

		pthread_pool.thread_params[ii].nb_weights = pool_init_struct.nb_weights;
		pthread_pool.thread_params[ii].weight_update = (float*)malloc(sizeof(float)*pool_init_struct.nb_weights);
	
		/*If we are above the master thread offset, we create worker threads*/
		if(ii>WORK_THREADS_OFFSET)
		{
			/*create a worker thread*/
			if (pthread_create
				(&(pthread_pool.worker_threads[ii-1]),
				 &thread_attr,
				 &worker_thread,
				 (void *)&(pthread_pool.thread_params[ii])))
			{
				printf("a worker thread couldn't be initialized\n");
			}
		}
	}
	
	return pthread_pool;
}

/**
 * exec_batch_iteration(struct Sthread_pool *pthread_pool)
 * 
 * @brief This function launch the pool which will be executing a batch iteration
 * @param pthread_pool, a pointer to the thread pool
 */
void exec_batch_iteration(struct Sthread_pool *pthread_pool)
{
	/*the master thread uses the trunk directly*/
	struct SNNetwork *pown_nnetwork = pthread_pool->thread_params[0].pnnetwork_trunk_copy;
	float *weight_update = pthread_pool->thread_params[0].weight_update;
	struct SWrapper *ptrain_dataset = pthread_pool->thread_params[0].ptrain_dataset;
	int nb_iterations = pthread_pool->thread_params[0].nb_iterations;
	int nb_weights = pthread_pool->thread_params[0].nb_weights;
	
	/*Each thread computes its batch and accumulate weight on their own*/
	/*Give the go signal*/
	for(ii=0;ii<NB_WORK_THREADS;ii++){
		sem_post(&go_sem);
	}
	
	/*init the weight update buffer*/
	for (jj = 0; jj < nb_weights; jj++) {
		WeightsUpdate[jj] = 0.0;
	}
	
	/*loop over all trials of a single batch iteration*/
	for (ii = 0; ii < nb_iterations; ii++) {

		/*network iteration*/
		GetRandomTrial(ptrain_dataset, &input, &trial_solution); /*get a random input sample*/
		SetInput(pown_nnetwork, input); /*set the input of the network*/
		ComputeNetwork(pown_nnetwork); /*propagate the network activity forward*/
		GetOutput(pown_nnetwork, network_output); /*read the output of the network*/
		BackPropError(pown_nnetwork, trial_solution, weight_update); /*backpropagate the error*/
			
	}
	
	/*wait for all thread to be finished*/
	pthread_barrier_wait(&done_barrier);
	
	/*Gather the weights and update the trunk network*/
	for (ii=WORK_THREADS_OFFSET;ii<(NB_WORK_THREADS+WORK_THREADS_OFFSET);ii++){
		for (jj=0;jj<nb_weights;jj++){
			weight_update += pthread_pool->thread_params[ii].weight_update[jj];
		}
	}
	
	/*apply weight update on the network*/
	ApplyWeightUpdate(pown_nnetwork, weight_update); 
	
}

/**
 * worker_thread(void *params)
 * 
 * @brief This function implements the worker threads. When it gets the go
 *        through the semaphore, it runs a subset of the batch. to quit it require
 *        it's alive variable to be set to 0 and get a go through the semaphore  
 * @param params, the thread parameter
 */
void *worker_thread(void *params)
{	
	/*Make local copy of some parameters to reduce the number of pointer calls*/
	/*and simplify the code*/
	struct SNNetwork *ptrunk_nnetwork = (struct Sthread_params*)params)->pnnetwork_trunk_copy;
	struct SNNetwork *pown_nnetwork = (struct Sthread_params*)params)->network_init_prop;
	float *weight_update = (struct Sthread_params*)params)->weight_update;
	struct SWrapper *ptrain_dataset = (struct Sthread_params*)params)->ptrain_dataset;
	int nb_iterations = (struct Sthread_params*)params)->nb_iterations;
	int nb_weights = pthread_pool->thread_params[0].nb_weights;
	
	float input[48];
	float trial_solution[8];
	float network_output[8];
	
	/*loop in the thread*/
	while(1){
	
		/*wait for the go signal*/
		sem_wait(&go_sem);
				
		/*init the weight update buffer*/
		for (jj = 0; jj < nb_weights; jj++) {
			WeightsUpdate[jj] = 0.0;
		}
	
		/*it got the signal to quit, it returns immediatly*/
		if (!((struct Sthread_params*)params)->is_alive){
			return;
		}
		
		/*makes a copy of the trunk network*/
		CopyWeights(pown_nnetwork, ptrunk_nnetwork);
	
		/*loop over all trials of a single batch iteration*/
		for (ii = 0; ii < nb_iterations; ii++) {

			/*network iteration*/
			GetRandomTrial(ptrain_dataset, &input, &trial_solution); /*get a random input sample*/
			SetInput(pown_nnetwork, input); /*set the input of the network*/
			ComputeNetwork(pown_nnetwork); /*propagate the network activity forward*/
			GetOutput(pown_nnetwork, network_output); /*read the output of the network*/
			BackPropError(pown_nnetwork, trial_solution, weight_update); /*backpropagate the error*/
				
		}	
	
		/*barrier*/
		pthread_barrier_wait(&done_barrier);
	}
}


/**
 * kill_thread_pool(struct Sthread_pool *pthread_pool)
 * 
 * @brief This function free the memory used by the thread pool
 * @param a handle on the thread pool
 */
void kill_thread_pool(struct Sthread_pool *pthread_pool)
{

	/*free the go semaphore*/
	sem_destroy(&go_sem);
	
	/*free the done barrier*/
	pthread_barrier_destroy(&done_barrier);
	
	/*free the memory utilized by the thread params*/
	for(ii=0;ii<(NB_WORK_THREADS + NB_MASTER_THREADS);ii++){
		pthread_pool.thread_params[ii].is_alive = 0;
		KillNetwork(pthread_pool.thread_params[ii].pnnetwork_own_copy);
		free(pthread_pool.thread_params[ii].weight_update);
	}
	
	/*let the worker threads finish*/
	for(ii=0;ii<NB_WORK_THREADS;ii++){
		sem_post(&go_sem);
	}
}









