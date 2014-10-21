#ifndef WRAPPER_H
#define WRAPPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NB_TRIALS 4000
#define NB_OF_NEURONS 48
#define MAX_LINE_SIZE 1000



struct SWrapperProp
{
	char FileName[200];
	int NbOfTrials;
};

struct SWrapper;

struct SWrapper* InitWrapper(struct SWrapperProp InitProp);
void KillWrapper(struct SWrapper* pWrapper);

int GetNbOfSample(struct SWrapper* pWrapper);
void GetRandomTrial(struct SWrapper* pWrapper, float (*Input)[48], float (*Output)[8]);

void GetTrial(struct SWrapper* pWrapper, float (*Input)[48], float (*Output)[8], int TrialID);

#endif
