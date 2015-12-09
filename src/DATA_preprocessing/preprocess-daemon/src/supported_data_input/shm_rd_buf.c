/**
 * @file shm_rd_buf.c
 * @author Frederic Simard, Atlants Embedded (fred.simard@atlantsembedded.com)
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
#include <errno.h>

#include "data_structure.h"
#include "data_input.h"
#include "shm_rd_buf.h"

static int shmid; /*id of the shared memory array*/
static int current_page; /*keeps track of the page to be written into*/
static char* shm_buf; /*pointer to the beginning of the shared buffer*/

static int semid; /*id of semaphore set*/
struct sembuf *sops; /* pointer to operations to perform */

data_input_options_t data_input_opts;

/**
 * int shm_rd_init(void *param)
 * @brief Setups the shared memory input (memory and semaphores linkage)
 *        this function also post the write ready semaphore to begin data streaming
 * @param param, unused
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int shm_rd_init(void *param){
	
	/*copy options*/
	memcpy(&data_input_opts,param,sizeof(data_input_opts));
	
    /*
     * initialise the shared memory array
     */
	if ((shmid = shmget(data_input_opts.shm_key, SHM_BUF_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        return EXIT_FAILURE;
    }
		
    /*
     * Now we attach it to our data space.
     */
    if ((shm_buf = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        return EXIT_FAILURE;
    }
    
    /*
     * Access the semaphore array.
     */
	if ((semid = semget(data_input_opts.sem_key, NB_SEM, IPC_CREAT | 0666)) == -1) {
		perror("semget failed\n");
		return EXIT_FAILURE;
    } 
	
	/*allocate the memory for the pointer to semaphore operations*/
	sops = (struct sembuf *) malloc(sizeof(struct sembuf));
	
	current_page = 0;

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
	
	int read_ptr;
	int i,j;
	
	double* data_buf = (double*)((data_t *)param)->ptr;
	float* input_buf = (float*)shm_buf;
					
	/*wait for data to be ready for preprocessing*/
	sops->sem_num = INTERFACE_OUT_READY; /*sem that indicates that a page is free to write to*/
	sops->sem_op = -1; /*decrement semaphore*/
	sops->sem_flg = 0; /*undo if fails and blocking call*/
	
	if(semop(semid, sops, 1) != 0){
		return EXIT_FAILURE;
	}
	
	/*compute the read location*/
	read_ptr = data_input_opts.window_width*data_input_opts.nb_channels*current_page;
	
	/*read all data in the page*/
	for(i=0;i<data_input_opts.window_width;i++){
		for(j=0;j<data_input_opts.nb_channels;j++){
			
			/*read and convert*/
			data_buf[i*data_input_opts.nb_channels+j] = input_buf[read_ptr+i*data_input_opts.nb_channels+j];
			
		}
	}
			
	/*change the page*/
	current_page = (current_page+1)%NB_PAGE;
		
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
	
	/* Deallocate the semaphore array. */
	semctl(semid, 0, IPC_RMID, 0);
	
	return EXIT_SUCCESS;
}
