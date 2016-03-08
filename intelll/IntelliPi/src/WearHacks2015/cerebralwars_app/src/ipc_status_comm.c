

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
int ipc_comm_init(ipc_comm_t* ipc_comm){
	
	/*
     * Access the semaphore array.
     */
    printf("semget: setting up the semaphore array\n");
	if ((semid = semget(ipc_comm->sem_key, NB_SEM, IPC_CREAT | 0666)) == -1) {
		perror("semget failed\n");
		return EXIT_FAILURE;
    } 
    else 
		printf("semget succeeded\n\n");
	
	/*allocate the memory for the pointer to semaphore operations*/
	ipc_comm->sops = (struct sembuf *) malloc(sizeof(struct sembuf));
	
	return EXIT_SUCCESS;
}

/**
 * int ipc_wait_for_harware()
 * @brief 
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int ipc_wait_for_harware(ipc_comm_t* ipc_comm){
	
	/*check if the current page is available (semaphore)*/
	ipc_comm->sops->sem_num = INTERFACE_CONNECTED; /*sem that indicates that a page is free to write to*/
	ipc_comm->sops->sem_op = -1; /*decrement semaphore*/
	ipc_comm->sops->sem_flg = SEM_UNDO; /*undo if fails and blocking call*/	
	
	if(semop(ipc_comm->semid, ipc_comm->sops, 1) == 0){
		/*yes, move on*/
		return 0x01;
	}
	
	/*something went wrong*/
	return 0x00;
	
}

/**
 * int ipc_comm_cleanup()
 * @brief 
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int ipc_comm_cleanup(ipc_comm_t* ipc_comm){
	
	free(ipc_comm->sops);
	return EXIT_SUCCESS;
}
