/**
 * @file shm_wr_buf.c
 * @author Frederic Simard, Atlants Embedded (frederic.simard.1@outlook.com)
 * @brief (...)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "data_structure.h"
#include "feature_output.h"
#include "shm_wrt_buf.h"

static int shmid; /*id of the shared memory array*/
static int feature_vect_count; /*keeps track of the number of samples that have been written in the page*/
static char* shm_buf; /*pointer to the beginning of the shared buffer*/

static int semid; /*id of semaphore set*/
struct sembuf *sops; /* pointer to operations to perform */

/* arg for semctl system calls. */
union semun {
		int val;                /* value for SETVAL */
		struct semid_ds *buf;   /* buffer for IPC_STAT & IPC_SET */
		ushort *array;          /* array for GETALL & SETALL */
		struct seminfo *__buf;  /* buffer for IPC_INFO */
		void *__pad;
};

/**
 * int shm_wrt_init(void *param)
 * @brief Setups the shared memory output (memory allocation, linking and semaphores)
 * @param param, unused
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int shm_wrt_init(void *param __attribute__ ((unused))){
	
	union semun semopts;    
		        
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
	if ((semid = semget(SEM_KEY, NB_SEM, IPC_CREAT | 0666)) == -1) {
		perror("semget failed\n");
		return EXIT_FAILURE;
    } 
    else 
		printf("semget succeeded\n");

    //semopts.val = 0;
    //semctl( semid, 0, SETVAL, semopts);
    //semctl( semid, 1, SETVAL, semopts);
	
	/*allocate the memory for the pointer to semaphore operations*/
	sops = (struct sembuf *) malloc(sizeof(struct sembuf));
	
	/*give initial values to static variables*/
	feature_vect_count = 0;
	
	return EXIT_SUCCESS;
}

/**
 * int shm_wrt_write_to_buf(void *param)
 * @brief 
 * @param 
 * @return 
 */
int shm_wrt_write_to_buf(void *param){
	
	data_t *data = (data_t *) param;
	int write_ptr;

	/*check if free slot available*/
	sops->sem_num = APP_IN_READY; /*sem: app ready for data*/
	sops->sem_op = -1; /*decrement semaphore*/
	sops->sem_flg = SEM_UNDO | IPC_NOWAIT; /*undo if fails and non-blocking call*/	
	
	if(semop(semid, sops, 1) == 0){
		
		/*yes, write a feature vector:*/
		
		/*-compute the write location*/
		write_ptr = SAMPLE_SIZE*feature_vect_count;
		
		/*-write data*/
		memcpy((void*)&(shm_buf[write_ptr]),(void*)data->ptr, data->nb_data*sizeof(double));
		
		/*-increment count (circular buffer)*/
		//feature_vect_count = (feature_vect_count+data->nb_data)%BUFFER_DEPTH;
		feature_vect_count = 0;
		
		/*-post the semaphore*/
		sops->sem_num = PREPROC_OUT_READY;
		sops->sem_op = 1;
		sops->sem_flg = SEM_UNDO | IPC_NOWAIT;
		semop(semid, sops, 1);
	}
	else{
		/*no, skip do nothing*/
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
