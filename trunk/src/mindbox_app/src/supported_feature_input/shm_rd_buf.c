/**
 * @file shm_rd_buf.c
 * @author Frederic Simard, Atlants Embedded (frederic.simard.1@outlook.com)
 * @brief This file implements the shared memory data input system.
 *        The shared memory is meant to be shared between at least two processes and
 *        takes the form of a circular buffer with several pages. When one page is done
 *        writing a semaphore is set to inform the reader (this process) 
 *        that a page is available to read. only when the reader confirms that 
 *        the data has been read is the writer allowed to write in it.
 * 
 *        When no page is available to write, the current process drops the sample. The number
 *        of pages should be kept as small as possible to prevent processing old data while
 *        dropping newest...
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "feature_structure.h"
#include "feature_input.h"
#include "shm_rd_buf.h"

static int shmid; /*id of the shared memory array*/
static char* shm_buf; /*pointer to the beginning of the shared buffer*/

static int semid; /*id of semaphore set*/
struct sembuf *sops; /* pointer to operations to perform */

/**
 * int shm_rd_init(void *param)
 * @brief Setups the shared memory input (memory and semaphores linkage)
 *        this function also post the write ready semaphore to begin data streaming
 * @param param, unused
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int shm_rd_init(void *param __attribute__ ((unused))){
	
	printf("\n**************\nInit SHM Input\n**************\n\n");
	
    /*
     * initialise the shared memory array
     */
	printf("shmget: setup shared memory space\n");
    if ((shmid = shmget(SHM_KEY, SHM_BUF_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        return EXIT_FAILURE;
    }
    else 
		printf("shmget succeeded\n\n");
		
    /*
     * Now we attach it to our data space.
     */
    printf("\nshmat: attach shared memory space\n");
    if ((shm_buf = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        return EXIT_FAILURE;
    }
    else 
		printf("shmat succeeded\n\n");
    
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

	printf("\n**************\nSuccess\n**************\n\n");
	return EXIT_SUCCESS;
}


/**
 * int shm_rd_read_from_buf(void *param)
 * @brief Reads data from the shared memory buffer and writes in the structure
 *        received as input.
 * @param param, reference to the structure where the data should be put
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int shm_rd_read_from_buf(void *param){
	
	/*open the buffer string*/
	/*preprocessing opened*/
	sops->sem_num = PREPROC_IN_READY;
	sops->sem_op = 1; 
	sops->sem_flg = SEM_UNDO | IPC_NOWAIT;
	semop(semid, sops, 1);
	
	/*application opened*/
	sops->sem_num = APP_IN_READY;
	sops->sem_op = 1; 
	sops->sem_flg = SEM_UNDO | IPC_NOWAIT;
	semop(semid, sops, 1);
	
	/*wait for features to be ready*/
	sops->sem_num = PREPROC_OUT_READY; 
	sops->sem_op = -1;
	sops->sem_flg = SEM_UNDO;	
	if(semop(semid, sops, 1) != 0){
		return EXIT_FAILURE;
	}
	
	/*copy them in the buffer*/
	printf("sample received\n");
	
	return EXIT_SUCCESS;
}

/**
 * int shm_rd_cleanup(void *param)
 * @brief Clean up shared memory and semaphore linkage
 * @param param, unused
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int shm_rd_cleanup(void *param __attribute__ ((unused))){
	
	/* Detach the shared memory segment. */
	shmdt(shm_buf);
	
	return EXIT_SUCCESS;
}
