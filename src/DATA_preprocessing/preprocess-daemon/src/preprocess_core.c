
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "xml.h"
#include "preprocess_core.h"
#include "data_structure.h"

int nb_channels;
int nb_samples;
int feature_vect_length;

char timeseries_enabled = 0x00;
char fft_enabled = 0x00;
char alpha_pwr_enabled = 0x00;
char beta_pwr_enabled = 0x00;
char gamma_pwr_enabled = 0x00;

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
void abs_dft_interval(const double *signal, double *abs_power_interval, int n, int interval_start, int interval_stop);

int init_preprocess_core(appconfig_t *config){
	
	/*copy infos about input data*/
	nb_channels = config->nb_channels;
	nb_samples = config->window_width;
	feature_vect_length = 0;
	
	/*adjust the size of the feature buffer according to selected options*/ 
	if(config->timeseries){
		feature_vect_length += config->window_width*config->nb_channels;
		timeseries_enabled = 0x01;
	}
	
	if(config->fft){
		feature_vect_length += config->window_width/2*config->nb_channels;
		fft_enabled = 0x01;
	}
	
	if(config->power_alpha){
		feature_vect_length += config->nb_channels;
		alpha_pwr_enabled = 0x01;
	}
	
	if(config->power_beta){
		feature_vect_length += config->nb_channels;
		beta_pwr_enabled = 0x01;
	}
	
	if(config->power_gamma){
		feature_vect_length += config->nb_channels;
		gamma_pwr_enabled = 0x01;
	}
	
	/*allocate memory for buffer required during processing*/
	signals_avg_vector = (double*)malloc(config->nb_channels*sizeof(double));
	signals_wo_avg = (double*)malloc(config->nb_channels*config->window_width*sizeof(double));
	signals_transposed = (double*)malloc(config->nb_channels*config->window_width*sizeof(double));
	dft_vector = (double*)malloc(feature_vect_length*sizeof(double));
	
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
	
	int i,j;
	double* signals_array = (double*)data_input->ptr;
	double* features_array = (double*)feature_output->ptr;
	
	printf("preprocessing a sample\n");
	
	/*compute the average of each signal*/
	stat_mean(signals_array, signals_avg_vector, nb_samples, nb_channels);
	
	/*remove the average of the signals*/
	remove_mean_col(signals_array, signals_avg_vector, signals_wo_avg, nb_samples, nb_channels);
	
	/*transpose the signal array*/
	mtx_transpose(signals_wo_avg, signals_transposed, nb_samples, nb_channels);
	
	if(fft_enabled){
		/*compute abs power-dft for each each channel*/
		for(i=0;i<nb_channels;i++){
			/*squared dft values*/
			abs_dft_interval(&(signals_transposed[i*nb_samples]), dft_vector, nb_samples, 0, nb_samples/2);
			
			for(j=0;j<nb_samples/2;j++){
				features_array[i*nb_samples/2+j] = dft_vector[j];
			}
		}
	}
	
	for(i=0;i<nb_samples/2;i++){
		printf("fft[i]: \t%f \t%f \t%f \t%f\n",features_array[i], features_array[i+nb_samples/2], features_array[i+2*nb_samples/2], features_array[i+3*nb_samples/2]);
	}
	
	return EXIT_SUCCESS;
}


void abs_dft_interval(const double *signal, double *abs_power_interval, int n, int interval_start, int interval_stop){
	int k;
	int coef_idx = 0;
	for (k = interval_start; k < interval_stop; k++) {
		
		double sumreal = 0;
		double sumimag = 0;
		int t;
		
		for (t = 0; t < n; t++) {
			double angle = -2*M_PI * ((long long)t * k % n) / n;
			sumreal += signal[t]*cos(angle);
			sumimag += signal[t]*sin(angle);
		}
		
		abs_power_interval[coef_idx] = 2*sqrt(sumreal*sumreal+sumimag*sumimag)/n;
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
