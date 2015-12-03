


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "feature_processing.h"
#include "feature_input.h"

void stat_mean(double *a, double *mean, int dim_i, int dim_j);
void stat_std(double *a, double *mean, double *std, int dim_i, int dim_j);

int init_feat_processing(feat_proc_t* feature_proc){
	
	return EXIT_SUCCESS;
}

void train_feat_processing(feat_proc_t* feature_proc){

	int i,j,k;
	double* feature_array = (double*)feature_proc->feature_input->shm_buf;
	double* training_set = (double*)malloc(feature_proc->nb_train_samples*2*sizeof(double));
	
	double max_left;
	double max_right;
	
	
	/*drop first 3 packets to prevent errors*/
	for(i=0;i<3;i++){
		REQUEST_FEAT_FC(feature_proc->feature_input);
		WAIT_FEAT_FC(feature_proc->feature_input);
	}
	
	/*start acquisition*/
	for(i=0;i<feature_proc->nb_train_samples;i++){
		
		/*log the next sequence of samples*/
		REQUEST_FEAT_FC(feature_proc->feature_input);
		WAIT_FEAT_FC(feature_proc->feature_input);
		
		max_left = feature_array[4];
		max_right = feature_array[4+3*55];
		
		for(k=5;k<7;k++){
			if(feature_array[k]>max_left){
				max_left = feature_array[k];
			}
			
			if(feature_array[k+3*55]>max_right){
				max_right = feature_array[k+3*55];
			}
		}
		
		
		/*pick the two alpha wave samples*/
		training_set[i*2] = max_left;
		training_set[i*2+1] = max_right;
		
		if(i%5==0){
			printf("training progress: %.1f\n",(float)i/(float)feature_proc->nb_train_samples*100);
			fflush(stdout);
		}	
	}
	
	printf("left\tright\n");
	for(i=0;i<feature_proc->nb_train_samples;i++){
		printf("[%i]:\t%f\t%f\n",i,training_set[i*2],training_set[i*2+1]);
	}
	
	/*extract the training set parameters:*/
	/* -compute the mean*/
	printf("Computing the mean\n");
	stat_mean(training_set, feature_proc->mean, feature_proc->nb_train_samples, 2);
	printf("mean:\t%f\t%f\n",i,feature_proc->mean[0],feature_proc->mean[1]);
	
	/* -compute the standard deviation*/
	printf("Computing the std\n");
	stat_std(training_set, feature_proc->mean, feature_proc->std_dev, feature_proc->nb_train_samples, 2);
	printf("std:\t%f\t%f\n",i,feature_proc->std_dev[0],feature_proc->std_dev[1]);
	fflush(stdout);	
	
	printf("Done\n");
	free(training_set);
	printf("and Done\n");
	
}


int get_normalized_sample(feat_proc_t* feature_proc){
	
	int i,k;
	double* feature_array = (double*)feature_proc->feature_input->shm_buf;
	double features[2];
	char eye_blink_detected = 0x01;
	double max_left;
	double max_right;
		
	while(eye_blink_detected){	
		/*wait for a sample*/
		REQUEST_FEAT_FC(feature_proc->feature_input);
		WAIT_FEAT_FC(feature_proc->feature_input);
		
		max_left = feature_array[4];
		max_right = feature_array[4+3*55];
		
		for(k=5;k<7;k++){
			if(feature_array[k]>max_left){
				max_left = feature_array[k];
			}
			
			if(feature_array[k+3*55]>max_right){
				max_right = feature_array[k+3*55];
			}
		}
		
		/*get the samples*/
		features[0] = (max_left-feature_proc->mean[0])/feature_proc->std_dev[0];
		features[1] = (max_right-feature_proc->mean[1])/feature_proc->std_dev[1];
		
		/*get the normalized average*/
		feature_proc->sample = (features[0]+features[1])/2;
		
		/*hardcoded blink detection*/
		if(feature_proc->sample < 3.5){
			eye_blink_detected = 0x00;
		}else{
			printf("Eye blink detected!\n");
			fflush(stdout);
		}
	}
	
	return EXIT_SUCCESS;
}


int clean_up_feat_processing(feat_proc_t* feature_proc){
	
	return EXIT_SUCCESS;
}


/**
 * void stat_mean(double *a, double *mean, int dim_i, int dim_j)
 * @brief Function to calculate the mean matrix
 * @param double *a , the matrix containing the original matrix
 * @param double *mean , the matrix contraining the mean of each column of *a
 * @param int dim_i, the number of rows in the matrix
 * @param int dim_j, the number of colomns in the matrix
 */
void stat_mean(double *a, double *mean, int dim_i, int dim_j){
	int i,j,n;  
	n=0;
   
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


void stat_std(double *a, double *mean, double *std, int dim_i, int dim_j){
	int i,j,n;  
	n=0;
   
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
