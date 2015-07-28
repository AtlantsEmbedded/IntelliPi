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

#include "data_input.h"
#include "shm_rd_buf.h"

static int shmid; /*id of the shared memory array*/
static int samples_count; /*keeps track of the number of samples that have been written in the page*/
static int current_page; /*keeps track of the page to be written into*/
static char page_opened; /*flags indicate if the page is being written into*/
static char* shm_buf; /*pointer to the beginning of the shared buffer*/

static int semid; /*id of semaphore set*/
struct sembuf *sops; /* pointer to operations to perform */

static FILE* fp; /*for debug purpose only*/

/**
 * int shm_rd_init(void *param)
 * @brief Setups the shared memory input (memory and semaphores linkage)
 *        this function also post the write ready semaphore to begin data streaming
 * @param param, unused
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int shm_rd_init(void *param __attribute__ ((unused))){
	
    /*
     * initialise the shared memory array
     */
	printf("\nshmget: setup shared memory space\n");
    if ((shmid = shmget(SHM_KEY, SHM_BUF_SIZE, 0666)) < 0) {
        perror("shmget");
        return EXIT_FAILURE;
    }
    else 
		printf("shmget succeeded\n");
		
    /*
     * Now we attach it to our data space.
     */
    printf("\nshmat: attach shared memory space\n");
    if ((shm_buf = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        return EXIT_FAILURE;
    }
    else 
		printf("nshmat succeeded\n");
    
    /*
     * Access the semaphore array.
     */
    printf("\nsemget: setting up the semaphore array\n");
	if ((semid = semget(SEM_KEY, NB_SEM, 0666)) == -1) {
		perror("semget failed\n");
		return EXIT_FAILURE;
    } 
    else 
		printf("semget succeeded\n");
	
	/*allocate the memory for the pointer to semaphore operations*/
	sops = (struct sembuf *) malloc(sizeof(struct sembuf));
	
	samples_count = 0;
	current_page = 0;
	page_opened = 0x00;

	/*indicate to the data sender that the buffer is available*/
	sops->sem_num = WRITE_READY_SEM; /*sem that indicates that a page is free to write to*/
	sops->sem_op = NB_PAGE; /*increment of the number of pages*/
	sops->sem_flg = SEM_UNDO | IPC_NOWAIT; /*undo if fails and non-blocking call*/	
	semop(semid, sops, 1);
	
	/*for debug purpose only*/
	fp = fopen("eeg_data.csv","w");

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
	
	//data_t *data = (data_t *) param;
	int read_ptr;
	int i,j;
	int integer_read;
	
	/*check if the page is not opened*/
	if(!page_opened){
		/*check if the current page is available (semaphore)*/
		sops->sem_num = DATA_SENT_SEM; /*sem that indicates that a page is free to write to*/
		sops->sem_op = -1; /*decrement semaphore*/
		sops->sem_flg = SEM_UNDO | IPC_NOWAIT; /*undo if fails and non-blocking call*/	
		
		if(semop(semid, sops, 1) == 0){
			/*yes, open the page*/
			page_opened = 0x01;
		}
	}
	
	/*if the page is opened*/
	if(page_opened){
		printf("page opened\n");
		
		/*compute the write location*/
		read_ptr = SHM_PAGE_SIZE*current_page;
		
		/*read all data in the page*/
		for(i=0;i<NB_SAMPLES_PER_PAGE;i++){
			for(j=0;j<NB_FEATURES;j++){
				
				memcpy((void*)&integer_read, (void*)&shm_buf[read_ptr+FEATURE_SIZE*j+i*SAMPLE_SIZE],sizeof(int));
				fprintf(fp,"%i;",integer_read); /*patch just to see if it works*/
			}
			fprintf(fp,"\n"); /*patch just to see if it works*/
		}
		
		/*code that should run once everything is ready*/
		//data->type = INT32;
		//data->nb_data = NB_SAMPLES_PER_PAGE;
		//memcpy(data->ptr,&shm_buf[read_ptr],sizeof(int)*NB_SAMPLES_PER_PAGE);
			
		/*close the page*/
		page_opened = 0x00;
		/*change the page*/
		current_page = (current_page+1)%NB_PAGE;
		
		/*post on the semaphore*/
		sops->sem_num = WRITE_READY_SEM;  /*sem that indicates that a page has been written to*/
		sops->sem_op = 1; /*increment semaphore of one*/
		sops->sem_flg = SEM_UNDO | IPC_NOWAIT; /*undo if fails and non-blocking call*/
		semop(semid, sops, 1);
		
	}
	else{
		/*else drop the sample*/
	}
	
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
	
	/*for debug purpose only*/
	fclose(fp);
	
	return EXIT_SUCCESS;
}
