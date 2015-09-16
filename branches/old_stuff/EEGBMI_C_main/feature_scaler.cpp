/**
 * @file feature_scaler.cpp
 * @author Frédéric Simard
 * @date February, 2015
 * @brief This file implements a feature scaler for the purpose of machine learning
 *  	  applications. Generally, it is better to scale the features of a dataset
 * 		  before feading it into a classifier of any kind. This lib offers the possibility
 * 	      to use one of two scaling techniques. 
 * 			STANDARTISATION: remove mean and divide by standard deviation
 * 			SCALING: divide by max value
 * 
 *        Feel free to use, modify and distribute, but please respect authorship
 *        Atom Embedded, 2015.
 */


#include "feature_scaler.h"


int compute_mean(double **feature_array, double *mean_vector, int nb_samples, int nb_features);
int compute_std(double **feature_array, double *mean_vector, double *std_vector, int nb_samples, int nb_features);
int compute_max(double **feature_array, double *max_vector, int nb_samples, int nb_features);

/**
 * t_feat_scaler *init_feat_scaler(double **feature_array, int nb_samples, int nb_features)
 * 
 * @brief Initialize a feature scaler. Computes all info required from the training set
 * @param feature_array, NxF array, where N is the number of samples and F is the number of features
 * @param nb_samples, value of N
 * @param nb_features, value of F
 * @return a feature scaler ready for use
 */
t_feat_scaler *init_feat_scaler(double **feature_array, int nb_samples, int nb_features){
	
	t_feat_scaler *pfeat_scaler = (t_feat_scaler*)malloc(sizeof(t_feat_scaler));
	
	/*allocate memory for the scaler*/
	pfeat_scaler->nb_features = nb_features;
	pfeat_scaler->feat_mean = (double*)malloc(sizeof(double)*nb_features);
	pfeat_scaler->feat_std = (double*)malloc(sizeof(double)*nb_features);
	pfeat_scaler->feat_max = (double*)malloc(sizeof(double)*nb_features);
	
	/*compute the parameters of the scaler*/
	compute_mean(feature_array, pfeat_scaler->feat_mean, nb_samples, nb_features);
	compute_std(feature_array, pfeat_scaler->feat_mean, pfeat_scaler->feat_std, nb_samples, nb_features);
	compute_max(feature_array, pfeat_scaler->feat_max, nb_samples, nb_features);
	
	/*return the scaler*/
	return pfeat_scaler;
}

/**
 * int kill_feat_scaler(t_feat_scaler *pfeat_scaler)
 * 
 * @brief Clear up the memory used by a feature scaler
 * @param pfeat_scaler, the scaler to be erased
 * @return EXIT_SUCCESS
 */
int kill_feat_scaler(t_feat_scaler *pfeat_scaler){
	
	/*free up the memory used by the scaler*/
	free(pfeat_scaler->feat_mean);
	free(pfeat_scaler->feat_std);
	free(pfeat_scaler->feat_max);
	free(pfeat_scaler);
	
	return EXIT_SUCCESS;
}

/**
 * int scale_features(t_feat_scaler *pfeat_scaler, double **feature_array, int nb_samples, int nb_features, scaling_option option)
 * 
 * @brief Scales the feature received in input using the parameters found during initialization
 * @param feature_array, NxF array to be scaled, where N is the number of samples and F is the number of features
 * @param nb_samples, value of N
 * @param nb_features, value of F
 * @param option, type of scaling required (SCALING: divide by max value, STANDARDISATION: remove mean and divide by std_dev)
 * @return EXIT_SUCCESS
 */
int scale_features(t_feat_scaler *pfeat_scaler, double **feature_array, int nb_samples, int nb_features, scaling_option option){
	
	int i,j;
	
	/*check out which option was given*/
	switch(option){
		
		/*STANDARDISATION: remove the mean and divide by standard deviation*/
		case STANDARTISATION:
		
				for(i=0;i<nb_samples;i++){
					for(j=0;j<nb_features;j++){
						feature_array[i][j] = (feature_array[i][j]-pfeat_scaler->feat_mean[j])/pfeat_scaler->feat_std[j];
					}
				}
		
			break;
			
		/*SCALING: divide by max absolute value*/
		case SCALING:

				for(i=0;i<nb_samples;i++){
					for(j=0;j<nb_features;j++){
						feature_array[i][j] = feature_array[i][j]/pfeat_scaler->feat_max[j];
					}
				}
				
			break;
			
		default:
			break;
		
	}
	
	return EXIT_SUCCESS;
}

/**
 * int save_feat_scaler(t_feat_scaler *pfeat_scaler, char* filename)
 * 
 * @brief Saves the scaler to a binary file
 * @param pfeat_scaler, the scaler to be used
 * @param filename, filename to be used
 * @return EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int save_feat_scaler(t_feat_scaler *pfeat_scaler, char* filename){
	
	/*open file to save to*/
	FILE* pfile = fopen(filename,"wb");
	
	if(!pfile){
		printf("Error: scaler save file cannot be opened\n");
		return EXIT_FAILURE;
	}
	
	/*write the scaler to binary file*/
	fwrite(&(pfeat_scaler->nb_features),sizeof(int),1,pfile);
	fwrite(pfeat_scaler->feat_mean,sizeof(double),pfeat_scaler->nb_features,pfile);
	fwrite(pfeat_scaler->feat_std,sizeof(double),pfeat_scaler->nb_features,pfile);
	fwrite(pfeat_scaler->feat_max,sizeof(double),pfeat_scaler->nb_features,pfile);
	
	return EXIT_SUCCESS;
}

/**
 * t_feat_scaler *load_feat_scaler(char* filename)
 * 
 * @brief Loads the scaler from a binary file
 * @param filename, filename to be used
 * @return a scaler ready for use if successful, NULL otherwise
 */
t_feat_scaler *load_feat_scaler(char* filename){
	
	
	/*init pointer*/
	t_feat_scaler *pfeat_scaler = (t_feat_scaler*)malloc(sizeof(t_feat_scaler));
	
	/*do and confirm success of file opening*/
	FILE* pfile = fopen(filename,"rb");
	
	if(!pfile){
		printf("Error: scaler read file cannot be opened\n");
		free(pfeat_scaler);
		return NULL;
	}
	
	/*read size from the file*/
	fread(&(pfeat_scaler->nb_features),sizeof(int),1,pfile);
	
	/*allocate memory*/
	pfeat_scaler->feat_mean = (double*)malloc(sizeof(double)*pfeat_scaler->nb_features);
	pfeat_scaler->feat_std = (double*)malloc(sizeof(double)*pfeat_scaler->nb_features);
	pfeat_scaler->feat_max = (double*)malloc(sizeof(double)*pfeat_scaler->nb_features);
	
	/*read parameters from the file*/
	fread(pfeat_scaler->feat_mean,sizeof(double),pfeat_scaler->nb_features,pfile);
	fread(pfeat_scaler->feat_std,sizeof(double),pfeat_scaler->nb_features,pfile);
	fread(pfeat_scaler->feat_max,sizeof(double),pfeat_scaler->nb_features,pfile);
	
	return pfeat_scaler;
}


/**
 * int compute_mean(double **feature_array, double *mean_vector, int nb_samples, int nb_features)
 * 
 * @brief Compute the mean vector for the scaler
 * @param feature_array, NxF array, where N is the number of samples and F is the number of features
 * @param (out)mean_vector, F long vector with the means of the features
 * @param nb_samples, nb of samples in the set
 * @param nb_features, nb of features in the set 
 * @return EXIT_SUCCESS if successful
 */
int compute_mean(double **feature_array, double *mean_vector, int nb_samples, int nb_features){
	
	int i,j;
	
	/*initialize the mean vector*/
	for(i=0;i<nb_features;i++){
		mean_vector[i] = 0.0;
	}
	
	/*sums up the feature wise elements*/
	for(i=0;i<nb_samples;i++){
		for(j=0;j<nb_features;j++){
			mean_vector[j] += feature_array[i][j];
			
		}
	}
	
	/*compute the mean value*/
	for(i=0;i<nb_features;i++){
		mean_vector[i] /= (double)nb_samples;
	}
	
	return EXIT_SUCCESS;
	
}

/**
 * int compute_std(double **feature_array, double *mean_vector, double *std_vector, int nb_samples, int nb_features)
 * 
 * @brief Compute the standard deviation vector for the scaler
 * @param feature_array, NxF array, where N is the number of samples and F is the number of features
 * @param mean_vector, F long vector with the means of the features
 * @param (out)std_vector, F long vector with the standard deviation of the features
 * @param nb_samples, nb of samples in the set
 * @param nb_features, nb of features in the set 
 * @return EXIT_SUCCESS if successful
 */
int compute_std(double **feature_array, double *mean_vector, double *std_vector, int nb_samples, int nb_features){
	
	int i,j;
	
	/*initialize the std vector*/
	for(i=0;i<nb_features;i++){
		std_vector[i] = 0.0;
	}
	
	/*sums up the squared distance between samples and mean, over all samples*/
	/*and for each feature.*/
	for(i=0;i<nb_samples;i++){
		for(j=0;j<nb_features;j++){
			std_vector[j] += (mean_vector[j]-feature_array[i][j])*(mean_vector[j]-feature_array[i][j]);
		}
	}
	
	/*compute the standard deviation*/
	for(i=0;i<nb_features;i++){
		std_vector[i] /= nb_samples;
		std_vector[i] = sqrt(std_vector[i]);
	}
}

/**
 * int compute_max(double **feature_array, double *mean_vector, int nb_samples, int nb_features)
 * 
 * @brief Compute the max features value for the scaler
 * @param feature_array, NxF array, where N is the number of samples and F is the number of features
 * @param (out)max_vector, F long vector with the max value of the features
 * @param nb_samples, nb of samples in the set
 * @param nb_features, nb of features in the set 
 * @return EXIT_SUCCESS if successful
 */
int compute_max(double **feature_array, double *max_vector, int nb_samples, int nb_features){
	
	int i,j;
	
	/*hypothesis, the first sample is the max...*/
	for(i=0;i<nb_features;i++){
		max_vector[i] = abs(feature_array[0][i]);
	}
	
	/*then find the max element over all samples for each features*/
	for(i=1;i<nb_samples;i++){
		for(j=0;j<nb_features;j++){
			if(abs(feature_array[i][j]) > max_vector[j]){
				max_vector[j] = abs(feature_array[i][j]);
			}
		}
	}
	
	/*Removes very small numbers and replace them with a 1*/
	/*this prevents division by zeros from occuring*/
	for(i=0;i<nb_features;i++){
		if(max_vector[i] < 0.000001){
			max_vector[i] = 1;
		}
	}
}
