/**
 * @file shm_buf.c
 * @author Frederic Simard, Atlants Embedded (frederic.simard.1@outlook.com)
 * @brief Handles the shm_buf function pointers.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "data_output.h"
#include "shm_buf.h"

static int shmid; /*id of the shared memory array*/
static int samples_count[NB_PAGE]; /*keeps track of the number of samples that have been written in the page*/
static char* page_start[NB_PAGE]; /*pointers on page starts*/
static int current_page;

static int semid; /*id of semaphore set*/
struct sembuf *sops; /* pointer to operations to perform */

int shm_init(void *param __attribute__ ((unused))){
	
	printf("\nshmget: setup shared memory space\n");
    if ((shmid = shmget(SHM_KEY, SHM_BUF_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        return EXIT_FAILURE;
    }
    else 
		printf("shmget succeeded\n");
		
    /*
     * Now we attach it to our data space.
     */
    printf("\nshmat: attach shared memory space\n");
    if ((page_start[0] = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        return EXIT_FAILURE;
    }
    else 
		printf("nshmat succeeded\n");
    
    /*
     * Access the semaphore array.
     */
    printf("\nsemget: setting up the semaphore array\n");
	if ((semid = semget(SHM_BUF_SIZE, NB_SEM, IPC_CREAT | 0666)) == -1) {
		perror("semget failed\n");
		return EXIT_FAILURE;
    } 
    else 
		printf("semget succeeded\n");
	
	/*allocate the memory for the pointer to semaphore operations*/
	sops = (struct sembuf *) malloc(sizeof(struct sembuf));

	return EXIT_SUCCESS;
}

int shm_write_in_buf(void *param){
	
	
	
	
}

int shm_cleanup(void *param __attribute__ ((unused))){
	
	/******************/
	/* CLEAN UP       */
	/******************/
	/* Detach the shared memory segment. */
	shmdt(page_start[0]);
	/* Deallocate the shared memory segment. */
	shmctl(shmid, IPC_RMID, 0);
	/* Deallocate the semaphore array. */
	semctl(semid, 0, IPC_RMID, 0);
	
}
