#ifndef WRAPPER_H
#define WRAPPER_H
 /**
 * @file wrapper.h
 * @author Frédéric Simard
 * @date October, 2014
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
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NB_TRIALS 4000
#define NB_OF_NEURONS 48
#define MAX_LINE_SIZE 1000

/** 
 * Properties of the wrapper struct 
 */
struct SWrapperProp {

	/*filename of the dataset */
	char FileName[200];

	/*number of trials in the dataset */
	int NbOfTrials;

};

/** 
 * struct containing a single trial, used only in this file
 */
struct STrial {

	int label;

	float NeuronActivation[NB_OF_NEURONS];

};

/** 
 * Struct wrapper itself
 */
struct SWrapper {

	struct SWrapperProp OwnProp;

	struct STrial TrialList[MAX_NB_TRIALS];

};

struct SWrapper *InitWrapper(struct SWrapperProp InitProp);

void KillWrapper(struct SWrapper *pWrapper);

void GetRandomTrial(struct SWrapper *pWrapper, float (*Input)[48],
		    float (*Output)[8]);

void GetTrial(struct SWrapper *pWrapper, float (*Input)[48], float (*Output)[8],
	      int TrialID);

#endif
