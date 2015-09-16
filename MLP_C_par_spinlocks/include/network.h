#ifndef NETWORK_H
#define NETWORK_H
 /**
 * @file network.h
 * @author Frédéric Simard
 * @date October, 2014
 *
 * @brief This file implements the parallel processing solution for a multilayer perceptron (MLP).  
 * To get it to work, you need to run the following: 
 * 
 * training:
 * - call to InitNetwork(), with all the properties set for your need.
 * - call to SetInput(), to define the state of the first layer.
 * - call to ComputeNetwork(), to execute the forward pass.
 * - call to BackPropError(), to execute one backward pass (you need to feed in the desired output)
 * - repeat
 *
 * test:
 * - call to SetInput(), to define the state of the first layer.
 * - call to ComputeNetwork(), to execute the forward pass.
 * - call to GetOutput(), to get the state of the output layer.

 */
#define _GNU_SOURCE
#include <sched.h>		//cpu_set_t , CPU_SET
#include <pthread.h>		//pthread_t
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <errno.h>
#include <include/spinCheckpoint.h>

#define NB_WORK_THREADS 1	/* min=1, max=4 */
#define NB_MASTER_THREADS 1	/* do not change */
#define WORK_THREADS_OFFSET 1

/**
 *  Layer IDs
 */
enum TypeOfNeuron {
	Input,
	Hidden,
	Output
};

/**
 * Checkpoint names enum
 */
enum CheckPointNames {
	check_start_job,
	check_done_one,
	check_go_two,
	check_done_two,
	check_finish_job
};

/**
 * Defines the configuration of a worker threads
 */
enum TypeOfJobs {
	ForwardProp,		/*forward pass */
	BackProp,		/*backward pass */
	Idle			/*do nothing */
};

/**
 *   Properties of the network used during initialization and computations
 */
struct SNNetworkProp {

	int NbOfLayer;		/*  must be 3  */

	int NbOfNeurons[3];

	int TotNumberOfNeurons;	/* Don't need to be initialized  */

	int TotNumberOfWeights;	/* Don't need to be initialized  */

	float OutputLayerBias;	/* set to 1  */

	float HiddenLayerBias;	/* set to 1  */

	float LearningRate;	/* set to 0.001-0.05  */

};

/** 
 * Initialization parameters for worker threads
 */
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

	struct Scheckpoints *pCheckpoints;	/*checkpoints used for synchronization */
};

/**
 * Parameters for main (or master) thread
 */
struct SMasterThreadParams {
	pthread_t WorkerThreads[NB_WORK_THREADS];	/*thread handles */
	struct SWorkerThreadInitParams WorkerThreadParams[NB_WORK_THREADS + NB_MASTER_THREADS];	/*worker and master thread workload params */

	struct Scheckpoints *pCheckpoints;	/*checkpoints used for synchronization */
};

/**
 * The neural network structure
 */
struct SNNetwork {
	struct SNNetworkProp OwnProp;	/*properties of the network */
	float *Weights;		/*all weights in a vector */
	float *NeuronsActivity;	/*neurons activation */

	float *OutputDeltas;	/*deltas used during the backprop */
	float *HiddenDeltas;	/*deltas used during the backprop */

	struct SMasterThreadParams MasterThreadParams;	/*a copy of the params for the main thread */
};

struct SNNetwork *InitNetwork(struct SNNetworkProp InitProp);

int InitWorkerParams(struct SNNetwork *pNNetwork);

void KillNetwork(struct SNNetwork *pNNetwork);

void SetInput(struct SNNetwork *pNNetwork, float *InputActivity);

void ComputeNetwork(struct SNNetwork *pNNetwork);

void BackPropError(struct SNNetwork *pNNetwork, float *DesiredOutput);

static void *WorkerThread(void *params);

void GetOutput(struct SNNetwork *pNNetwork, float *OutputActivity);

void TerminateParProc(struct SNNetwork *pNNetwork);

static int InitParProc(struct SNNetwork *pNNetwork);
#endif
