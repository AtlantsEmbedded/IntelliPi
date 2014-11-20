/**
 * @file main.c
 * @author Frédéric Simard
 * @date October, 2014
 * @brief This program create and trains a MultiLayer Neural Network (MLP) using
 *        the batch strategy. A single batch iteration consist in presenting several 
 *        inputs in sequence and accumulating the weight changes. Once the batch is over
 *        the changes are applied to the network and another batch iteration take place.
 *
 *        This specific implementation makes only use of sequential execution. It is meant to be
 *        compared with the results obtained with the parallel solution.
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#include <include/network.h>
#include <include/wrapper.h>

#define TRAINDATASET_NAME "data/TrainDataSet"
#define TESTDATASET_NAME "data/TestDataSet"
#define SCORELOG_NAME "Scorelog"

/**
 * main(int argc, char **argv[])
 * 
 * @brief
 * @param argc
 * @param argv
 * @return 0 for success, non-zero for failure
 */ 
int main(int argc, char **argv[])
{
	/*registers used as iterators in loops*/
	register int ii = 0;
	register int jj = 0;
	register int kk = 0;

	float Input[48]; /*buffer for network inputs*/
	float TrialSolution[8]; /*buffer for trial solution*/
	float NetworkOutput[8]; /*buffer for network output*/
	
	int NbOfWeights = 0; /*temporary variable containing the number of weights of the network*/
	float* WeightsUpdate; /*Buffer for weights update*/

	/*training length definition*/
	int BatchSize = 100; /*Size of a single batch*/
	int NbOfBatch = 250; /*Nb of batch to be executed*/
	
	/*variables used to compute performance on validation set*/
	int NbOfCorrectTrials = 0;
	float MaxResponse = 0;
	int MaxResponseID = 0;
	
	srand((int)time(NULL)); /*initialize the random generator*/

	/*inform user of state of the program*/
	printf("Ok, let's get started\n");
	printf("Reading File\n");

	/* Init the train dataset properties*/
	struct SWrapperProp TrainDataSetInitProp;
	strcpy(TrainDataSetInitProp.FileName, TRAINDATASET_NAME);
	TrainDataSetInitProp.NbOfTrials = 3000;

	/* Init the test dataset properties*/
	struct SWrapperProp TestDataSetInitProp;
	strcpy(TestDataSetInitProp.FileName, TESTDATASET_NAME);
	TestDataSetInitProp.NbOfTrials = 1000;

	/*Init both datasets*/
	struct SWrapper *pTrainDataSet = InitWrapper(TrainDataSetInitProp);
	struct SWrapper *pTestDataSet = InitWrapper(TestDataSetInitProp);

	/*update user on state of the program*/
	printf("Setting up NN\n");
	
	/* Initialize NetworkProp struct and setup NN */
	struct SNNetworkProp NetworkInitProp = {0};
	NetworkInitProp.NbOfLayer = 3;
	NetworkInitProp.NbOfNeurons[0] = 48;
	NetworkInitProp.NbOfNeurons[1] = 24;
	NetworkInitProp.NbOfNeurons[2] = 8;
	NetworkInitProp.LearningRate = 0.04;
	NetworkInitProp.HiddenLayerBias = 1.0;
	NetworkInitProp.OutputLayerBias = 1.0;

	struct SNNetwork *pNetwork = InitNetwork(NetworkInitProp);
	
	/*compute the number of weights*/
	NbOfWeights = NetworkInitProp.NbOfNeurons[0]*NetworkInitProp.NbOfNeurons[1]+NetworkInitProp.NbOfNeurons[1]+
				  NetworkInitProp.NbOfNeurons[1]*NetworkInitProp.NbOfNeurons[2]+NetworkInitProp.NbOfNeurons[2];
	
	/*init a buffer to hold weights update*/
	WeightsUpdate = (float*)malloc(sizeof(float)*NbOfWeights);
	
	/*check if valid*/			
	if(!WeightsUpdate){
		printf("Malloc for WeightsUpdate failed");
	}		
	
	/* Begin batch training*/		
							
	/*Loop over all batch iteration*/
	for (kk = 0; kk < NbOfBatch; kk++) {
	
		/*initialize weight update to 0.0*/
		for (jj = 0; jj < NbOfWeights; jj++) {
			WeightsUpdate[jj] = 0.0;
		}		
		
		/*run the program for the number of iteration set by the size of the batch*/	
		for (ii = 0; ii < BatchSize; ii++) {

			GetRandomTrial(pTrainDataSet, &Input, &TrialSolution); /*get a random trial*/
			SetInput(pNetwork, Input); /*get a random trial*/
			ComputeNetwork(pNetwork); /*compute the network*/
			GetOutput(pNetwork, NetworkOutput); /*get the output of the network*/
			BackPropError(pNetwork, TrialSolution, WeightsUpdate); /*Execute the backprop algo, get the weight update*/
		}
		
		/*after the batch, update the weight of the network*/
		ApplyWeightUpdate(pNetwork, WeightsUpdate);	

		/*reinit variables*/
		NbOfCorrectTrials = 0;
		MaxResponse = 0;
		MaxResponseID = 0;
		
		/*go over the validation set*/
		for (ii = 0; ii < 200; ii++) {

			/*Network iteration*/
			GetTrial(pTestDataSet, &Input, &TrialSolution, ii); /*get a sample*/
			SetInput(pNetwork, Input); /*set network input*/
			ComputeNetwork(pNetwork); /*propagate the network*/
			GetOutput(pNetwork, NetworkOutput); /*get the network output*/
			
			/*identify the label found by the network using a max function*/
			MaxResponse = NetworkOutput[0]; 
			MaxResponseID = 0;
			for (jj = 1; jj < 8; jj++) {
				if (NetworkOutput[jj] > MaxResponse) {
					MaxResponse = NetworkOutput[jj];
					MaxResponseID = jj;
				}
			}

			/*check if this correspond to the solution*/
			if (TrialSolution[MaxResponseID] == 1) {
				NbOfCorrectTrials++; /*yes! add one to correct trials count*/
			}

		}
		/*inform user on the progression of the training*/
		printf("TestSet Score = %.2f\n", (float)NbOfCorrectTrials / 200);

	}


	/* cleanup memory*/
	KillWrapper(pTrainDataSet);
	KillWrapper(pTestDataSet);
	KillNetwork(pNetwork);

	/*inform user that all is done and quit*/
	printf("Program Over!\n");

	return 0;

}
