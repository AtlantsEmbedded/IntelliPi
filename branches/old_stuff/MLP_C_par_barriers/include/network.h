#ifndef NETWORK_H
#define NETWORK_H
 /** 
 * @file network.h
 * @author Frédéric Simard
 * @date October, 2014
 * 
 * @brief This file implements the parallel processing solution for a multilayer perceptron (MLP).  
 *  To get it to work, you need to run the following: 
 *  *** training
 *  - call to InitNetwork(), with all the properties set for your need.
 *  - call to SetInput(), to define the state of the first layer.
 *  - call to ComputeNetwork(), to execute the forward pass.
 *  - call to BackPropError(), to execute one backward pass (you need to feed in the desired output)
 *  - repeat
 * 
 *  *** test
 *  - call to SetInput(), to define the state of the first layer.
 *  - call to ComputeNetwork(), to execute the forward pass.
 *  - call to GetOutput(), to get the state of the output layer.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <errno.h>
#include <include/ticket_barrier.h>

#define NB_WORK_THREADS 4
#define VERBOSITY 1

/** Layer IDs*/
enum TypeOfNeuron {
	Input,
	Hidden,
	Output
};

/** Properties of the network
 * used to initialize the network and during computations*/
struct SNNetworkProp {
	int NbOfLayer;		/*must be 3 */
	int NbOfNeurons[3];
	int TotNumberOfNeurons;	/*Don't need to be initialized */
	int TotNumberOfWeights;	/*Don't need to be initialized */

	float OutputLayerBias;	/*set to 1 */
	float HiddenLayerBias;	/*set to 1 */

	float LearningRate;	/*set to 0.001-0.05 */
};

/** Defines the configuration of a worker threads*/
enum TypeOfJobs {
	ForwardProp,		/*forward pass */
	BackProp,		/*backward pass */
	Idle			/*do nothing */
};

/** This struct is known to all thread and is used to access
 * synchronization mechanisms*/
struct SSyncStuff {
	sem_t WorkerGo_sem;	/* the semaphore giving the go signal */
	pthread_barrier_t AllInSync_barrier;	/* synchronization barrier */
};

/** Initialization parameters for worker threads*/
struct SWorkerThreadInitParams {

	struct SNNetworkProp OwnProp;	/*a copy of the network properties */

	char IsAlive;		/*state variable for */
	int TypeOfJobToDo;	/*current thread config */

	int NbNeuronsToCompute[3];

	float *AllInputActivity;	/*direct access to input neuron activations (for reading) */
	float *AllHiddenActivity;	/*direct access to hidden neuron activations (for reading) */
	float *AllOutputActivity;	/*direct access to output neuron activations (for reading) */

	float *ToComputeHiddenActivity;	/*direct access to hidden neuron activations (for writing) */
	float *ToComputeOutputActivity;	/*direct access to output neuron activations (for writing) */

	float *HiddenWeights;	/*direct access to input-hidden weights */
	float *OutputWeights;	/*direct access to hidden-output weights */

	float *HiddenDeltas;	/*direct access to hidden layer deltas */
	float *OutputDeltas;	/*direct access to output layer deltas */

	struct SSyncStuff *pSyncStuff;	/*pointer to sync mechanism */
};

/** Parameters for main (or master) thread */
struct SMasterThreadParams {
	
	pthread_t WorkerThreads[NB_WORK_THREADS];	/*thread handles */
	struct SWorkerThreadInitParams WorkerThreadParams[NB_WORK_THREADS];	/*thread params handles */
	struct SSyncStuff SyncStuff;	/*sync variables instance */
};

/** the neural network */
struct SNNetwork {
	struct SNNetworkProp OwnProp;	/*properties of the network */
	float *Weights;		/*all weights in a vector */
	float *NeuronsActivity;	/*neurons activation */

	float *OutputDeltas;	/*deltas used during the backprop */
	float *HiddenDeltas;	/*deltas used during the backprop */

	struct SMasterThreadParams MasterThreadParams;	/*a copy of the params for the main thread */
};

struct SNNetwork *InitNetwork(struct SNNetworkProp InitProp);

void KillNetwork(struct SNNetwork *pNNetwork);

void SetInput(struct SNNetwork *pNNetwork, float *InputActivity);

void ComputeNetwork(struct SNNetwork *pNNetwork);

void BackPropError(struct SNNetwork *pNNetwork, float *DesiredOutput);

void GetOutput(struct SNNetwork *pNNetwork, float *OutputActivity);

static inline float SigmoidFunction(float x);

void *WorkerThread(void *params);

void TerminateParProc(struct SNNetwork *pNNetwork);
#endif
