/**
 * @file wrapper.c
 * @author Frédéric Simard
 * @date October, 2014
 * @brief Sequential MLP tests
 */

#include <include/wrapper.h>

/**
 * InitWrapper(struct SWrapperProp InitProp)
 * 
 * @brief Init the wrapper by reading and storing the infos from the data file
 * @param InitProp
 * @return NUll if error, non-NULL SWrapper struct if success
 */
struct SWrapper *InitWrapper(struct SWrapperProp InitProp)
{

	int LineCount = 0;
	FILE *pTextFile = NULL;
	int ii = 0;
	int ret __attribute__ ((unused));

	struct SWrapper *pWrapper = (struct SWrapper *)malloc(sizeof(struct SWrapper));

	pWrapper->OwnProp = InitProp;

	/* Read the file */
	pTextFile = fopen(InitProp.FileName, "r");

	if (pTextFile) {
		while (LineCount < pWrapper->OwnProp.NbOfTrials) {
			ret = fscanf(pTextFile, "%i", &pWrapper->TrialList[LineCount].label);

			for (ii = 0; ii < NB_OF_NEURONS; ii++) {
				ret = fscanf(pTextFile, "%f ", &(pWrapper->TrialList[LineCount].NeuronActivation[ii]));
			}

			LineCount++;
		}
		fclose(pTextFile);

	} else {
		printf("Filename invalid");
		return NULL;
	}

	return pWrapper;

}

/**
 * KillWrapper(struct SWrapper *pWrapper)
 * 
 * @brief cleans up pWrapper memory
 * @param pWrapper
 */
void KillWrapper(struct SWrapper *pWrapper)
{
	free(pWrapper);
}

/**
 * GetNbOfSample(struct SWrapper *pWrapper)
 * 
 * @brief Get NB of samples
 * @param pWrapper
 * @return number of NB trials
 */
int GetNbOfSample(struct SWrapper *pWrapper)
{
	return pWrapper->OwnProp.NbOfTrials;
}

/**
 * GetRandomTrial(struct SWrapper *pWrapper, float (*Input)[48], float (*Output)[8])
 * 
 * @brief Retrieve random trial
 * @param pWrapper
 * @param input
 * @param output
 */
void GetRandomTrial(struct SWrapper *pWrapper, float (*Input)[48], float (*Output)[8])
{

	register int ii = 0;
	int TrialID = rand() % pWrapper->OwnProp.NbOfTrials;
	memcpy(*Input, pWrapper->TrialList[TrialID].NeuronActivation, sizeof(float) * 48);

	for (ii = 0; ii < 8; ii++) {
		(*Output)[ii] = 0;

		if (ii == pWrapper->TrialList[TrialID].label - 1) {
			(*Output)[ii] = 1;
		}
	}
}

/**
 * GetRandomTrial(struct SWrapper *pWrapper, float (*Input)[48], float (*Output)[8])
 * 
 * @brief Retrieve trial
 * @param pWrapper
 * @param input
 * @param output
 * @param trialID
 */
void GetTrial(struct SWrapper *pWrapper, float (*Input)[48], float (*Output)[8], int TrialID)
{

	int ii = 0;
	memcpy(*Input, pWrapper->TrialList[TrialID].NeuronActivation, sizeof(float) * 48);

	for (ii = 0; ii < 8; ii++) {
		(*Output)[ii] = 0;
		if (ii == pWrapper->TrialList[TrialID].label - 1) {
			(*Output)[ii] = 1;
		}
	}
}
