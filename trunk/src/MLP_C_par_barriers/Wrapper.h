#ifndef WRAPPER_H
#define WRAPPER_H
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NB_TRIALS 4000
#define NB_OF_NEURONS 48
#define MAX_LINE_SIZE 1000


/*properties of the wrapper*/
struct SWrapperProp
{
	/*filename of the dataset*/
	char FileName[200];
	/*number of trials in the dataset*/
	int NbOfTrials;
};

struct SWrapper;

/*function to init the wrapper*/
struct SWrapper* InitWrapper(struct SWrapperProp InitProp);
/*function to kill and clean the wrapper*/
void KillWrapper(struct SWrapper* pWrapper);

/*read the number of trials in the dataset*/
int GetNbOfSample(struct SWrapper* pWrapper);

/*to get a trial at random*/
void GetRandomTrial(struct SWrapper* pWrapper, float (*Input)[48], float (*Output)[8]);
/*to get a specific trial*/
void GetTrial(struct SWrapper* pWrapper, float (*Input)[48], float (*Output)[8], int TrialID);

#endif
