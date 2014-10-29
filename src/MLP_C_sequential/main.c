



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include "Network.h"
#include "Wrapper.h"


int main(void)
{
	
	register int ii=0;
	register int jj=0;
	register int kk=0;
	int TrainingLenght = 1000;
	
	float Input[48];
	float TrialSolution[8];
	float NetworkOutput[8];

	float MeanSquareError;

	srand((int)time(NULL));

	printf("Ok, let's get started\n");
	printf("Reading File\n");
	
	int NbOfBatch = 25; 

	struct SWrapperProp TrainDataSetInitProp;
	strcpy(TrainDataSetInitProp.FileName,"TrainDataSet");
	TrainDataSetInitProp.NbOfTrials = 3000;
	
	struct SWrapperProp TestDataSetInitProp;
	strcpy(TestDataSetInitProp.FileName,"TestDataSet");
	TestDataSetInitProp.NbOfTrials = 1000;
	
	struct SWrapper* pTrainDataSet = InitWrapper(TrainDataSetInitProp);
	struct SWrapper* pTestDataSet = InitWrapper(TestDataSetInitProp);
	
	printf("File Read\n");
	printf("Setting up NN\n");
	
	struct SNNetworkProp NetworkInitProp;

	NetworkInitProp.NbOfLayer = 3;
	NetworkInitProp.NbOfNeurons[0] = 48;
	NetworkInitProp.NbOfNeurons[1] = 24;
	NetworkInitProp.NbOfNeurons[2] = 8;
	
	NetworkInitProp.LearningRate = 0.04;
	NetworkInitProp.HiddenLayerBias = 1.0;
	NetworkInitProp.OutputLayerBias = 1.0;

	struct SNNetwork* pNetwork = InitNetwork(NetworkInitProp);
	
	FILE* ScoreLog;
	ScoreLog = fopen("ScoreLog","w");

	for(kk=0;kk<NbOfBatch;kk++)
	{
		for(ii=0;ii<TrainingLenght;ii++)
		{

			GetRandomTrial(pTrainDataSet,&Input,&TrialSolution);

			SetInput(pNetwork, Input);
			ComputeNetwork(pNetwork);
			GetOutput(pNetwork, NetworkOutput);
			BackPropError(pNetwork, TrialSolution);
			
			MeanSquareError = 0;
			for(jj=0;jj<8;jj++)
			{
				MeanSquareError += (TrialSolution[jj]-NetworkOutput[jj])*(TrialSolution[jj]-NetworkOutput[jj]);
			}

			MeanSquareError/=2;
			
			for(jj=0;jj<8;jj++)
			{
				fprintf(ScoreLog,"%f ",TrialSolution[jj]);
				fprintf(ScoreLog,"%f \t",NetworkOutput[jj]);
			}

			fprintf(ScoreLog,"%f\n",MeanSquareError);

		}


		int NbOfCorrectTrials = 0;
		float MaxResponse = 0;
		int MaxResponseID = 0;


		for(ii=0;ii<1000;ii++)
		{

			GetTrial(pTestDataSet,&Input,&TrialSolution,ii);

			SetInput(pNetwork, Input);
			ComputeNetwork(pNetwork);
			GetOutput(pNetwork, NetworkOutput);

			
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

			if(TrialSolution[MaxResponseID]==1)
			{
				NbOfCorrectTrials++;
			}
				
		}	
		printf("TestSet Score = %.2f\n",(float)NbOfCorrectTrials/1000);
	}
	

	fclose(ScoreLog);
	KillWrapper(pTrainDataSet);
	KillWrapper(pTestDataSet);
	KillNetwork(pNetwork);

	printf("Program Over!\n");

	return 0;
}
