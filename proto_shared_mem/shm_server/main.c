/**
 * @file shm_server/main.c
 * @author Frédéric Simard, Atlans embedded (frederic.simard.1@outlook.com)
 * @date July, 2015
 * @brief shm-server - server program to demonstrate the combined shared memory and interprocess semaphores usage
 *        This program implements the server. It creates the shared elements (memory and semaphores).
 *        It writes a sequence of characters in the shared memory and post its semaphore. It then waits
 *        on the client semaphore, before reading the shared memory, where a special character should have
 *        been placed.
 * 
 * I got the shared memory part here:
 * http://www.cs.cf.ac.uk/Dave/C/node27.html
 * 
 * and the semaphore usage here:
 * http://www.cs.cf.ac.uk/Dave/C/node26.html
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SHMSZ     27

main()
{
	
	/***************************/
	/* INITIALIZATION          */
	/***************************/
	
	/*variables for the shared memory definition*/
    char c;
    int shmid;
    key_t shm_key = 5678;
    char *shm, *s;
    
	/*variables for the shared semaphore definition*/
    int semid; /* semid of semaphore set */
	key_t sem_key = 1234; /* key to pass to semget() */
	int nsems = 2; /* nsems to pass to semget() */
	struct sembuf *sops = (struct sembuf *) malloc(sizeof(struct sembuf)); /* ptr to operations to perform */
	//union semun arg;
	//struct semid_ds semid_ds;
	//arg.buf = &semid_ds;

    /*
     * Create the shared memory segment.
     */
    printf("\nshmget: setup shared memory space\n");
    if ((shmid = shmget(shm_key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    else 
		printf("shmget succeeded\n");

    /*
     * Now we attach it to our data space.
     */
    printf("\nshmat: attach shared memory space\n");
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    else 
		printf("nshmat succeeded\n");
    
    /*
     * Access the semaphore array.
     */
    printf("\nsemget: setting up the semaphore array\n");
	if ((semid = semget(sem_key, nsems, IPC_CREAT | 0666)) == -1) {
		perror("semget failed\n");
		exit(1);
    } 
    else 
		printf("semget succeeded\n");

	/************************************/
	/* INFORMATION PICK UP AND FEEDBACK */
	/************************************/
	
    /*
     * Now put some things into the memory for the
     * other process to read.
     */
    s = shm;

    for (c = 'a'; c <= 'z'; c++)
        *s++ = c;
    *s = NULL;
    
    /*
     * Post a semaphore to indicate that content is available
     * */
	sops->sem_num = 0; /*sem 0 is data sent*/
	sops->sem_op = 1; /*increment semaphore*/
	sops->sem_flg = SEM_UNDO; /*undo if fails*/
    semop(semid, sops, 1);
     
    /*
     * Wait on semaphore for other process to put something in shared mem
     * */
	sops->sem_num = 1; /*sem 1 is feedback*/
	sops->sem_op = -1; /*decrement semaphore*/
	sops->sem_flg = SEM_UNDO; /*undo if fails*/
    semop(semid, sops, 1);

	/*check content is what is expected*/
	if(*shm != '*')
		printf("Server, incorrect content read, test failed\n");
	else
		printf("Server, content read is correct, test succeded\n");


	/******************/
	/* CLEAN UP       */
	/******************/
	/* Detach the shared memory segment. */
	shmdt(shm);
	/* Deallocate the shared memory segment. */
	shmctl(shmid, IPC_RMID, 0);
	/* Deallocate the semaphore array. */
	semctl(semid, 0, IPC_RMID, 0);

    exit(0);
}

