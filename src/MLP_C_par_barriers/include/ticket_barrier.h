#ifndef TICKET_BARRIER_H
#define TICKET_BARRIER_H
 /** 
 * @file ticket_barrier.h
 * @author Frédéric Simard
 * @date October, 2014
 * 
 * @brief Ticket barrier header
 */

#include <stdio.h>
#include <stdlib.h>
#include <linux/futex.h>
#include <sys/time.h>
#include <pthread.h>
#include <sched.h>
#include <limits.h>

#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>

/* Compile Barrier */
#define barrier() asm volatile("": : :"memory")

/* Atomic add, returning the new value after the addition */
#define atomic_add(P, V) __sync_add_and_fetch((P), (V))

/* Atomic add, returning the value before the addition */
#define atomic_xadd(P, V) __sync_fetch_and_add((P), (V))

/* Atomic or */
#define atomic_or(P, V) __sync_or_and_fetch((P), (V))

/* Force a read of the variable */
#define atomic_read(V) (*(volatile typeof(V) *)&(V))

typedef struct ticket_barrier_t ticket_barrier_t;

struct ticket_barrier_t {
	unsigned count_next;
	unsigned total;
	union {
		struct {
			unsigned count_in;
			unsigned count_out;
		};
		unsigned long long reset;
	};
};

void ticket_barrier_init(ticket_barrier_t * b, unsigned count);
void ticket_barrier_destroy(ticket_barrier_t * b);
int ticket_barrier_wait(ticket_barrier_t * b);

#endif
