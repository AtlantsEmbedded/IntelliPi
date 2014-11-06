/**
 * @file main.c
 * @author Frédéric Simard
 * @date October, 2014
 * @brief Sequential MLP tests
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

	register int ii = 0;
	register int jj = 0;
	register int kk = 0;

	int BatchSize = 1000;

	float Input[48];
	float TrialSolution[8];
	float NetworkOutput[8];
	float MeanSquareError;
	float* WeightsUpdate;

	srand((int)time(NULL));

	printf("Ok, let's get started\n");
	printf("Reading File\n");

	int NbOfBatch = 25;
	
	int NbOfWeights = 0;

	/* Init trandata */
	struct SWrapperProp TrainDataSetInitProp;
	strcpy(TrainDataSetInitProp.FileName, TRAINDATASET_NAME);
	TrainDataSetInitProp.NbOfTrials = 3000;

	/* Testdata init */
	struct SWrapperProp TestDataSetInitProp;
	strcpy(TestDataSetInitProp.FileName, TESTDATASET_NAME);
	TestDataSetInitProp.NbOfTrials = 1000;

	struct SWrapper *pTrainDataSet = InitWrapper(TrainDataSetInitProp);
	struct SWrapper *pTestDataSet = InitWrapper(TestDataSetInitProp);

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

	NbOfWeights = NetworkInitProp.NbOfNeurons[0]*NetworkInitProp.NbOfNeurons[1]+
				  NetworkInitProp.NbOfNeurons[1]*NetworkInitProp.NbOfNeurons[2];
	
	WeightsUpdate = (float*)malloc(sizeof(float)*NbOfWeights);
										
	struct SNNetwork *pNetwork = InitNetwork(NetworkInitProp);

	/* Open file to be used to record score results */
	FILE *ScoreLog = NULL;
	ScoreLog = fopen(SCORELOG_NAME, "w");

	/* Begin batch tests */
	for (kk = 0; kk < NbOfBatch; kk++) {
	
		for (ii = 0; ii < BatchSize; ii++) {

			for (jj = 0; jj < NbOfWeights; jj++) {
				WeightsUpdate[jj] = 0.0;
			}		
		
			GetRandomTrial(pTrainDataSet, &Input, &TrialSolution);
			SetInput(pNetwork, Input);
			ComputeNetwork(pNetwork);
			GetOutput(pNetwork, NetworkOutput);
			BackPropError(pNetwork, TrialSolution, WeightsUpdate);
			ApplyWeightUpdate(pNetwork, WeightsUpdate);
			MeanSquareError = 0;

			for (jj = 0; jj < 8; jj++) {

				MeanSquareError +=
				    (TrialSolution[jj] - NetworkOutput[jj]) * (TrialSolution[jj] - NetworkOutput[jj]);

			}

			MeanSquareError /= 2;

			/* Print results to file stream */
			for (jj = 0; jj < 8; jj++) {

				fprintf(ScoreLog, "%f ", TrialSolution[jj]);\
				fprintf(ScoreLog, "%f \t", NetworkOutput[jj]);

			}

			fprintf(ScoreLog, "%f\n", MeanSquareError);

		}

		int NbOfCorrectTrials = 0;
		float MaxResponse = 0;
		int MaxResponseID = 0;

		for (ii = 0; ii < 1000; ii++) {

			GetTrial(pTestDataSet, &Input, &TrialSolution, ii);
			SetInput(pNetwork, Input);
			ComputeNetwork(pNetwork);
			GetOutput(pNetwork, NetworkOutput);
			MaxResponse = NetworkOutput[0];
			MaxResponseID = 0;

			for (jj = 1; jj < 8; jj++) {

				if (NetworkOutput[jj] > MaxResponse) {

					MaxResponse = NetworkOutput[jj];
					MaxResponseID = jj;

				}

			}

			if (TrialSolution[MaxResponseID] == 1) {

				NbOfCorrectTrials++;

			}

		}

		printf("TestSet Score = %.2f\n", (float)NbOfCorrectTrials / 1000);

	}

	fclose(ScoreLog);

	/* cleanup */
	KillWrapper(pTrainDataSet);
	KillWrapper(pTestDataSet);
	KillNetwork(pNetwork);

	printf("Program Over!\n");

	return 0;

}
