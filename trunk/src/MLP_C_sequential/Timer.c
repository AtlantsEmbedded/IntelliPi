

#include "Timer.h"

#define NB_OF_TIMER 10

LARGE_INTEGER TimeStart[NB_OF_TIMER], TimeEnd[NB_OF_TIMER], ProcFreq;
double TimeExec[NB_OF_TIMER];


void InitTimers(void)
{
	int ii = 0;
	QueryPerformanceFrequency(&ProcFreq);
	
	for(ii=1;ii<NB_OF_TIMER;ii++)
	{
		TimeExec[ii] = 0.0;
	}
}

void ResetTimer(int TimerNb)
{
	TimeExec[TimerNb] = 0.0;
}


void StartTimer(int TimerNb)
{
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&(TimeStart[TimerNb]));
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
}


void StopTimer(int TimerNb)
{
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&(TimeEnd[TimerNb]));
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	TimeExec[TimerNb] += (double)(TimeEnd[TimerNb].QuadPart - TimeStart[TimerNb].QuadPart)/ProcFreq.QuadPart*1000000;
}

void PrintTimer(int TimerNb)
{
	printf("Timer ID: %i \n",TimerNb);
	printf("Execution Time: %.2f usec\n",TimeExec[TimerNb]);
	//printf("Proc Freq: %.2f GHz\n",(double)ProcFreq.QuadPart/1000000);
}


