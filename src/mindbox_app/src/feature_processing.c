


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feature_processing.h"
#include "feature_input.h"

feat_proc_options_t v_feat_proc_options;
double* v_training_set;
double* v_mean;
double* v_std_dev;

feature_t v_feature_vect;
double* v_feature_array;

void stat_mean(double *a, double *mean, int dim_i, int dim_j);
void stat_std(double *a, double *mean, double *std, int dim_i, int dim_j);

int init_feat_processing(feat_proc_options_t init_struct){
	
	v_feat_proc_options = init_struct;
	v_mean = (double*)malloc(sizeof(double)*v_feat_proc_options.nb_features);
	v_std_dev = (double*)malloc(sizeof(double)*v_feat_proc_options.nb_features);
	v_training_set = (double*)malloc(sizeof(double)*v_feat_proc_options.nb_features*v_feat_proc_options.nb_train_samples);
	
	v_feature_vect.ptr = (unsigned char*)malloc(sizeof(double)*v_feat_proc_options.nb_features);
	v_feature_vect.nb_features = v_feat_proc_options.nb_features;
	v_feature_array = v_feature_vect.ptr;
	
	return EXIT_SUCCESS;
}

int train_feat_processing(){

	int i,j;
	
	/*start acquisition*/
	for(i=0;i<v_feat_proc_options.nb_train_samples;i++){
		
		/*log the next sequence of samples*/
		READ_FEAT_FC(&v_feature_vect);
		
		for(j=0;j<v_feat_proc_options.nb_features;j++){
			v_training_set[i*v_feat_proc_options.nb_features+j] = v_feature_array[j];
		}
	
	}
	
	/*extract the training set parameters:*/
	/* -compute the mean*/
	stat_mean(v_training_set, v_mean, v_feat_proc_options.nb_train_samples, v_feat_proc_options.nb_features);
	
	/* -compute the standard deviation*/
	stat_std(v_training_set, v_mean, v_std_dev, v_feat_proc_options.nb_train_samples, v_feat_proc_options.nb_features);
	
	printf("acquiring training set, done\n");
	
	
	printf("Mean per features:\n");
	
	for(j=0;j<v_feat_proc_options.nb_features;j++){
		printf("%i:%f\n",j,v_mean[j]);
	}
	
	printf("Std. dev. per features:\n");
	for(j=0;j<v_feat_proc_options.nb_features;j++){
		printf("%i:%f\n",j,v_std_dev[j]);
	}
	
	return EXIT_SUCCESS;
}


int get_normalized_sample(feature_t* feature_vect){
	
	int i;
	double* features =  (double*)feature_vect->ptr;
	
	/*wait for a sample*/
	READ_FEAT_FC(feature_vect);
	
	/*normalize each features*/
	for(i=0;i<v_feat_proc_options.nb_features;i++){
		features[i] = (features[i]-v_mean[i])/v_std_dev[i];
	}
	
	return EXIT_SUCCESS;
}


int clean_up_feat_processing(){
	
	free(v_mean);
	free(v_std_dev);
	free(v_feature_vect.ptr);
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
		std[j] /= dim_i;
		std[j] = sqrt(std[j]);
	} 
}
