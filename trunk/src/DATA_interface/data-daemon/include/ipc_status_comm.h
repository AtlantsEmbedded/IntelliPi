#ifndef IPC_STATUS_COMM_H
#define IPC_STATUS_COMM_H

/*this list must be shared between the following processes:
 * - DATA_interface
 * - DATA_preprocessing
 * - application software
 */
#define SEM_KEY 1234 /*key to sem array*/
#define NB_SEM 6

#define INTERFACE_OUT_READY 0 //sem posted when interface has written data
#define PREPROC_IN_READY 1 //sem posted when preprocess ready for new data
#define PREPROC_OUT_READY 2 //sem posted when preprocess has written a feature vector
#define APP_IN_READY 3 //sem posted when application ready for new feature vector
#define CONNECT_INTERFACE_REQ 4 //sem posted when application request interface connection
#define INTERFACE_CONNECTED 5 //sem posted when interface connection established
/**/

int ipc_comm_init();
int ipc_tell_harware_is_on();
int ipc_comm_cleanup();




#endif
