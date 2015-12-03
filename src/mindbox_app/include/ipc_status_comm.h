#ifndef IPC_STATUS_COMM_H
#define IPC_STATUS_COMM_H

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

typedef struct ipc_comm_s{
	/*to be set before initialization*/
	int sem_key;
	/*will be set during initialization*/
	int semid;
	struct sembuf *sops;
}ipc_comm_t;

int ipc_comm_init(ipc_comm_t* ipc_comm);
int ipc_wait_for_harware(ipc_comm_t* ipc_comm);
int ipc_comm_cleanup(ipc_comm_t* ipc_comm);




#endif
