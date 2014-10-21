

#include "Wrapper.h"


struct STrial
{
	int label;
	float NeuronActivation[NB_OF_NEURONS];
};

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

	struct SWrapper* pWrapper = (struct SWrapper*)malloc(sizeof(struct SWrapper));
	pWrapper->OwnProp = InitProp;

	/*read the file*/
	pTextFile = fopen(InitProp.FileName,"r");

	if(pTextFile)
	{
		
		while(LineCount < pWrapper->OwnProp.NbOfTrials)
		{
			fscanf(pTextFile,"%i",&pWrapper->TrialList[LineCount].label);

			for(ii=0;ii<NB_OF_NEURONS;ii++)
			{
				fscanf(pTextFile,"%f ", &(pWrapper->TrialList[LineCount].NeuronActivation[ii]));
			}

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

void KillWrapper(struct SWrapper* pWrapper)
{
	free(pWrapper);
}

int GetNbOfSample(struct SWrapper* pWrapper)
{
	return pWrapper->OwnProp.NbOfTrials;
}

void GetRandomTrial(struct SWrapper* pWrapper, float (*Input)[48], float (*Output)[8])
{
	int ii=0;
	int TrialID = rand()%pWrapper->OwnProp.NbOfTrials;

	memcpy(*Input, pWrapper->TrialList[TrialID].NeuronActivation ,sizeof(float)*48);

	for(ii=0;ii<8;ii++)
	{
		(*Output)[ii] = 0;

		if(ii == pWrapper->TrialList[TrialID].label-1)
		{
			(*Output)[ii] = 1;
		}	
	}

}

void GetTrial(struct SWrapper* pWrapper, float (*Input)[48], float (*Output)[8], int TrialID)
{

	int ii=0;

	memcpy(*Input, pWrapper->TrialList[TrialID].NeuronActivation ,sizeof(float)*48);

	for(ii=0;ii<8;ii++)
	{
		(*Output)[ii] = 0;

		if(ii == pWrapper->TrialList[TrialID].label-1)
		{
			(*Output)[ii] = 1;
		}	
	}
}

