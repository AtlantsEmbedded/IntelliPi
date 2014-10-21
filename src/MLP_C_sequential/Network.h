#ifndef NETWORK_H
#define NETWORK_H


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
	int NbOfLayer;
	int NbOfNeurons[3];
	int TotNumberOfNeurons; /*Don't need to be initialized*/
	int TotNumberOfWeights; /*Don't need to be initialized*/

	float OutputLayerBias;
	float HiddenLayerBias;

	float LearningRate;
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
