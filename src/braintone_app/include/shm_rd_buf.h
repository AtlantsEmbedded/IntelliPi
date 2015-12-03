#ifndef SHM_RD_BUF_H
#define SHM_RD_BUF_H
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
#define NB_SEM 6

#define INTERFACE_OUT_READY 0 //sem posted when interface has written data
#define PREPROC_IN_READY 1 //sem posted when preprocess ready for new data
#define PREPROC_OUT_READY 2 //sem posted when preprocess has written a feature vector
#define APP_IN_READY 3 //sem posted when application ready for new feature vector
#define CONNECT_INTERFACE_REQ 4 //sem posted when application request interface connection
#define INTERFACE_CONNECTED 5 //sem posted when interface connection established
/**/

int shm_rd_init(void *param);
int shm_rd_request(void *param);
int shm_rd_wait_for_request_completed(void *param);
int shm_rd_cleanup(void *param);


#endif
