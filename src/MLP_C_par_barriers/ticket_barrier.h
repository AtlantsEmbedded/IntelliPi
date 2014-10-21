#ifndef TICKET_BARRIER_H
#define TICKET_BARRIER_H

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

typedef struct ticket_barrier_t ticket_barrier_t;

struct ticket_barrier_t
{
	unsigned count_next;
	unsigned total;
	union
	{
		struct
		{
			unsigned count_in;
			unsigned count_out;
		};
		unsigned long long reset;
	};
};


void ticket_barrier_init(ticket_barrier_t *b, unsigned count);
void ticket_barrier_destroy(ticket_barrier_t *b);
int ticket_barrier_wait(ticket_barrier_t *b);


#endif
