/**
 * @file shm_buf_writer.c
 * @author Frederic Simard, Atlants Embedded (frederic.simard.1@outlook.com)
 * @brief This file implements the shared memory data output system.
 *        The shared memory is meant to be shared between at least two processes and
 *        takes the form of a circular buffer with several pages. When one page is done
 *        writing a semaphore is set to inform the reader that a page is available to read.
 *        only when the reader confirms that the data has been read is the current process
 *        allowed to write in it.
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

#include "data_output.h"
#include "shm_wrt_buf.h"

static int shmid; /*id of the shared memory array*/
static int samples_count; /*keeps track of the number of samples that have been written in the page*/
static int current_page; /*keeps track of the page to be written into*/
static char page_opened; /*flags indicate if the page is being written into*/
static char* shm_buf; /*pointer to the beginning of the shared buffer*/

static int semid; /*id of semaphore set*/
struct sembuf *sops; /* pointer to operations to perform */

/**
 * int shm_wrt_init(void *param)
 * @brief Setups the shared memory output (memory allocation, linking and semaphores)
 * @param param, unused
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int shm_wrt_init(void *param __attribute__ ((unused))){
	
    /*initialise the shared memory array*/
	printf("\nshmget: setup shared memory space\n");
    if ((shmid = shmget(SHM_KEY, SHM_BUF_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        return EXIT_FAILURE;
    }
    else 
		printf("shmget succeeded\n");
		
    /*Now we attach it to our data space*/
    printf("\nshmat: attach shared memory space\n");
    if ((shm_buf = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        return EXIT_FAILURE;
    }
    else 
		printf("nshmat succeeded\n");
    
    /*Access the semaphore array*/
    printf("\nsemget: setting up the semaphore array\n");
	if ((semid = semget(SHM_BUF_SIZE, NB_SEM, IPC_CREAT | 0666)) == -1) {
		perror("semget failed\n");
		return EXIT_FAILURE;
    } 
    else 
		printf("semget succeeded\n");
	
	/*allocate the memory for the pointer to semaphore operations*/
	sops = (struct sembuf *) malloc(sizeof(struct sembuf));
	
	/*give initial values to static variables*/
	samples_count = 0;
	current_page = 0;
	page_opened = 0x00;

	return EXIT_SUCCESS;
}

/**
 * int shm_wrt_write_in_buf(void *param)
 * @brief Writes the data received to the shared memory. This function makes sure that:
 *        - the page is available
 *        - the data is written at the right place in the page
 *        - the page is changed once its filled
 *        - informs the reader that a page has been filled
 * @param param, refers to a data_t pointer, which contains the data to be written
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int shm_wrt_write_in_buf(void *param){
	
	data_t *data = (data_t *) param;
	int write_ptr;
	
	/*check if the page is not opened*/
	if(!page_opened){
		/*if not opened*/
		/*check if the current page is available (semaphore)*/
		sops->sem_num = WRITE_READY_SEM; /*sem that indicates that a page is free to write to*/
		sops->sem_op = -1; /*decrement semaphore*/
		sops->sem_flg = SEM_UNDO | IPC_NOWAIT; /*undo if fails and non-blocking call*/	
		if(semop(semid, sops, 1) == 0){
			/*yes, open the page*/
			page_opened = 0x01;
		}
	}
	
	/*if the page is opened*/
	if(page_opened){
		/*compute the write location*/
		write_ptr = SHM_PAGE_SIZE*current_page+SAMPLE_SIZE*samples_count;
		
		/*check which type of data is coming in*/
		switch(data->type){
			/*32 bits unsigned integer*/
			case INT32:
				/*write data*/
				memcpy((void*)&(shm_buf[write_ptr]),(void*)data->ptr, data->nb_data*sizeof(int));
			break;
			
			default:
				printf("Unsupported data->type in shm_buf");
			break;
		}
		samples_count++;
		
		/*check if the page is full*/
		if(samples_count>NB_SAMPLES_PER_PAGE){
			
			/*close the page*/
			page_opened = 0x00;
			/*change the page*/
			current_page = (current_page+1)%NB_PAGE;
			/*reset nb of samples read*/
			samples_count = 0; 
			
			/*post the semaphore*/
			sops->sem_num = DATA_SENT_SEM;  /*sem that indicates that a page has been written to*/
			sops->sem_op = 1; /*increment semaphore of one*/
			sops->sem_flg = SEM_UNDO | IPC_NOWAIT; /*undo if fails and non-blocking call*/
			semop(semid, sops, 1);
		}
	}
	else{
		/*else drop the sample (do nothing)*/
	}
	
	return EXIT_SUCCESS;
}

/**
 * int shm_cleanup((void *param)
 * @brief Clean up the shared memory: detach, deallocate mem and sem
 * @param param, unused
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int shm_wrt_cleanup(void *param __attribute__ ((unused))){
	
	/* Detach the shared memory segment. */
	shmdt(shm_buf);
	/* Deallocate the shared memory segment. */
	shmctl(shmid, IPC_RMID, 0);
	/* Deallocate the semaphore array. */
	semctl(semid, 0, IPC_RMID, 0);
	
	return EXIT_SUCCESS;
}
