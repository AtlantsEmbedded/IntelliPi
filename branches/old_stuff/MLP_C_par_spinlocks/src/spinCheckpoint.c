 /**
 * @file SpinCheckpoint.h
 * @author Frédéric Simard
 * @date October, 2014
 *
 * @brief This file implements a barrier/semaphore like mecanism, but based on the spinlock.
 * The advantage of this mecanism over barriers, conditions and semaphore, is that it 
 * doesn't put the threads to sleep. It allows for a lower latency on the 
 * detection of events. It should be noted, though, that this mecanism will
 * impair the execution of other processes running on the system.
 *
 * 
 * This mecanism needs to be used the following way. 
 * waiting side:
 * If a thread reach a checkpoint, it needs to pay a toll to get through. If the
 * checkpoint hasn't enough of opening to let it pass, the thread keep on spinning.
 * As soon as the checkpoint as enough opening, the thread pay its toll and move on.
 *
 * opening side:
 * If a thread want to open a checkpoint, it can increment it by either 1 or more.
 * This will allow threads that can pay the toll through.
 *
 * Ex: a worker thread might have to pay only 1 toll to pass, but a master may have to
 * pay a toll equal to the number of thread it commands to pass.
 *
 * Similarly, a worker might only allow 1 opening at a time, while a master might allow
 * an opening equal to the number of threads to pass.
 *
 * To use it, you need to:
 * - call init_checkpoints, to initialize the number of desired checkpoints.
 * - use wait_on_checkpoints, to wait on a specific check point until it has enough opening to pay its toll.
 * - use open_checkpoints, allow a certain amount of opening on the checkpoint.
 * - call to kill, when you are done with it.
 *
 * @note This mecanism leads to a higher variance in the speed of execution of programs using it.
 * So far, it has reached its peak performance when the threads were given high priority
 * and the program low (-20) niceness. Even then, we still find it to overshoot considerably
 * execution time of sequential solutions, once in a while. It is still in development.
 */
#include <pthread.h>
#include <include/spinCheckpoint.h>

/**
 * init_checkpoints(int nb_of_checkpoints)
 *  
 * @brief function that inits the checkpoints
 * it allow the memory and init all the spinlocks
 * @param 
 * @return 
 */
struct Scheckpoints *init_checkpoints(int nb_of_checkpoints)
{

	int i = 0;

	/// Allocate memory for checkpoint structures */
	struct Scheckpoints *pcheckpoints =
	    (struct Scheckpoints *)malloc(sizeof(struct Scheckpoints));

	pcheckpoints->nb_of_checkpoints = nb_of_checkpoints;

	pcheckpoints->opening = (int *)malloc(nb_of_checkpoints * sizeof(int));

	/// Check to see if malloc had an error
	if (pcheckpoints->opening == NULL) {

		return NULL;

	}

	pcheckpoints->spinlock =
	    (pthread_spinlock_t *) malloc(nb_of_checkpoints *
					  sizeof(pthread_spinlock_t));

	pthread_spin_init(&test, PTHREAD_PROCESS_PRIVATE);

	/// Init all checkpoints
	for (i = 0; i < nb_of_checkpoints; i++) {

		/// Begin init opening
		pcheckpoints->opening[i] = 0;

		/// Initialize spinlock and return NULL if error
		if (pthread_spin_init
		    (&(pcheckpoints->spinlock[i]), PTHREAD_PROCESS_PRIVATE)) {

			// If a problem occurs, inform user
			printf("could not init checkpoints");

			kill_checkpoints(pcheckpoints);

			return NULL;

		}

	}

	/// Return the initialized checkpoint
	return pcheckpoints;

}

/**
 * kill_checkpoints(struct Scheckpoints* pcheckpoints)
 * 
 * @brief Frees the memory used by the checkpoints
 * @param pcheckpoints
 */
static void kill_checkpoints(struct Scheckpoints *pcheckpoints)
{

	free((void *)pcheckpoints->opening);

	free((void *)pcheckpoints->spinlock);

	free((void *)pcheckpoints);

}

/**
 * wait_on_checkpoint(struct Scheckpoints* pcheckpoints, int checkpoints_id, int tool)
 * 
 * @brief this function blocks and only allow a thread if the checkpoint has enough
 * opening for it to pay the toll. Otherwise, it spins on it.
 * @param pcheckpoints
 * @param checkpoints_id
 * @param tool
 */
void wait_on_checkpoint(struct Scheckpoints *pcheckpoints, int checkpoints_id,
			int tool)
{

	/// Flag to know when the toll was paid
	char tmp_done_waiting = 0;

	/// Wait in while loop until the toll is paid 
	while (!tmp_done_waiting) {

		/// Acquire the spinlock (do not try to lock, if there is not enough opening available - save some performance)
		if (pcheckpoints->opening[checkpoints_id] >= tool
		    &&
		    !pthread_spin_trylock(&
					  (pcheckpoints->spinlock
					   [checkpoints_id]))) {

			/// Reconfirm that there is enough opening
			if (pcheckpoints->opening[checkpoints_id] >= tool) {

				/// Pay the toll
				pcheckpoints->opening[checkpoints_id] -= tool;

				/// Done waiting
				tmp_done_waiting = 1;

			}
			/// Free the lock
			pthread_spin_unlock(&
					    (pcheckpoints->spinlock
					     [checkpoints_id]));

		}

		else {

			/// Else spinlock is not free and yield to another thread (rather than sleep)
			sched_yield();

		}

	}

}

/**
 * open_checkpoint(struct Scheckpoints* pcheckpoints, int checkpoints_id, int opening, char lock)
 * 
 * @brief opens checkpoint for a spinlock operation
 * @param pcheckpoints
 * @param checkpoints_id
 * @param opening
 * @param lock
 * @note This function call is blocking until the opening has been given
 */
void open_checkpoint(struct Scheckpoints *pcheckpoints, int checkpoints_id,
		     int opening, char lock)
{

	/// If lock is not null or a non-zero value
	if (lock) {

		/// Get the lock
		pthread_spin_lock(&(pcheckpoints->spinlock[checkpoints_id]));

		/// Give the checkpoint an opening
		pcheckpoints->opening[checkpoints_id] += opening;

		/// Free the lock
		pthread_spin_unlock(&(pcheckpoints->spinlock[checkpoints_id]));

	}

	else {

		/// Else give the checkpoint an opening
		pcheckpoints->opening[checkpoints_id] += opening;

	}

}
