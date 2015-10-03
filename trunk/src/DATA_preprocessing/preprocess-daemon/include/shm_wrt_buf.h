#ifndef SHM_WRT_BUF_H
#define SHM_WRT_BUF_H
/**
 * @file shm_wrt_buf.c
 * @author Frederic Simard, Atlants Embedded (frederic.simard.1@outlook.com)
 * @brief This file implements the shared memory feature output system.
 *        The shared memory is meant to be shared between at least two processes and
 *        takes the form of a circular buffer with each slot containing
 *        one feature vector. 
 * 
 *        (...)
 */

//#define SHM_KEY 7804

#define NB_FEATURES 220
#define FEATURE_SIZE 8 
#define SAMPLE_SIZE NB_FEATURES*FEATURE_SIZE 
#define BUFFER_DEPTH 2 /*nb of samples in the buffer*/
#define SHM_BUF_SIZE BUFFER_DEPTH*SAMPLE_SIZE

/*this list must be shared between the following processes:
 * - DATA_interface
 * - DATA_preprocessing
 * - application software
 */
#define SEM_KEY 1234 /*key to sem array*/
#define NB_SEM 6/*2 semaphore, one posted for data avail, one posted for data read*/

#define INTERFACE_OUT_READY 0 //sem posted when interface has written data
#define PREPROC_IN_READY 1 //sem posted when preprocess ready for new data
#define PREPROC_OUT_READY 2 //sem posted when preprocess has written a feature vector
#define APP_IN_READY 3 //sem posted when application ready for new feature vector
#define CONNECT_INTERFACE_REQ 4 //sem posted when application request interface connection
#define INTERFACE_CONNECTED 5 //sem posted when interface connection established
/**/

int shm_wrt_init(void *param);
int shm_wrt_write_to_buf(void *param);
int shm_wrt_cleanup(void *param);


#endif
