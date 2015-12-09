/**
 * @file preprocess_core.c
 * @author Frederic Simard (fred.simard@atlantsembedded.com) | Atlants Embedded
 * @brief Signal preprocessing core. It can let the raw data flow through,
 *        compute the fft or the EEG freq power bands.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "xml.h"
#include "preprocess_core.h"
#include "data_structure.h"

/*data properties*/
static int nb_channels = 0;
static int nb_samples = 0;
static int feature_vect_length = 0;

/*preprocessing options*/
static char timeseries_enabled = 0x00;
static char fft_enabled = 0x00;
static char alpha_pwr_enabled = 0x00;
static char beta_pwr_enabled = 0x00;
static char gamma_pwr_enabled = 0x00;

/*prepocessing buffers*/
double* signals_avg_vector = NULL;
double* signals_wo_avg = NULL;
double* signals_transposed = NULL;
double* dft_vector = NULL;

/*function required for preprocessing*/
void remove_mean_col(double *a, double *mean, double *b, int dim_i, int dim_j);
void abs_powerdft_interval(const double *signal, double *abs_power_interval, int n, int interval_start, int interval_stop);
void stat_mean(double *a, double *mean, int dim_i, int dim_j);
void mtx_transpose(double *A, double *A_prime, int dim_i, int dim_j);
void abs_dft_interval(const double *signal, double *abs_power_interval, int n, int interval_start, int interval_stop);

/**
 * int init_preprocess_core(appconfig_t *config)
 * @brief initialize the preprocessing core
 * @param config, configuration options
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int init_preprocess_core(appconfig_t *config){
	
	/*copy infos about input data*/
	nb_channels = config->nb_channels;
	nb_samples = config->window_width;
	feature_vect_length = 0;
	
	/*adjust the size of the feature buffer according to selected options*/ 
	/*timeseries*/
	if(config->timeseries){
		feature_vect_length += config->window_width*config->nb_channels;
		timeseries_enabled = 0x01;
	}
	
	/*fft*/
	if(config->fft){
		feature_vect_length += config->window_width/2*config->nb_channels;
		fft_enabled = 0x01;
	}
	
	/*EEG power bands*/
	/*alpha*/
	if(config->power_alpha){
		feature_vect_length += config->nb_channels;
		alpha_pwr_enabled = 0x01;
	}
	/*beta*/
	if(config->power_beta){
		feature_vect_length += config->nb_channels;
		beta_pwr_enabled = 0x01;
	}
	/*gamma*/
	if(config->power_gamma){
		feature_vect_length += config->nb_channels;
		gamma_pwr_enabled = 0x01;
	}
	
	/*allocate memory for buffer required during processing*/
	signals_avg_vector = (double*)malloc(config->nb_channels*sizeof(double));
	signals_wo_avg = (double*)malloc(config->nb_channels*config->window_width*sizeof(double));
	signals_transposed = (double*)malloc(config->nb_channels*config->window_width*sizeof(double));
	dft_vector = (double*)malloc(feature_vect_length*sizeof(double));
	
	/*done*/
	return EXIT_SUCCESS;
}

/**
 * int cleanup_preprocess_core()
 * @brief cleanup preprocessing core
 * @return EXIT_SUCCESS
 */
void cleanup_preprocess_core(){
	
	/*free the memory*/
	free(signals_avg_vector);
	free(signals_wo_avg);
	free(signals_transposed);
	free(dft_vector);
}

/**
 * int preprocess_data(data_t* data_input, data_t* feature_output)
 * @brief Transform the raw data into a feature vector, according to selected options
 * @param data_input(in), input buffer
 * @param feature_output(out), output buffer
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int preprocess_data(data_t* data_input, data_t* feature_output){
	
	int i,j;
	int offset = 0;
	double* signals_array = (double*)data_input->ptr;
	double* features_array = (double*)feature_output->ptr;
	
	/*compute the average of each signal*/
	stat_mean(signals_array, signals_avg_vector, nb_samples, nb_channels);
	
	/*remove the average of the signals*/
	remove_mean_col(signals_array, signals_avg_vector, signals_wo_avg, nb_samples, nb_channels);
	
	/*transpose the signal array*/
	mtx_transpose(signals_wo_avg, signals_transposed, nb_samples, nb_channels);
	
	/*TODO: raw data pass-through*/
	
	/*If fft active*/
	if(fft_enabled){
		/*compute abs power-dft for each each channel*/
		for(i=0;i<nb_channels;i++){
			
			/*squared dft values*/
			abs_dft_interval(&(signals_transposed[i*nb_samples]), dft_vector, nb_samples, 0, nb_samples/2);
			
			/*copy to output buffer*/
			for(j=0;j<nb_samples/2;j++){
				features_array[i*nb_samples/2+j + offset] = dft_vector[j];
			}
		}
	}
	
	/*TODO: EEG power band:alpha*/
	
	/*TODO: EEG power band:beta*/
	
	/*TODO: EEG power band:gamma*/
	
	/*TODO: Eye blink detection*/
	
	/*TODO: artifact detection*/
	
	return EXIT_SUCCESS;
}

/**
 * void abs_dft_interval(const double *signal, double *abs_power_interval, int n, int interval_start, int interval_stop)
 * @brief This should come from signal_proc_lib.so
 */
void abs_dft_interval(const double *signal, double *abs_power_interval, int n, int interval_start, int interval_stop){
	
	int k;
	int coef_idx = 0;
	
	/*loop through all coefficients*/
	for (k = interval_start; k < interval_stop; k++) {
		
		/*computing registers*/
		double sumreal = 0;
		double sumimag = 0;
		int t;
		
		/*compute each terms*/
		for(t = 0; t < n; t++) {
			double angle = -2*M_PI * ((long long)t * k % n) / n;
			sumreal += signal[t]*cos(angle);
			sumimag += signal[t]*sin(angle);
		}
		
		/*compute 2*real value, to get abs fft*/
		abs_power_interval[coef_idx] = 2*sqrt(sumreal*sumreal+sumimag*sumimag)/n;
		coef_idx++;
	}
}  

/**
 * void remove_mean_col(double *a, double *mean, double *b, int dim_i, int dim_j)
 * @brief This should come from stats_lib.so
 */
void remove_mean_col(double *a, double *mean, double *b, int dim_i, int dim_j){
	 
	int i,j;
	
	/*remove the mean of all data samples*/
	for (i=0;i<dim_i;i++){
        for (j=0;j<dim_j;j++){
            b[i*dim_j+j] = a[i*dim_j+j]-mean[j];
		} 
	}
}

/**
 * void remove_mean_col(double *a, double *mean, double *b, int dim_i, int dim_j)
 * @brief This should come from stats_lib.so
 */
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

/**
 * void remove_mean_col(double *a, double *mean, double *b, int dim_i, int dim_j)
 * @brief This should come from lin_algebra.so
 */
void mtx_transpose(double *A, double *A_prime, int dim_i, int dim_j){
	
	int i,j = 0;
	
	/*transpose the matrix such that A'[j,i] = A[i,j]*/
	for (i = 0; i < dim_i; i++){
		for (j = 0; j < dim_j; j++){
			A_prime[j*dim_i + i] = A[i*dim_j + j];
		}
	}
}
