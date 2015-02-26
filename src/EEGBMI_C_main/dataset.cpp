/**
 * @file dataset.c
 * @author Frédéric Simard
 * @date January, 2015
 * @brief This file implements a wrapper that reads the samples from ASCII files.
 *		  It then process the timeseries to build the feature vectors (fft). It offers
 *        the functionalities to shuffle the dataset into a train and test set.
 */

#include "dataset.h"

void randsample(int array[], int n);


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
	//~ strcpy(filename,path);
	//~ strcat(filename,"infos");
	
	/*open the file*/
	pfile = fopen("ascii_files_3/infos","r");
	
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
	fscanf(pfile,"Nb of test trials: %i\n",&pdataset->nb_test_trials);
	
	/*sum up the number of trials*/
	pdataset->nb_trials = pdataset->nb_train_trials+pdataset->nb_test_trials;
	
	/*initialize labels memory array*/
	pdataset->labels = (int*)malloc(sizeof(int)*pdataset->nb_trials);
	
	/*read the file, drop the trial id*/
	for(i=0;i<pdataset->nb_trials;i++){
		fscanf(pfile,"%i %i\n",&dummy,&pdataset->labels[i]);
		
		/*redefine the labels to 1 and -1*/
		if(pdataset->labels[i]==2){
			pdataset->labels[i]=-1;
		}
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
		sprintf(filename,"%s",path);
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
			fscanf(pfile,"%lf %lf %lf %lf\n",
			&pdataset->samplesdata[i].timeseries[0][k],
			&pdataset->samplesdata[i].timeseries[1][k],
			&pdataset->samplesdata[i].timeseries[2][k],
			&pdataset->samplesdata[i].timeseries[3][k]);
		}
				
	}
	
	/*compute the number of train and test trials*/
	pdataset->nb_train_trials = ceil(TRAIN_TRIALS_PROP*pdataset->nb_trials);
	pdataset->nb_test_trials = pdataset->nb_trials - pdataset->nb_train_trials;
	
	/*allocated memory for indexes lists*/
	pdataset->list_of_train_trials = (int*)malloc(sizeof(int)*pdataset->nb_train_trials);
	pdataset->list_of_test_trials = (int*)malloc(sizeof(int)*pdataset->nb_test_trials);
	
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
	free(pdataset->list_of_train_trials);
	free(pdataset->list_of_test_trials);
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
			
			/*patch*/
			//pdataset->samplesdata[i].fft_vectors[j*2][0] = 0;
			//pdataset->samplesdata[i].fft_vectors[j*2+1][0] = 0;				 
							 
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

	int i, offset;
	int* trial_indexes = (int*)malloc(sizeof(int)*pdataset->nb_trials);

	/*randomly samples the train and test trials*/
	randsample(trial_indexes, pdataset->nb_trials);
	
	/*generate the train*/
	for(i=0;i<pdataset->nb_train_trials;i++){
		pdataset->list_of_train_trials[i] = trial_indexes[i];
	}
	
	/*and test trial lists*/
	offset = i;
	for(i=0;i<pdataset->nb_test_trials;i++){
		pdataset->list_of_test_trials[i] = trial_indexes[i+offset];
	}
	
	/*clean up*/
	free(trial_indexes);
}

/**
 * get_train_dataset(t_dataset *pdataset, float **feature_vectors, int* labels)
 * 
 * @brief return the ffted train dataset
 * @param pdataset, pointer to the dataset
 * @param (out)feature_vectors, feature vectors of the trials [size = nb_train_trials x FFTED_VECTOR_LENGTH]
 * @param (out)labels, labels of the trials [size = nb_train_trials]
 * @return the number of trials copied
 */
int get_train_dataset_ffted(t_dataset *pdataset, double **feature_vectors, int* labels){

	int i,j,k,offset;

	/*form an array of feature vectors using only train trials*/
	for(i=0;i<pdataset->nb_train_trials;i++){
		for(j=0;j<NB_CHANNELS;j++){
			offset = j*TIMESERIES_LENGTH/2;
			
			/*take only the first half of the fft vector*/
			for(k=0;k<TIMESERIES_LENGTH/2;k++){
				feature_vectors[i][k+offset] = pdataset->samplesdata[pdataset->list_of_train_trials[i]].fft_vectors[j][k];
			}
		}
		
		/*and one for the labels*/
		labels[i] = pdataset->labels[pdataset->list_of_train_trials[i]];
	}
	
	return pdataset->nb_train_trials;
}

/**
 * get_test_dataset(t_dataset *pdataset, float **feature_vectors, int* labels)
 * 
 * @brief return the timeseries of the test dataset 
 * @param pdataset, pointer to the dataset
 * @param (out)feature_vectors, feature vectors of the trials [size = nb_test_trials x TIMESERIES_LENGTH*NB_CHANNELS]
 * @param (out)labels, labels of the trials [size = nb_test_trials]
 * @return EXIT_SUCCESS
 */
int get_test_sample_timeseries(t_dataset *pdataset, double **timeseries_vectors, int* labels){

	int i,j, sample_id;

	/*select a test sample at random*/
	sample_id = rand()%pdataset->nb_train_trials;
	
	/*copy it in the output*/
	for(i=0;i<NB_CHANNELS;i++){
		for(j=0;j<TIMESERIES_LENGTH;j++){
			timeseries_vectors[i][j] = pdataset->samplesdata[pdataset->list_of_train_trials[sample_id]].timeseries[i][j];
		}
	}
	
	(*labels) = pdataset->labels[pdataset->list_of_train_trials[sample_id]];
	
	return EXIT_SUCCESS;
	
}

/**
 * randsample(int array[], int n)
 * 
 * @brief return an array of n elements. The array is the series of numbers [0, n-1] randomly permutated a large number of times (n*10)
 * @param n, size of the array and range of the indexes
 * @param (out), array of size n, values will be set in the fonction
 */
void randsample(int array[], int n){
	
	int i;
	int nb_permut = n*10;
	int temp, source, dest; 
	time_t t;
	
	/*seed random generator*/
	srand((unsigned) time(&t));
	
	/*init the array with continuous numbers*/
	for(i=0;i<n;i++){
		array[i] = i;
	}
	
	/*do many permutations*/
	for(i=0;i<nb_permut;i++){
		
		/*pick two samples at random*/
		source = rand() % n;
		dest = rand() % n;
		
		/*permute them*/
		temp = array[source];
		array[source] = array[dest];
		array[dest] = temp;
	}
}
