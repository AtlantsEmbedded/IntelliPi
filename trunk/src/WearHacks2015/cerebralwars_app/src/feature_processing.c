


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

int train_feat_processing(feat_proc_t* feature_proc){

	int i,j;
	double* feature_array = (double*)feature_proc->feature_input->shm_buf;
	double* training_set = (double*)malloc(feature_proc->nb_train_samples*2);

	
	/*start acquisition*/
	for(i=0;i<feature_proc->nb_train_samples;i++){
		
		/*log the next sequence of samples*/
		REQUEST_FEAT_FC(feature_proc->feature_input);
		WAIT_FEAT_FC(feature_proc->feature_input);
		
		/*pick the two alpha wave samples*/
		training_set[i*2] = feature_array[5];
		training_set[i*2+1] = feature_array[5+3*55];
	
	}
	
	/*extract the training set parameters:*/
	/* -compute the mean*/
	stat_mean(training_set, feature_proc->mean, feature_proc->nb_train_samples, 2);
	
	/* -compute the standard deviation*/
	stat_std(training_set, feature_proc->mean, feature_proc->std_dev, feature_proc->nb_train_samples, 2);
	
	printf("acquiring training set, done\n");
	printf("Mean per features:\n");
	
	return EXIT_SUCCESS;
}


int get_normalized_sample(feat_proc_t* feature_proc){
	
	int i;
	double* feature_array = (double*)feature_proc->feature_input->shm_buf;
	double features[2];
		
	/*wait for a sample*/
	REQUEST_FEAT_FC(feature_proc->feature_input);
	WAIT_FEAT_FC(feature_proc->feature_input);
	
	/*get the samples*/
	features[0] = (feature_array[5]-feature_proc->mean[0])/feature_proc->std_dev[0];
	features[1] = (feature_array[5+3*55]-feature_proc->mean[1])/feature_proc->std_dev[1];
	
	/*get the normalized average*/
	feature_proc->sample = (features[0]+features[1])/2;
	
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
		std[j] /= (double)dim_i;
		std[j] = sqrt(std[j]);
	} 
}
