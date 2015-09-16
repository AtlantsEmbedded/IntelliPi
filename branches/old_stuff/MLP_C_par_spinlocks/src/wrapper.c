 /**
 * @file  Wrapper.h
 * @author Frédéric Simard
 * @brief October, 2014
 *
 * @brief This file implements an interface with the dataset.  
 * To get it to work, you need to run the following: 
 *
 * - call to InitWrapper(), with all the properties set for your needs.
 *
 * you can then get access to one trial at a time using:
 * - call to GetRandomTrial(), which returns one trial picked at random
 * - call to GetTrial(), which returns the specified trial.
 *
 * please clean after yourself by:
 * - call to KillWrapper(), to free the memory.
 *
 */
#include <include/wrapper.h>

/**
 * InitWrapper(struct SWrapperProp InitProp)
 * @brief Init the wrapper by reading and storing the infos from the data file
 * @param InitProp
 * @return NULL if error, non-NULL if  okay
 */
struct SWrapper *InitWrapper(struct SWrapperProp InitProp)
{

	register int LineCount = 0;

	FILE *pTextFile = NULL;

	register int ii = 0;

	/*allocate memory */
	struct SWrapper *pWrapper =  (struct SWrapper *)malloc(sizeof(struct SWrapper));

	/*copy the prop */
	pWrapper->OwnProp = InitProp;

	/*read the file */
	pTextFile = fopen(InitProp.FileName, "r");

	int ret __attribute__ ((unused));

	/*if valid file */
	if (pTextFile) {

		/*read the required number of trials */
		while (LineCount < pWrapper->OwnProp.NbOfTrials) {

			/*parse the file */
			/*first the label */
			ret =
			    fscanf(pTextFile, "%i",
				   &pWrapper->TrialList[LineCount].label);

			/*then the state of the neurons */
			for (ii = 0; ii < NB_OF_NEURONS; ii++) {

				ret =
				    fscanf(pTextFile, "%f ",
					   &(pWrapper->TrialList
					     [LineCount].NeuronActivation[ii]));

			}

			/*count the progression */
			LineCount++;

		}

		fclose(pTextFile);

	}

	else {

		printf("Filename invalid");

	}

	return pWrapper;

}

/**
 * KillWrapper(struct SWrapper* pWrapper)
 * 
 * @brief free the wrapper
 * @param pWrapper
 */
void KillWrapper(struct SWrapper *pWrapper)
{

	free(pWrapper);

}

/**
 * GetNbOfSample(struct SWrapper* pWrapper)
 * 
 * @brief Get access to the property
 * @param pWrapper
 * @return int numer of trials
 */
static inline int GetNbOfSample(struct SWrapper *pWrapper)
{

	return pWrapper->OwnProp.NbOfTrials;

}

/**
 * @brief Get a trial at random
 * @param pWrapper
 * @param input
 * @param output
 */
void GetRandomTrial(struct SWrapper *pWrapper, float (*Input)[48],
		    float (*Output)[8])
{

	register int ii = 0;

	int TrialID = rand() % pWrapper->OwnProp.NbOfTrials;

	/*simply copy the state of input neurons */
	memcpy(*Input, pWrapper->TrialList[TrialID].NeuronActivation,
	       sizeof(float) * 48);

	/*generate the output from the label, in a one hot scheme */
	for (ii = 0; ii < 8; ii++) {

		(*Output)[ii] = 0;

		if (ii == pWrapper->TrialList[TrialID].label - 1) {

			(*Output)[ii] = 1;

		}

	}

}

/**
 * GetTrial(struct SWrapper *pWrapper, float (*Input)[48], float (*Output)[8],
	      int TrialID)
 * 
 * @brief Get a specific trial
 * @param pWrapper
 * @param input
 * @param output
 * @param TrialID
 */
void GetTrial(struct SWrapper *pWrapper, float (*Input)[48], float (*Output)[8],
	      int TrialID)
{

	register int ii = 0;

	/*simply copy the state of input neurons */
	memcpy(*Input, pWrapper->TrialList[TrialID].NeuronActivation,
	       sizeof(float) * 48);

	/*generate the output from the label, in a one hot scheme */
	for (ii = 0; ii < 8; ii++) {

		(*Output)[ii] = 0;

		if (ii == pWrapper->TrialList[TrialID].label - 1) {

			(*Output)[ii] = 1;

		}

	}

}
