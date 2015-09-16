#ifndef NETWORK_H
#define NETWORK_H


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/** Layer IDs */
enum TypeOfNeuron
{
	Input,
	Hidden,
	Output
};

/**
 * Properties of the network used to initialize the network and during computations
 */
struct SNNetworkProp
{
	int NbOfLayer;
	int NbOfNeurons[3];
	int TotNumberOfNeurons; /* Not required to be initialized*/
	int TotNumberOfWeights; /* Not required to be initialized*/

	float OutputLayerBias;
	float HiddenLayerBias;

	float LearningRate;
};

/** SNNetwork structure */
struct SNNetwork {
	struct SNNetworkProp OwnProp;
	float *Weights;
	float *NeuronsActivity;

	float *OutputDeltas;
	float *HiddenDeltas;
};

static inline float SigmoidFunction(float x);

struct SNNetwork* InitNetwork(struct SNNetworkProp InitProp);

void KillNetwork(struct SNNetwork* pNNetwork);

void SetInput(struct SNNetwork* pNNetwork, float* InputActivity);

int CopyWeights(struct SNNetwork *pNNetwork_dest, const struct SNNetwork *pNNetwork_src);

void ComputeNetwork(struct SNNetwork* pNNetwork);

void BackPropError(struct SNNetwork *pNNetwork, float *DesiredOutput, float *WeightUpdate);

void ApplyWeightUpdate(struct SNNetwork *pNNetwork, float *WeightUpdate);

void GetOutput(struct SNNetwork* pNNetwork, float* OutputActivity);

#endif
