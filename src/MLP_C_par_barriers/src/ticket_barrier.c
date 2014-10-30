 /** 
 * 
 * @file ticket_barrier.c
 * @author Frédéric Simard
 * @date October, 2014
 * @brief Implements ticket barriers for MLP code
 */
#include <include/ticket_barrier.h>

/**
 * sys_futex(void *addr1, int op, int val1, struct timespec *timeout, void *addr2, int val3)
 * 
 * @brief Range from count_next to count_next - (1U << 31) is allowed to continue
 * @param addr1
 * @param op
 * @param val1
 * @param timeout
 * @param addr2
 * @param val3
 * @return success or failure if syscall happens
 */
static inline long sys_futex(void *addr1, int op, int val1, struct timespec *timeout, void *addr2, int val3)
{
	return syscall(SYS_futex, addr1, op, val1, timeout, addr2, val3);
}

/**
 * ticket_barrier_init(ticket_barrier_t * b, unsigned count)
 * 
 * @brief Initializes ticket barrier
 * @param b
 * @param count
 */
void ticket_barrier_init(ticket_barrier_t * b, unsigned count)
{
	b->count_in = 0;
	b->count_out = 0;
	b->count_next = -1;
	b->total = count;
}

/**
 * ticket_barrier_destroy(ticket_barrier_t * b)
 * 
 * @brief Cleanup ticket barriers and sys_futex
 * @param b
 */
void ticket_barrier_destroy(ticket_barrier_t * b)
{
	/* Alter the refcount so we trigger futex wake */
	atomic_add(&b->count_in, -1);

	/* However, threads can be leaving... so wait for them */
	while (1) {
		unsigned count_out = atomic_read(b->count_out);

		/* Make sure everything is synchronized */
		barrier();

		if (count_out == atomic_read(b->count_in) + 1)
			return;

		sys_futex(&b->count_out, FUTEX_WAIT_PRIVATE, count_out, NULL, NULL, 0);
	}
}

/**
 * ticket_barrier_wait(ticket_barrier_t * b)
 * 
 * @brief Implements the ticket barrier wait function
 * @param b
 * @return int zero if processing is allowed to continue
 */
int ticket_barrier_wait(ticket_barrier_t * b)
{
	unsigned wait = atomic_xadd(&b->count_in, 1);
	unsigned next;
	unsigned long long temp;

	int ret = 0;

	while (1) {
		next = atomic_read(b->count_next);

		/* Have the required number of threads entered? */
		if (wait - next == b->total) {
			/* Move to next bunch */
			b->count_next += b->total;

			/* Wake up waiters */
			sys_futex(&b->count_next, FUTEX_WAKE_PRIVATE, INT_MAX, NULL, NULL, 0);

			ret = PTHREAD_BARRIER_SERIAL_THREAD;

			break;
		}

		/* Are we allowed to go? */
		if (wait - next >= (1UL << 31)) {
			ret = 0;
			break;
		}

		/* Go to sleep until our bunch comes up */
		sys_futex(&b->count_next, FUTEX_WAIT_PRIVATE, next, NULL, NULL, 0);
	}

	/* Add to count_out, simultaneously reading count_in */
	temp = atomic_xadd(&b->reset, 1ULL << 32);

	/* Does count_out == count_in? */
	if ((temp >> 32) == (unsigned)temp) {
		/* Notify destroyer */
		sys_futex(&b->count_out, FUTEX_WAKE_PRIVATE, 1, NULL, NULL, 0);
	}

	return ret;
}
