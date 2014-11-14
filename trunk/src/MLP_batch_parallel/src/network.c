/**
 * @file network.c
 * @author Frédéric Simard
 * @date October, 2014
 * @brief  MultiLayer perceptron implementation and trained using back-prop
 */

#include <include/network.h>

/**
 * InitNetwork(struct SNNetworkProp InitProp)
 * 
 * @brief
 * @param InitProp
 * @return NULL if error, SNNetwork struct if okay
 */ 
struct SNNetwork *InitNetwork(struct SNNetworkProp InitProp)
{
	int i = 0;
	int j = 0;
	int TotNumberOfHiddenWeights = 0;
	int TotNumberOfOutputWeights = 0;
	struct SNNetwork *pNNetwork = (struct SNNetwork *)malloc(sizeof(struct SNNetwork));
	pNNetwork->OwnProp = InitProp;

	/* these two aren't required to be initialized by the user */
	pNNetwork->OwnProp.TotNumberOfNeurons = 0;
	pNNetwork->OwnProp.TotNumberOfWeights = 0;

	/* 
	 * Allocate memory for all neurons and
	 * compute the total number of units 
	 * */
	for (i = 0; i < InitProp.NbOfLayer; i++) {
		pNNetwork->OwnProp.TotNumberOfNeurons += InitProp.NbOfNeurons[i];
	}

	/* allocate memory */
	pNNetwork->NeuronsActivity = (float *)malloc(sizeof(float) * pNNetwork->OwnProp.TotNumberOfNeurons);

	/* init the activity of all neurons */
	for (i = 0; i < pNNetwork->OwnProp.TotNumberOfNeurons; i++) {
		pNNetwork->NeuronsActivity[i] = 0.0;
	}

	/* allocate memory for weights and
	 * Compute the total number of weights 
	 */
	for (i = 1; i < InitProp.NbOfLayer; i++) {
		/*weights connecting neurons */
		pNNetwork->OwnProp.TotNumberOfWeights += InitProp.NbOfNeurons[i - 1] * InitProp.NbOfNeurons[i];
		/*weights connecting biases */
		pNNetwork->OwnProp.TotNumberOfWeights += InitProp.NbOfNeurons[i];
	}

	/*allocate memory */
	pNNetwork->Weights = (float *)malloc(sizeof(float) * pNNetwork->OwnProp.TotNumberOfWeights);

	/*All weights and biases */
	TotNumberOfHiddenWeights =
	    InitProp.NbOfNeurons[Input] * InitProp.NbOfNeurons[Hidden] + InitProp.NbOfNeurons[Hidden];
	TotNumberOfOutputWeights =
	    InitProp.NbOfNeurons[Hidden] * InitProp.NbOfNeurons[Output] + InitProp.NbOfNeurons[Output];

	/* intialize the Hidden weights between 0 and 0.1 */
	for (i = 0; i < TotNumberOfHiddenWeights; i++) {
		pNNetwork->Weights[j++] = ((((float)rand()) / (float)RAND_MAX) * (float)0.2) - 0.1;
	}

	for (i = 0; i < TotNumberOfOutputWeights; i++) {
		pNNetwork->Weights[j++] = ((((float)rand()) / (float)RAND_MAX) * (float)0.002) - .001;
	}

	/* allocate the memory for the deltas, used in back propagation output layer */
	pNNetwork->OutputDeltas = (float *)malloc(sizeof(float) * InitProp.NbOfNeurons[Output]);

	for (i = 0; i < InitProp.NbOfNeurons[Output]; i++) {
		pNNetwork->OutputDeltas[i] = 0.0;
	}
	/* hidden layer */
	pNNetwork->HiddenDeltas = (float *)malloc(sizeof(float) * InitProp.NbOfNeurons[Hidden]);

	for (i = 0; i < InitProp.NbOfNeurons[Output]; i++) {
		pNNetwork->OutputDeltas[i] = 0.0;
	}

	return pNNetwork;
}


/**
 * CopyWeights(struct SNNetwork *pNNetwork_dest, const struct SNNetwork *pNNetwork_src)
 * 
 * @brief This function copy the weights from the source network to the destination network. 
 *        It makes sure that the two networks have the same number of weights before copying them. 
 * @param pNNetwork_dest, the network to which we want to copy the weights
 * @param pNNetwork_src, the network from which we want to copy the weights
 * @return 1, if success; 0, if error
 */ 
int CopyWeights(struct SNNetwork *pNNetwork_dest, const struct SNNetwork *pNNetwork_src)
{
	/*Verifies that both networks have the same number of weights*/
	if pNNetwork_src->OwnProp.TotNumberOfWeights != pNNetwork_dest->OwnProp.TotNumberOfWeights
		return 0;

	/*copy the weights and return*/
	memcpy(pNNetwork_dest->Weights,pNNetwork_src->Weights,sizeof(float)*pNNetwork_src->OwnProp.TotNumberOfWeights);
	return 1;
}



/**
 * KillNetwork(struct SNNetwork *pNNetwork)
 * 
 * @brief cleanup dynamic allocated structs in memory
 * @param pNNetwork
 */
void KillNetwork(struct SNNetwork *pNNetwork)
{
	free(pNNetwork->NeuronsActivity);
	free(pNNetwork->Weights);
	free(pNNetwork->HiddenDeltas);
	free(pNNetwork->OutputDeltas);
	free(pNNetwork);
}

/**
 * SetInput(struct SNNetwork *pNNetwork, float *InputActivity)
 * 
 * @brief
 * @param pNNetwork
 * @param InputActivity
 */ 
void SetInput(struct SNNetwork *pNNetwork, float *InputActivity)
{
	int i = 0;
	for (i = 0; i < pNNetwork->OwnProp.NbOfNeurons[Input]; i++) {
		pNNetwork->NeuronsActivity[i] = InputActivity[i];
	}
}

/**
 * ComputeNetwork(struct SNNetwork *pNNetwork)
 * 
 * @brief Feedforward propogation of activation. The activity of the input layer
 * should already have been set using SetInput()
 * This function will compute the activity of all cells down inside
 * output layer 
 * @param pNNetwork
 */
void ComputeNetwork(struct SNNetwork *pNNetwork)
{
	int i, j = 0;
	int WeightPointer = 0;
	int NeuronPointer = 0;
	int HiddenLayerOffset = 0;
	int OutputLayerOffset = 0;
	float InputSum;

	/* 
	 * Compute activation for hidden layer 
	 * Offset over the Input Layer 
	 */
	NeuronPointer = pNNetwork->OwnProp.NbOfNeurons[Input];
	float *NeuronsActivity = pNNetwork->NeuronsActivity;
	float *Weights = pNNetwork->Weights;
	struct SNNetworkProp OwnProp = pNNetwork->OwnProp;

	/* For all hidden unit */
	for (i = 0; i < OwnProp.NbOfNeurons[Hidden]; i++) {
		/* sum inputs from input layer */
		InputSum = 0;
		for (j = 0; j < OwnProp.NbOfNeurons[Input]; j++) {
			InputSum += Weights[WeightPointer++] * NeuronsActivity[j];
		}
		/* plus the connection to the bias */
		InputSum += Weights[WeightPointer++] * OwnProp.HiddenLayerBias;

		/* sigmoid non-nonlinearity */
		NeuronsActivity[NeuronPointer++] = SigmoidFunction(InputSum);
	}

	/* 
	 * Compute Activation for output layer 
	 * precompute the limits of sources (in hidden layer) 
	 */
	HiddenLayerOffset = OwnProp.NbOfNeurons[Input];
	OutputLayerOffset = OwnProp.NbOfNeurons[Input] + OwnProp.NbOfNeurons[Hidden];

	/* For all output layer */
	for (i = 0; i < OwnProp.NbOfNeurons[Output]; i++) {
		/* sum all inputs */
		InputSum = 0;
		for (j = HiddenLayerOffset; j < OutputLayerOffset; j++) {
			InputSum += Weights[WeightPointer++] * NeuronsActivity[j];
		}
		InputSum += Weights[WeightPointer++] * OwnProp.OutputLayerBias;

		/* sigmoid non-nonlinearity */
		NeuronsActivity[NeuronPointer++] = SigmoidFunction(InputSum);
	}
}

/** 
 * BackPropError(struct SNNetwork *pNNetwork, float *DesiredOutput)
 * 
 * @brief Back propagation of error algorithm
 * We start by computing the deltas of the output and hidden layers
 * and then we compute the changes to be applied to the weights. 
 * These are returned and not applied directly.
 * @param pNNetwork Pointer to the network
 * @param DesiredOutput The output desired
 * @param WeightUpdate A memory area where to accumulate the changes
 */
void BackPropError(struct SNNetwork *pNNetwork, float *DesiredOutput, float *WeightUpdate)
{
	int i, j = 0;
	float SumOfWeightedOutputs = 0.0;

	/* Computes the offsets */
	int HiddenActOffset = pNNetwork->OwnProp.NbOfNeurons[0];
	int OutputLayerOffset = pNNetwork->OwnProp.NbOfNeurons[Input] + pNNetwork->OwnProp.NbOfNeurons[Hidden];

	/* Get Local pointers to reduce indirect memory accesses */
	float *InputLayer = &(pNNetwork->NeuronsActivity[0]);
	float *HiddenLayer = &(pNNetwork->NeuronsActivity[HiddenActOffset]);
	float *OutputLayer = &(pNNetwork->NeuronsActivity[OutputLayerOffset]);
	float *OutputDeltas = &(pNNetwork->OutputDeltas[0]);
	float *HiddenDeltas = &(pNNetwork->HiddenDeltas[0]);
	struct SNNetworkProp OwnProp = pNNetwork->OwnProp;

	/* 
	 * Compute the deltas 
	 * Compute the deltas for the output layer 
	 * Delta for output = F'(Yo)*(To-Yo) 
	 * Do = Gain*(To-Yo) 
	 * To = Target(or desired) output 
	 * Yo = actual output 
	 */
	for (i = 0; i < OwnProp.NbOfNeurons[Output]; i++) {
		OutputDeltas[i] = (DesiredOutput[i] - OutputLayer[i]);
	}

	/*Compute the deltas for the hidden layer */
	/*precompute the offsets */
	int HidToOutWeightOffset =
	    OwnProp.NbOfNeurons[Hidden] * OwnProp.NbOfNeurons[Input] + OwnProp.NbOfNeurons[Hidden];
	float *HidToOutLayerWeights = &(Weights[HidToOutWeightOffset]);
	int JumpOffset = OwnProp.NbOfNeurons[Hidden] + 1;

	/* 
	 * Delta = F'(Yh)*SumForAllOutput(Do*Who) 
	 * Delta = F'(Yh)*E(Do*Who)
	 * Delta = Yh*(1-Yh)*E(Do*Who) 
	 */
	for (i = 0; i < OwnProp.NbOfNeurons[Hidden]; i++) {
		SumOfWeightedOutputs = 0.0;
		for (j = 0; j < OwnProp.NbOfNeurons[Output]; j++) {
			SumOfWeightedOutputs += OutputDeltas[j] * HidToOutLayerWeights[i + j * JumpOffset];
		}

		HiddenDeltas[i] = HiddenLayer[i] * (1 - HiddenLayer[i]) * SumOfWeightedOutputs;
	}

	/* 
	 * Compute the new weights 
	 * We proceed backward, (usually it goes from output to hidden) 
	 * but we precomputed the deltas, so we can go from input to output 
	 * this is slightly more consistent with the cache memory and easier on computation 
	 */
	int WeightOffset = 0;

	/* 
	 * Weights between the input and hidden layer
	 * dWih = N*Dh*Yi 
	 */
	for (i = 0; i < OwnProp.NbOfNeurons[Hidden]; i++) {
		for (j = 0; j < OwnProp.NbOfNeurons[Input]; j++) {
			WeightUpdate[WeightOffset++] += OwnProp.LearningRate * HiddenDeltas[i] * InputLayer[j];
		}
		WeightUpdate[WeightOffset++] += OwnProp.LearningRate * HiddenDeltas[i] * OwnProp.HiddenLayerBias;
	}

	/* 
	 * Weights between the hidden and output layer 
	 * dWho = N*Do*Yh 
	 */
	for (i = 0; i < OwnProp.NbOfNeurons[Output]; i++) {
		for (j = 0; j < OwnProp.NbOfNeurons[Hidden]; j++) {
			WeightUpdate[WeightOffset++] += OwnProp.LearningRate * OutputDeltas[i] * HiddenLayer[j];
		}
		WeightUpdate[WeightOffset++] += OwnProp.LearningRate * OutputDeltas[i] * OwnProp.OutputLayerBias;
	}
}


/** 
 * ApplyWeightUpdate(struct SNNetwork *pNNetwork, float *WeightUpdate)
 * 
 * @brief Update of weights values
 * We take the updates received in input and apply them to the weights of the network
 * @param pNNetwork Pointer to the network
 * @param WeightUpdate A memory area where the changes to be applied are located
 */
 void ApplyWeightUpdate(struct SNNetwork *pNNetwork, float *WeightUpdate)
{
	int i,j = 0;
	int WeightOffset = 0;
	struct SNNetworkProp OwnProp = pNNetwork->OwnProp;
	float *Weights = &(pNNetwork->Weights[0]);

	for (i = 0; i < OwnProp.NbOfNeurons[Hidden]; i++) {
		for (j = 0; j < OwnProp.NbOfNeurons[Input]; j++) {
			Weights[WeightOffset] += WeightUpdate[WeightOffset];
			WeightOffset++;
		}
	}
	
	for (i = 0; i < OwnProp.NbOfNeurons[Output]; i++) {
		for (j = 0; j < OwnProp.NbOfNeurons[Hidden]; j++) {
			Weights[WeightOffset] += WeightUpdate[WeightOffset];
			WeightOffset++;
		}
	}
}

/** 
 * GetOutput(struct SNNetwork *pNNetwork, float *OutputActivity)
 * 
 * @brief Get output
 * Copy the output layer into the memory area provided by the user
 * @param pNNetwork
 * @param OutputActivity
 */
void GetOutput(struct SNNetwork *pNNetwork, float *OutputActivity)
{
	int i = 0;
	int OutputOffset = pNNetwork->OwnProp.TotNumberOfNeurons - pNNetwork->OwnProp.NbOfNeurons[Output];
	float *OutputLayer = &(pNNetwork->NeuronsActivity[OutputOffset]);

	for (i = 0; i < pNNetwork->OwnProp.NbOfNeurons[Output]; i++) {
		OutputActivity[i] = OutputLayer[i];
	}

}

/**
 * SigmoidFunction(float x)
 * 
 * @brief the non-linear function
 * @param x
 * @return float
 */
static inline float SigmoidFunction(float x)
{
	return 1 / (1 + exp(-x));
}
