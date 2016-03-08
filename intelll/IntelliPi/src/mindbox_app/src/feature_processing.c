/**
 * @file feature_processing.c
 * @author Frederic Simard (fred.simard@atlantsembedded.com) | Atlants Embedded
 * @brief Implements the feature processing capabilities
 * 
 * 		  It defines a method to train the feature processor and a method
 *        to get new processed samples.
 * 
 * 		  In the current implementation, the feature processing consists in
 * 		  z-scoring samples. Training defines the transform parameters.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "feature_processing.h"
#include "feature_input.h"

#define NB_PACKETS_TO_DROP 3
#define RIGHT_CHANNEL_OFFSET 3*55

#define FEAT_RANGE_START 4
#define FEAT_RANGE_STOP 7

#define BLINK_THRESHOLD 3.5

void proc_get_max_values(double* feature_vector, double* max_left, double* max_right);

/*stats methods, should be in a shared object (to be fixed)*/
void stat_mean(double *a, double *mean, int dim_i, int dim_j);
void stat_std(double *a, double *mean, double *std, int dim_i, int dim_j);

/**
 * int init_feat_processing(feat_proc_t* feature_proc)
 * @brief initialize the feature processing
 * @param feature_proc, pointer to a feature processing struct
 * @return EXIT_SUCCESS
 */ 
int init_feat_processing(feat_proc_t* feature_proc){
	
	/*nothing to do for this feature processor*/
	return EXIT_SUCCESS;
}

/**
 * int train_feat_processing(feat_proc_t* feature_proc)
 * @brief trainer for the feature processor, in this case we find average and standard dev of features
 * @param feature_proc, pointer to a feature processing struct
 * @return EXIT_SUCCESS
 */ 
int train_feat_processing(feat_proc_t* feature_proc){

	int i,j,k;
	
	/*allocate memory to log training set*/
	double* feature_array = (double*)feature_proc->feature_input->shm_buf;
	double* training_set = (double*)malloc(feature_proc->nb_train_samples*2*sizeof(double));
	
	/*feature range max variable*/
	double max_left;
	double max_right;
	
	/*check if memory was allocated correctly*/
	if(training_set==NULL){
		return EXIT_FAILURE;
	}
	
	/*drop first few packets to prevent errors*/
	for(i=0;i<NB_PACKETS_TO_DROP;i++){
		REQUEST_FEAT_FC(feature_proc->feature_input);
		WAIT_FEAT_FC(feature_proc->feature_input);
	}
	
	/*start acquisition*/
	for(i=0;i<feature_proc->nb_train_samples;i++){
		
		/*log the next sequence of samples*/
		REQUEST_FEAT_FC(feature_proc->feature_input);
		WAIT_FEAT_FC(feature_proc->feature_input);
		
		/*find the peak values*/
		proc_get_max_values(feature_array, &max_left, &max_right);		
				
		/*pick the two peak values*/
		training_set[i*2] = max_left;
		training_set[i*2+1] = max_right;
		
		/*inform on training progress*/
		if(i%5==0){
			printf("training progress: %.1f\n",(float)i/(float)feature_proc->nb_train_samples*100);
			fflush(stdout);
		}	
	}
	
	/*show the training set*/
	printf("left\tright\n");
	for(i=0;i<feature_proc->nb_train_samples;i++){
		printf("[%i]:\t%f\t%f\n",i,training_set[i*2],training_set[i*2+1]);
	}
	
	/*extract the training set parameters:*/
	/* -compute the mean*/
	printf("Computing the mean\n");
	stat_mean(training_set, feature_proc->mean, feature_proc->nb_train_samples, 2);
	printf("mean:\t%f\t%f\n",feature_proc->mean[0],feature_proc->mean[1]);
	
	/* -compute the standard deviation*/
	printf("Computing the std\n");
	stat_std(training_set, feature_proc->mean, feature_proc->std_dev, feature_proc->nb_train_samples, 2);
	printf("std:\t%f\t%f\n",feature_proc->std_dev[0],feature_proc->std_dev[1]);
	fflush(stdout);	
	
	/*free training set*/
	free(training_set);
	printf("Done\n");
	
	return EXIT_SUCCESS;
}

/**
 * int get_normalized_sample(feat_proc_t* feature_proc)
 * @brief 
 * @param feature_proc, pointer to a feature processing struct
 * @return EXIT_SUCCESS
 */ 
int get_normalized_sample(feat_proc_t* feature_proc){
	
	int i,k;
	/*re-cast for simplicity*/
	double* feature_array = (double*)feature_proc->feature_input->shm_buf;
	/*will contain features*/
	double features[2];
	/*flag for eye_blink*/
	char eye_blink_detected = 0x01;
	/*local processing*/
	double max_left;
	double max_right;

	/*filter out blink samples*/
	while(eye_blink_detected){	
		
		/*request a sample*/
		REQUEST_FEAT_FC(feature_proc->feature_input);
		
		/*wait for it*/
		WAIT_FEAT_FC(feature_proc->feature_input);

		/*find the peak values*/
		proc_get_max_values(feature_array, &max_left, &max_right);		
		
		/*normalize*/
		features[0] = (max_left-feature_proc->mean[0])/feature_proc->std_dev[0];
		features[1] = (max_right-feature_proc->mean[1])/feature_proc->std_dev[1];
		
		/*get the normalized average*/
		feature_proc->sample = (features[0]+features[1])/2;
		
		/*hardcoded blink detection*/
		if(feature_proc->sample < BLINK_THRESHOLD){
			eye_blink_detected = 0x00;
		}else{
			printf("Eye blink detected!\n");
			fflush(stdout);
		}
	}
	
	return EXIT_SUCCESS;
}

/**
 * void proc_get_max_values(double* feature_array, double* max_left, double* max_right)
 * @brief This function will find the max values in the range of features defined.
 *        It is hard coded right now.
 * @param feature_array(in), array of features - length in defines
 * @param max_left(out), max value for the left channel
 * @param max_right(out), max value for the right channel
 */
void proc_get_max_values(double* feature_array, double* max_left, double* max_right){

	int k = FEAT_RANGE_START;

	/*find the max among features around 10Hz*/
	*max_left = feature_array[k];
	*max_right = feature_array[RIGHT_CHANNEL_OFFSET+k];
	k++;
	
	/*loop through adjacent features to find the peak*/
	for(;k<FEAT_RANGE_STOP;k++){
		
		/*check if found a peak in left channel*/
		if(feature_array[k]> *max_left){
			*max_left = feature_array[k];
		}
		
		/*check if found a peak in right channel*/
		if(feature_array[RIGHT_CHANNEL_OFFSET+k]> *max_right){
			*max_right = feature_array[RIGHT_CHANNEL_OFFSET+k];
		}
	}
}

/**
 * void proc_get_max_values(double* feature_array, double* max_left, double* max_right)
 * @brief clean up the feature processing core
 * @param feature_proc, pointer to a feature processing struct
 */
int clean_up_feat_processing(feat_proc_t* feature_proc){
	
	/*nothing to do*/
	return EXIT_SUCCESS;
}


/**
 * void stat_mean(double *a, double *mean, int dim_i, int dim_j)
 * @brief Function to compute the mean vector
 * @param a(in), the matrix
 * @param mean(out), the vector with the mean of each column of a
 * @param dim_i, the number of rows in the matrix
 * @param dim_j, the number of colomns in the matrix
 */
void stat_mean(double *a, double *mean, int dim_i, int dim_j){
	
	int i,j,n;  
	n=0;
	
	/*reset to 0*/
	for (j=0;j<dim_j;j++){
            mean[j] = 0;
	}
   
	/*Sum of each column*/
	for(i=0;i<dim_i;i++){
		n = i*dim_j;
		for (j=0;j<dim_j;j++){
            mean[j] += a[j+n];
		}
	}

	/*Divide to get average*/
	for (j=0;j<dim_j;j++){
		mean[j] /= dim_i;
	} 
}

/**
 * void stat_std(double *a, double *mean, double *std, int dim_i, int dim_j)
 * @brief Function to compute the standard deviation vector
 * @param a(in), the matrix containing the original matrix
 * @param mean(in), the vector with the mean of each column of a
 * @param std(out), the vector with the standard deviation of each column of a
 * @param dim_i, the number of rows in the matrix
 * @param dim_j, the number of colomns in the matrix
 */
void stat_std(double *a, double *mean, double *std, int dim_i, int dim_j){
	
	int i,j,n;  
	n=0;
   
	/*reset to 0*/
	for (j=0;j<dim_j;j++){
        std[j] = 0;
	}
   
	/*Sum of each column*/
	for(i=0;i<dim_i;i++){
		n = i*dim_j;
		for (j=0;j<dim_j;j++){
             std[j] += (a[j+n]-mean[j])*(a[j+n]-mean[j]);
		}
	}

	/*Divide to get average*/
	for (j=0;j<dim_j;j++){
		std[j] /= (double)(dim_i-1);
		std[j] = sqrt(std[j]);
	} 
}
