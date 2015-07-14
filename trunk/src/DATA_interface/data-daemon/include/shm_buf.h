#ifndef SHM_BUF_H
#define SHM_BUF_H
/**
 * @file shm_buf.c
 * @author Frederic Simard, Atlants Embedded (frederic.simard.1@outlook.com)
 * @brief Handles the shm_buf function pointers.
 */

#define SHM_KEY 5678

#define SAMPLE_SIZE 4*8 /*4 channels, each containing a double*/
#define NB_SAMPLES 220
#define NB_PAGE 2 /*2 pages, one sent and the other being filled*/
#define SHM_PAGE_SIZE NB_SAMPLES*SAMPLE_SIZE /*size of one page, containing a max of 250 samples*/
#define SHM_BUF_SIZE SHM_PAGE_SIZE*NB_PAGE /*size of the memory array*/ 
 
#define SEM_KEY 1234 
#define NB_SEM 2 /*2 semaphore, one posted for data avail, one posted for data read*/

int shm_init(void *param);
int shm_write_in_buf(void *param);
int shm_cleanup(void *param);


#endif
