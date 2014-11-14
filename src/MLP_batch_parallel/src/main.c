/**
 * @file main.c
 * @author Frédéric Simard
 * @date October, 2014
 * @brief This program create and trains a MultiLayer Neural Network (MLP) using
 *        the batch strategy. A single batch iteration consist in presenting several 
 *        inputs in sequence and accumulating the weight changes. Once the batch is over
 *        the changes are applied to the network and another batch iteration take place.
 *
 *        This specific implementation aims at taking advantage of parallel processing to speed
 *        up the training. Single bactch iterations are splitted among worker threads and each 
 *        accumulate weight changes on their own. The weights changes are gathered at the end of 
 * 		  the iteration and update the trunk network, which is then recopied to each thread
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
	float MeanSquareError - 0; /*measured mean square error (temp)*/
	int NbOfWeights = 0; /*temporary variable containing the number of weights of the network*/
	float* WeightsUpdate; /*Buffer for weights update*/

	/*training length definition*/
	int BatchSize = 1000; /*Size of a single batch*/
	int NbOfBatch = 25; /*Nb of batch to be executed*/

	struct Spool_init_struct pool_init_struct;
	struct Sthread_pool *pthread_pool;
	
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
	
	/*create a buffer to keep track of weights update*/
	NbOfWeights = NetworkInitProp.NbOfNeurons[0]*NetworkInitProp.NbOfNeurons[1]+
				  NetworkInitProp.NbOfNeurons[1]*NetworkInitProp.NbOfNeurons[2];

	/* Open file to be used to record score results */
	//FILE *ScoreLog = NULL;
	//ScoreLog = fopen(SCORELOG_NAME, "w");
	
	/*init thread pool*/
	pool_init_struct.nb_iterations = BatchSize/(NB_WORK_THREADS+NB_MASTER_THREADS);
	pool_init_struct.ptrain_dataset = pTrainDataSet;
	pool_init_struct.pnnetwork_trunk_copy = pNetwork;
	pool_init_struct.network_init_prop = NetworkInitProp;
	pool_init_struct.nb_weights = NbOfWeights;
	pthread_pool = init_thread_pool(pool_init_struct);

	/* Begin batch training*/
	
	/*Loop over all batch iteration*/
	for (kk = 0; kk < NbOfBatch; kk++) {
	

		/*Execute batch iteration*/
		exec_batch_iteration(pthread_pool);

			/*comput and log the mean square error*/
			//MeanSquareError = 0;
			//for (jj = 0; jj < 8; jj++) {
			//	MeanSquareError +=
			//	    (TrialSolution[jj] - NetworkOutput[jj]) * (TrialSolution[jj] - NetworkOutput[jj]);
			//}
			//MeanSquareError /= 8; /*average over the number of output*/

			/* Print results to file stream */
			//for (jj = 0; jj < 8; jj++) {
			//	fprintf(ScoreLog, "%f ", TrialSolution[jj]);
			//	fprintf(ScoreLog, "%f \t", NetworkOutput[jj]);
			//}
			//fprintf(ScoreLog, "%f\n", MeanSquareError);

		/*
			Validation: This validate the performance of the network over a set of samples
						not encountered during training.
		*/
		
		int NbOfCorrectTrials = 0; /*nb of correct trials in the dataset (used for statistics)*/
		float MaxResponse = 0;  /*max output response*/
		int MaxResponseID = 0;  /*label identified by the network*/

		/*go over the validation set*/
		for (ii = 0; ii < 1000; ii++) {

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
		printf("TestSet Score = %.2f\n", (float)NbOfCorrectTrials / 1000);
	}

	/*close log file*/
	//fclose(ScoreLog);

	/* cleanup memory*/
	KillWrapper(pTrainDataSet);
	KillWrapper(pTestDataSet);
	kill_thread_pool(pthread_pool);
	KillNetwork(pNetwork);

	/*inform user that all is done and quit*/
	printf("Program Over!\n");
	return 0;

}
