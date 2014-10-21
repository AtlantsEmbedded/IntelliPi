#ifndef SPINCHECKPOINT_H
#define SPINCHECKPOINT_H
//
//
// SpinCheckpoint.h
//
// This file implements a barrier/semaphore like mecanism, but based on the spinlock.
// The advantage of this mecanism over barriers, conditions and semaphore, is that it 
// doesn't put the threads to sleep. It allows for a lower latency on the 
// detection of events. It should be noted, though, that this mecanism will
// impair the execution of other processes running on the system.
//
// 
// This mecanism needs to be used the following way. 
// waiting side:
// If a thread reach a checkpoint, it needs to pay a toll to get through. If the
// checkpoint hasn't enough of opening to let it pass, the thread keep on spinning.
// As soon as the checkpoint as enough opening, the thread pay its toll and move on.
//
// opening side:
// If a thread want to open a checkpoint, it can increment it by either 1 or more.
// This will allow threads that can pay the toll through.
//
// Ex: a worker thread might have to pay only 1 toll to pass, but a master may have to
// pay a toll equal to the number of thread it commands to pass.
//
// Similarly, a worker might only allow 1 opening at a time, while a master might allow
// an opening equal to the number of threads to pass.
//
// To use it, you need to:
// - call init_checkpoints, to initialize the number of desired checkpoints.
// - use wait_on_checkpoints, to wait on a specific check point until it has enough opening to pay its toll.
// - use open_checkpoints, allow a certain amount of opening on the checkpoint.
// - call to kill, when you are done with it.
//
// NOTE: This mecanism leads to a higher variance in the speed of execution of programs using it.
// So far, it has reached its peak performance when the threads were given high priority
// and the program low (-20) niceness. Even then, we still find it to overshoot considerably
// execution time of sequential solutions, once in a while. It is still in development.
//
// Frédéric Simard, 2014
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <errno.h>

struct Scheckpoints;

struct Scheckpoints* init_checkpoints(int nb_of_checkpoints);
void kill_checkpoints(struct Scheckpoints* pcheckpoints);

void wait_on_checkpoint(struct Scheckpoints* pcheckpoints, int checkpoints_id, int Tool);
void open_checkpoint(struct Scheckpoints* pcheckpointsm, int checkpoints_id, int Opening);

#endif
