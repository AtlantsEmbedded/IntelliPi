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

#define SHM_KEY 5678

#define NB_FEATURES 4
#define DATA_SIZE 4 // to be fixed, only supporting int right now
#define SAMPLE_SIZE NB_FEATURES*DATA_SIZE /*4 channels, each containing an int*/ 
#define NB_SAMPLES_PER_PAGE 110 /*nb of samples per page*/
#define NB_PAGE 2 /*2 pages, one sent and the other being filled*/
#define SHM_PAGE_SIZE NB_SAMPLES_PER_PAGE*SAMPLE_SIZE /*size of one page, containing a max of 250 samples*/
#define SHM_BUF_SIZE SHM_PAGE_SIZE*NB_PAGE /*size of the memory array*/ 
 
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




int shm_rd_init(void *param);
int shm_rd_read_from_buf(void *param);
int shm_rd_cleanup(void *param);


#endif
