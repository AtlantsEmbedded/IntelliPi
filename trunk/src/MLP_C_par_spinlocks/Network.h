#ifndef NETWORK_H
#define NETWORK_H
//
//
// Network.h
//
// This file implements the parallel processing solution for a multilayer perceptron (MLP).  
// To get it to work, you need to run the following: 
// *** training
// - call to InitNetwork(), with all the properties set for your need.
// - call to SetInput(), to define the state of the first layer.
// - call to ComputeNetwork(), to execute the forward pass.
// - call to BackPropError(), to execute one backward pass (you need to feed in the desired output)
// - repeat
//
// *** test
// - call to SetInput(), to define the state of the first layer.
// - call to ComputeNetwork(), to execute the forward pass.
// - call to GetOutput(), to get the state of the output layer.
//
//
// Frédéric Simard, 2014
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <errno.h>
#include "SpinCheckpoint.h"

#define NB_WORK_THREADS 1 /*min=1, max=4*/
#define NB_MASTER_THREADS 1 /*do not change*/
#define WORK_THREADS_OFFSET 1
#define VERBOSITY 1

/*Layer IDs*/
enum TypeOfNeuron
{
	Input,
	Hidden,
	Output
};

/*Properties of the network*/
/*used to initialize the network and during computations*/
struct SNNetworkProp
{
	int NbOfLayer; /*must be 3*/
	int NbOfNeurons[3];
	int TotNumberOfNeurons; /*Don't need to be initialized*/
	int TotNumberOfWeights; /*Don't need to be initialized*/

	float OutputLayerBias; /*set to 1*/
	float HiddenLayerBias; /*set to 1*/

	float LearningRate; /*set to 0.001-0.05*/
};

struct SNNetwork;

/*Call to initialize a new network*/
struct SNNetwork* InitNetwork(struct SNNetworkProp InitProp);
/*Call to terminate the network*/
void KillNetwork(struct SNNetwork* pNNetwork);

/*Call to set the input layer activity*/
void SetInput(struct SNNetwork* pNNetwork, float* InputActivity);
/*Call to propagate the activity to the output layer*/
void ComputeNetwork(struct SNNetwork* pNNetwork);
/*Call to compute the backprop algo*/
void BackPropError(struct SNNetwork* pNNetwork, float* DesiredOutput);

/*Call to get the activity of the output layer*/
/*OutputActivity should be a memory space large enough to contain*/
/*the activity of the output layer*/
void GetOutput(struct SNNetwork* pNNetwork, float* OutputActivity);

#endif
