//
//
// Wrapper.h
//
// This file implements an interface with the dataset.  
// To get it to work, you need to run the following: 
//
// - call to InitWrapper(), with all the properties set for your needs.
//
// you can then get access to one trial at a time using:
// - call to GetRandomTrial(), which returns one trial picked at random
// - call to GetTrial(), which returns the specified trial.
//
// please clean after yourself by:
// - call to KillWrapper(), to free the memory.
//
// Frédéric Simard, 2014
//
#include "Wrapper.h"

/*struct containing a single trial, used only in this file*/
struct STrial
{
	int label;
	float NeuronActivation[NB_OF_NEURONS];
};

/*the wrapper itself*/
struct SWrapper
{
	struct SWrapperProp OwnProp;
	struct STrial TrialList[MAX_NB_TRIALS];
};

/*Init the wrapper by reading and storing the infos from the data file*/
struct SWrapper* InitWrapper(struct SWrapperProp InitProp)
{
	int LineCount = 0;
	FILE* pTextFile;
	int ii = 0;

	/*allocate memory*/
	struct SWrapper* pWrapper = (struct SWrapper*)malloc(sizeof(struct SWrapper));
	/*copy the prop*/
	pWrapper->OwnProp = InitProp;

	/*read the file*/
	pTextFile = fopen(InitProp.FileName,"r");

	/*if valid file*/
	if(pTextFile)
	{
		/*read the required number of trials*/
		while(LineCount < pWrapper->OwnProp.NbOfTrials)
		{
			/*parse the file*/
			/*first the label*/
			fscanf(pTextFile,"%i",&pWrapper->TrialList[LineCount].label);

			/*then the state of the neurons*/
			for(ii=0;ii<NB_OF_NEURONS;ii++)
			{
				fscanf(pTextFile,"%f ", &(pWrapper->TrialList[LineCount].NeuronActivation[ii]));
			}
			
			/*count the progression*/
			LineCount++;
		}
		
		fclose(pTextFile);
	
	}
	else
	{
		printf("Filename invalid");
	}

	return pWrapper;
}

/*free the wrapper*/
void KillWrapper(struct SWrapper* pWrapper)
{
	free(pWrapper);
}

/*get access to the property*/
int GetNbOfSample(struct SWrapper* pWrapper)
{
	return pWrapper->OwnProp.NbOfTrials;
}

/*Get a trial at random*/
void GetRandomTrial(struct SWrapper* pWrapper, float (*Input)[48], float (*Output)[8])
{
	int ii=0;
	int TrialID = rand()%pWrapper->OwnProp.NbOfTrials;
	
	/*simply copy the state of input neurons*/
	memcpy(*Input, pWrapper->TrialList[TrialID].NeuronActivation ,sizeof(float)*48);

	/*generate the output from the label, in a one hot scheme*/
	for(ii=0;ii<8;ii++)
	{
		(*Output)[ii] = 0;

		if(ii == pWrapper->TrialList[TrialID].label-1)
		{
			(*Output)[ii] = 1;
		}	
	}

}

/*Get a specific trial*/
void GetTrial(struct SWrapper* pWrapper, float (*Input)[48], float (*Output)[8], int TrialID)
{

	int ii=0;

	/*simply copy the state of input neurons*/
	memcpy(*Input, pWrapper->TrialList[TrialID].NeuronActivation ,sizeof(float)*48);

	/*generate the output from the label, in a one hot scheme*/
	for(ii=0;ii<8;ii++)
	{
		(*Output)[ii] = 0;

		if(ii == pWrapper->TrialList[TrialID].label-1)
		{
			(*Output)[ii] = 1;
		}	
	}
}

