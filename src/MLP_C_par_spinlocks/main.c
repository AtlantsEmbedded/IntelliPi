//
//
// main.c
//
// This file implements the program that runs a MLP algorithm on a set of data.
// What it does:
// - load the dataset
// - Train by batch and show the resulting performance
// - The mean squared error is logged and available in ASCII format in the file "ScoreLog"
// - The file also has a timer for profiling purpose
//
// Frédéric Simard, 2014
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include "Network.h"
#include "Wrapper.h"

struct timespec diff(struct timespec start, struct timespec end);

int main(void)
{
	
	int kk=0; /*batch counter*/
	int ii=0; /*training counter*/
	int NbOfBatch = 25; /*nb of batchs*/
	int TrainingLenght = 1000; /*nb of trials per batch*/
	
	int jj=0; /*General Purpose counter*/
	
	float Input[48]; /*this holds the current input*/
	float TrialSolution[8]; /*this holds the current desired output*/
	float NetworkOutput[8]; /*this holds the current desired network output*/

	float MeanSquareError;
	
	srand((int)time(NULL));
	
	/*profiling timer, timestamps*/
	struct timespec time1, time2, tot;

	printf("Ok, let's get started\n");
	printf("Reading File\n");
	
	/*init and read the training data set*/
	struct SWrapperProp TrainDataSetInitProp;
	strcpy(TrainDataSetInitProp.FileName,"TrainDataSet");
	TrainDataSetInitProp.NbOfTrials = 3000;
	struct SWrapper* pTrainDataSet = InitWrapper(TrainDataSetInitProp);
	
	/*init and read the test data set*/
	struct SWrapperProp TestDataSetInitProp;
	strcpy(TestDataSetInitProp.FileName,"TestDataSet");
	TestDataSetInitProp.NbOfTrials = 1000;
	struct SWrapper* pTestDataSet = InitWrapper(TestDataSetInitProp);
	
	printf("File Read\n");
	printf("Setting up NN\n");
	
	/*init the neural network*/
	struct SNNetworkProp NetworkInitProp;

	/*network architecture*/
	NetworkInitProp.NbOfLayer = 3;
	NetworkInitProp.NbOfNeurons[0] = 48;
	NetworkInitProp.NbOfNeurons[1] = 24;
	NetworkInitProp.NbOfNeurons[2] = 8;
	
	NetworkInitProp.LearningRate = 0.04;
	NetworkInitProp.HiddenLayerBias = 1.0; /*will be removed*/
	NetworkInitProp.OutputLayerBias = 1.0; /*will be removed*/

	struct SNNetwork* pNetwork = InitNetwork(NetworkInitProp); /*call to constructor*/
	
	/*init the log file*/
	FILE* ScoreLog;
	ScoreLog = fopen("ScoreLog","w");

	/*get first timer timestamp*/
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
	
	/*for the required number of batchs*/
	for(kk=0;kk<NbOfBatch;kk++)
	{
		/*for the required number of trials*/
		for(ii=0;ii<TrainingLenght;ii++)
		{
			/*get a trial from the training set*/
			GetRandomTrial(pTrainDataSet,&Input,&TrialSolution);
			
			/*define the input to the network*/
			SetInput(pNetwork, Input);
			/*forward pass*/
			ComputeNetwork(pNetwork);
			/*get the network output (for log purpose)*/
			GetOutput(pNetwork, NetworkOutput);
			/*backprop the error*/
			BackPropError(pNetwork, TrialSolution);
			
			/*compute and log the desired output, network output and mean square error*/
			MeanSquareError = 0;
			for(jj=0;jj<8;jj++)
			{
				MeanSquareError += (TrialSolution[jj]-NetworkOutput[jj])*(TrialSolution[jj]-NetworkOutput[jj]);
			}
			/*there are 8 outputs*/
			MeanSquareError/=8;
			
			for(jj=0;jj<8;jj++)
			{
				fprintf(ScoreLog,"%f ",TrialSolution[jj]);
				fprintf(ScoreLog,"%f \t",NetworkOutput[jj]);
			}

			fprintf(ScoreLog,"%f\n",MeanSquareError);

		}

		/*at the end of each batch*/
		/*test the network on the test set*/
		int NbOfCorrectTrials = 0;
		float MaxResponse = 0;
		int MaxResponseID = 0;

		/*run the whole of the test set*/
		for(ii=0;ii<1000;ii++)
		{
			/*get a trial*/
			GetTrial(pTestDataSet,&Input,&TrialSolution,ii);

 			/*execute the network*/
			SetInput(pNetwork, Input);
			ComputeNetwork(pNetwork);
			GetOutput(pNetwork, NetworkOutput);

			/*apply max function on the output*/
			MaxResponse = NetworkOutput[0];
			MaxResponseID = 0;

			for(jj=1;jj<8;jj++)
			{
				if (NetworkOutput[jj]>MaxResponse)
				{
					MaxResponse = NetworkOutput[jj];
					MaxResponseID = jj;
				}
			}
			
			/*check if it matches the trial desired output*/
			if(TrialSolution[MaxResponseID]==1)
			{
				NbOfCorrectTrials++;
			}
				
		}	
		/*show the performance*/
		printf("TestSet Score = %.2f\n",(float)NbOfCorrectTrials/1000);
	}
	
	/*second timestamp*/
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
	/*compute the difference*/
	tot = diff(time1, time2);
	/*show the result on screen*/
	printf("Exec. Time = %d:%d\n",tot.tv_sec,tot.tv_nsec);

	/*clean up*/
	fclose(ScoreLog);
	KillWrapper(pTrainDataSet);
	KillWrapper(pTestDataSet);
	KillNetwork(pNetwork);

	printf("Program Over!\n");

	return 0;
}

/*timer operation used for profiling*/
/*it computes the difference between two time stamps*/
struct timespec diff(struct timespec start, struct timespec end)
{
	struct timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}
