
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "xml.h"
#include "preprocess_core.h"
#include "data_structure.h"

int nb_channels;
int nb_samples;

double* signals_avg_vector;
double* signals_wo_avg;
double* signals_transposed;
double* dft_vector;

/*function required for preprocessing*/
void remove_mean_col(double *a, double *mean, double *b, int dim_i, int dim_j);
void abs_powerdft_interval(const double *signal, double *abs_power_interval, int n, int interval_start, int interval_stop);
void stat_mean(double *a, double *mean, int dim_i, int dim_j);
void mtx_transpose(double *A, double *A_prime, int dim_i, int dim_j);
double stat_vect_mean(double *a, int n);

int init_preprocess_core(appconfig_t *config){
	
	/*copy infos about input data*/
	nb_channels = config->nb_channels;
	nb_samples = config->buffer_depth;
	
	/*allocate memory for buffer required during processing*/
	signals_avg_vector = (double*)malloc(config->nb_channels*sizeof(double));
	signals_wo_avg = (double*)malloc(config->nb_channels*config->buffer_depth*sizeof(double));
	signals_transposed = (double*)malloc(config->nb_channels*config->buffer_depth*sizeof(double));
	dft_vector = (double*)malloc(3*sizeof(double));
	
	return EXIT_SUCCESS;
}


int cleanup_preprocess_core(){
	
	/*free the memory*/
	free(signals_avg_vector);
	free(signals_wo_avg);
	free(signals_transposed);
	free(dft_vector);
	
	return EXIT_SUCCESS;
}


int preprocess_data(data_t* data_input, data_t* feature_output){
	
	int i;
	double* signals_array = (double*)data_input->ptr;
	double* features_array = (double*)feature_output->ptr;
	
	printf("preprocessing a sample\n");
	
	/*compute the average of each signal*/
	stat_mean(signals_array, signals_avg_vector, nb_samples, nb_channels);
	
	/*remove the average of the signals*/
	remove_mean_col(signals_array, signals_avg_vector, signals_wo_avg, nb_samples, nb_channels);
	
	/*transpose the signal array*/
	mtx_transpose(signals_wo_avg, signals_transposed, nb_samples, nb_channels);
	
	/*compute abs power-dft for each each channel*/
	for(i=0;i<nb_channels;i++){
		/*squared dft values*/
		abs_powerdft_interval(&(signals_transposed[i*nb_samples]), dft_vector, 110, 9, 12);
		
		/*average squared dft values and*/
		/*store in feature vector*/
		features_array[i] = stat_vect_mean(dft_vector, 3);
	}
	
	return EXIT_SUCCESS;
}


void abs_powerdft_interval(const double *signal, double *abs_power_interval, int n, int interval_start, int interval_stop){
	int k;
	int coef_idx = 0;
	double n_sqr = n*n;
	for (k = interval_start; k < interval_stop; k++) {
		
		double sumreal = 0;
		double sumimag = 0;
		int t;
		
		for (t = 0; t < n; t++) {
			double angle = -2*M_PI * ((long long)t * k % n) / n;
			sumreal += signal[t]*cos(angle);
			sumimag += signal[t]*sin(angle);
		}
		
		abs_power_interval[coef_idx] = (sumreal*sumreal+sumimag*sumimag)/n_sqr;
		coef_idx++;
	}
}

void remove_mean_col(double *a, double *mean, double *b, int dim_i, int dim_j){
	 
	int i,j;
	
	for (i=0;i<dim_i;i++){
        for (j=0;j<dim_j;j++){
            b[i*dim_j+j] = a[i*dim_j+j]-mean[j];
		} 
	}
}

void stat_mean(double *a, double *mean, int dim_i, int dim_j){
	
	int i,j,n;
	n=0;
	
	/*Sum over each column*/
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

double stat_vect_mean(double *a, int n){
	
	int i;
	double sum = 0.0;
	
	/*Sum*/
	for(i=0;i<n;i++){
		 sum += a[i];
	}
	
	return sum/=n;
  
}

void mtx_transpose(double *A, double *A_prime, int dim_i, int dim_j){
	
	int i,j = 0;
	
	/*transpose the matrix such that A'[j,i] = A[i,j]*/
	for (i = 0; i < dim_i; i++){
		for (j = 0; j < dim_j; j++){
			A_prime[j*dim_i + i] = A[i*dim_j + j];
		}
	}
}
