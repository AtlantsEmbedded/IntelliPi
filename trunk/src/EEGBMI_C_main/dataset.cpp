/**
 * @file dataset.c
 * @author Frédéric Simard
 * @date January, 2015
 * @brief This file implements a wrapper that reads the samples from ASCII files.
 *		  It then process the timeseries to build the feature vectors. It offers
 *        the functionalities to shuffle the dataset into a train and test set.
 */

#include "dataset.h"

 
#if 0
typedef struct Sdataset {
	
	/*Labels and information relative to each samples*/
	int *labels; 
	t_sampledata *samplesdata;
	
	/*nb of train and test trials*/
	int nb_trials
	int nb_train_trials;
	int nb_test_trials;
	
	/*list of train and test trials*/
	int list_of_train_trials;
	int list_of_test_trials;

}t_dataset;
#endif


/**
 * init_dataset(char* path)
 * 
 * @brief Init the wrapper by reading, storing and processing the infos from the data file
 * @param path, the path to the dataset
 * @return NUll if error, non-NULL t_dataset struct if success
 */
t_dataset *init_dataset(const char* path){

	int i,j,k;

	FILE* pfile;

	/*initialize a dataset*/
	t_dataset* pdataset = (t_dataset*)malloc(sizeof(t_dataset));

	/*string buffers*/
	char filename[100]; 
	char buffer[20]; 

	int dummy;

    /**********************
     * Read the infos file
	 **********************/
	
	/*build path/filename*/
	strcpy(filename,path);
	strcat(filename,"infos");
	
	/*open the file*/
	pfile = fopen(filename,"r");
	
	/*confirm*/
	if(pfile == NULL){
		printf("Error: infos file not found");
		free(pdataset);
		pdataset = NULL;
	}
	
	/*skip the subjects initial line*/
	fgets(buffer,100,pfile);
	/*skip the number of task line*/
	fgets(buffer,100,pfile);
	
	/*get the number of train trials*/
	fscanf(pfile,"Nb of record trials: %i\n",&pdataset->nb_train_trials);
	
	/*get the number of test trials*/
	fscanf(pfile,"Nb of record trials: %i\n",&pdataset->nb_test_trials);
	
	/*sum up the number of trials*/
	pdataset->nb_trials = pdataset->nb_train_trials+pdataset->nb_test_trials;
	
	/*initialize labels memory array*/
	pdataset->labels = (int*)malloc(sizeof(int)*pdataset->nb_trials);
	
	/*read the file, drop the trial id*/
	for(i=0;i<pdataset->nb_trials;i++){
		fscanf(pfile,"%i %i\n",&dummy,&pdataset->labels[i]);
	}
	
	/*close the info file*/
	fclose(pfile);

    /**********************
     * Read the trials file
	 **********************/

	pdataset->samplesdata = (t_sampledata*)malloc(sizeof(t_sampledata)*pdataset->nb_trials);
	 
	/*Read each trial data file*/ 
	for(i=0;i<pdataset->nb_trials;i++){
	
		/*initialize memory array*/
		pdataset->samplesdata[i].timeseries = (double**)malloc(sizeof(double*)*NB_CHANNELS);
		
		for(j=0;j<NB_CHANNELS;j++){
			pdataset->samplesdata[i].timeseries[j] = (double*)malloc(sizeof(double)*TIMESERIES_LENGTH);
		}
	
		/*open the file*/
			
		/*build path/filename*/
		strcpy(filename,path);
		strcat(filename,"trial_");
		sprintf(buffer,"%i",i+1);
		strcat(filename,buffer);
		strcat(filename,".asc");
		pfile = fopen(filename,"r");
					
		/*confirm*/
		if(pfile == NULL){
			printf("Error: trial %i file not found",i);
			break;
		}
			
			
		/*read it and copy it in the array*/
		for(k=0;k<TIMESERIES_LENGTH;k++){
			fscanf(pfile,"%f %f %f %f\n",
			&pdataset->samplesdata[i].timeseries[0][k],
			&pdataset->samplesdata[i].timeseries[1][k],
			&pdataset->samplesdata[i].timeseries[2][k],
			&pdataset->samplesdata[i].timeseries[3][k]);
		}
				
	}
	
	return pdataset;
}


/**
 * kill_dataset(t_dataset *pdataset)
 * 
 * @brief Clean up the memory occupied by the dataset
 * @param pdataset, a pointer to the dataset to be cleaned
 */
void kill_dataset(t_dataset *pdataset){
	
	int i,j;
	
    /*Clean the memory*/
	for(i=0;i<pdataset->nb_trials;i++){
		
		for(j=0;j<NB_CHANNELS;j++){
			free(pdataset->samplesdata[i].fft_vectors[j]);
			free(pdataset->samplesdata[i].timeseries[j]);
		}
		
		free(pdataset->samplesdata[i].fft_vectors);
		free(pdataset->samplesdata[i].timeseries);
	}

	free(pdataset->samplesdata);
	free(pdataset);
}


/**
 * preprocess_data(t_dataset *pdataset)
 * 
 * @brief process the data to form the feature vectors
 * @param pdataset, pointer to the dataset
 * @return EXIT_SUCCESS if success, EXIT_FAILURE for failure
 */
int preprocess_data(t_dataset *pdataset){
	 
	int i,j,k;
	double average;
	 
	/*Read each trial data file*/ 
	for(i=0;i<pdataset->nb_trials;i++){
	
		/*initialize memory array*/
		pdataset->samplesdata[i].fft_vectors = (double**)malloc(sizeof(double*)*NB_CHANNELS);
		
		for(j=0;j<NB_CHANNELS;j++){
			pdataset->samplesdata[i].fft_vectors[j] = (double*)malloc(sizeof(double)*TIMESERIES_LENGTH);
		}
	}

	for(i=0;i<pdataset->nb_trials;i++){

		for(j=0;j<NB_CHANNELS;j++){
			/*remove the average of the signal before doing the fft*/
			average = 0;
				
			for(k=0;k<TIMESERIES_LENGTH;k++){
				average += pdataset->samplesdata[i].timeseries[j][k];
			}
		
			average /= pdataset->nb_trials;

			for(k=0;k<TIMESERIES_LENGTH;k++){
				pdataset->samplesdata[i].timeseries[j][k] -= average;
			}
		}
		
		for(j=0;j<NB_CHANNELS/2;j++){

			/*preprocess each channels using fft*/
			abs_fft_2signals(pdataset->samplesdata[i].timeseries[j*2], 
							 pdataset->samplesdata[i].timeseries[j*2+1], 
							 pdataset->samplesdata[i].fft_vectors[j*2], 
							 pdataset->samplesdata[i].fft_vectors[j*2+1],
							 TIMESERIES_LENGTH);
		}
	}			 

	return EXIT_SUCCESS;
}



/**
 * split_train_test_sets(t_dataset *pdataset)
 * 
 * @brief Form the train and test trial lists
 * @param pdataset, pointer to the dataset
 * @return EXIT_SUCCESS if success, EXIT_FAILURE for failure
 */
int split_train_test_sets(t_dataset *pdataset){

	/*generate the train and test trial lists*/

}


/**
 * get_train_dataset(t_dataset *pdataset, float **feature_vectors, int* labels)
 * 
 * @brief return the train dataset
 * @param pdataset, pointer to the dataset
 * @param (out)feature_vectors, feature vectors of the trials
 * @param (out)labels, labels of the trials 
 * @return the number of trials in the dataset
 */
int get_train_dataset(t_dataset *pdataset, float **feature_vectors, int* labels){

	/*form an array using only train trials*/

}

/**
 * get_test_dataset(t_dataset *pdataset, float **feature_vectors, int* labels)
 * 
 * @brief return the test dataset
 * @param pdataset, pointer to the dataset
 * @param (out)feature_vectors, feature vectors of the trials
 * @param (out)labels, labels of the trials
 * @return the number of trials in the dataset
 */
int get_test_dataset(t_dataset *pdataset, float **feature_vectors, int* labels){

	/*form an array using only test trials*/

}
