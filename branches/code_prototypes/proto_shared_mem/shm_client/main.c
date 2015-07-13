/**
 * @file shm_client/main.c
 * @author Frédéric Simard, Atlans embedded (frederic.simard.1@outlook.com)
 * @date July, 2015
 * @brief shm-client - client program to demonstrate the combined shared memory and interprocess semaphores usage
 *        This program implements the client. It waits on a semaphore posted by the server. Once it
 *        got it, it reads the shared memory, write a special caracter and post its own semaphore to confirm. 
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
	
	/*variables for the shared memory definition and reference*/
    int shmid;
    key_t shm_key = 5678;
    char *shm, *s;
    
	/*variables for the shared semaphore definition*/
    int semid; /* semid of semaphore set */
	key_t sem_key = 1234; /* key to pass to semget() */
	int nsems = 2; /* nsems to pass to semget() */
	struct sembuf *sops = (struct sembuf *) malloc(sizeof(struct sembuf)); /* ptr to operations to perform */
	
    /*
     * Locate the segment.
     */
    if ((shmid = shmget(shm_key, SHMSZ, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    /*
     * Access the semaphore array.
     */
    printf("\nsemget: setting up the semaphore array\n");
	if ((semid = semget(sem_key, nsems, 0666)) == -1) {
		perror("semget failed\n");
		exit(1);
    } 
    else 
		printf("semget succeeded\n");


	/************************************/
	/* INFORMATION PICK UP AND FEEDBACK */
	/************************************/

    /*
     * Wait on semaphore for other process to put something in shared mem
     * */
	sops->sem_num = 0; /*sem 0 is data sent*/
	sops->sem_op = -1; /*decrement semaphore*/
	sops->sem_flg = SEM_UNDO; /*undo if fails*/
    semop(semid, sops, 1);
    
    /*
     * Now read what the server put in the memory.
     */
    for (s = shm; *s != NULL; s++)
        putchar(*s);
    putchar('\n');

    /*
     * Finally, change the first character of the 
     * segment to '*', indicating we have read 
     * the segment.
     */
    *shm = '*';

    /*
     * Post a semaphore to indicate that the special character is available
     * */
	sops->sem_num = 1; /*sem 0 is data sent*/
	sops->sem_op = 1; /*increment semaphore*/
	sops->sem_flg = SEM_UNDO; /*undo if fails*/
    semop(semid, sops, 1);
    
	/******************/
	/* CLEAN UP       */
	/******************/
    
	/* Detach the shared memory segment. */
	shmdt(shm);

    exit(0);
}
