

#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>


void InitTimers(void);

void ResetTimer(int TimerNb);
void StartTimer(int TimerNb);
void StopTimer(int TimerNb);
void PrintTimer(int TimerNb);


#endif