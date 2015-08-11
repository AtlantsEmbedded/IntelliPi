

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "ipc_status_comm.h"

static int semid; /*id of semaphore set*/
struct sembuf *sops; /* pointer to operations to perform */

/**
 * int ipc_comm_init()
 * @brief 
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int ipc_comm_init(){
	
	/*
     * Access the semaphore array.
     */
    printf("semget: setting up the semaphore array\n");
	if ((semid = semget(SEM_KEY, NB_SEM, IPC_CREAT | 0666)) == -1) {
		perror("semget failed\n");
		return EXIT_FAILURE;
    } 
    else 
		printf("semget succeeded\n\n");
	
	/*allocate the memory for the pointer to semaphore operations*/
	sops = (struct sembuf *) malloc(sizeof(struct sembuf));
	
	return EXIT_SUCCESS;
}

/**
 * int ipc_wait_for_harware()
 * @brief 
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int ipc_tell_harware_is_on(){
	
	/*check if the current page is available (semaphore)*/
	sops->sem_num = INTERFACE_CONNECTED; /*sem that indicates that a page is free to write to*/
	sops->sem_op = 1; /*decrement semaphore*/
	sops->sem_flg = SEM_UNDO|IPC_NOWAIT; /*undo if fails and non-blocking call*/	
	
	semop(semid, sops, 1);
	
	/*something went wrong*/
	return 0x01;
	
}

/**
 * int ipc_comm_cleanup()
 * @brief 
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int ipc_comm_cleanup(){
	return EXIT_SUCCESS;
}
