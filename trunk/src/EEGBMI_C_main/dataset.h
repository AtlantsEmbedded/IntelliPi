#ifndef DATASET_H
#define DATASET_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <fft.h>

#define TIMESERIES_LENGTH 600
#define NB_CHANNELS 4
#define FFTED_VECTOR_LENGTH TIMESERIES_LENGTH*NB_CHANNELS
#define TRAIN_TRIALS_PROP 0.85

typedef struct Ssampledata{
	double **timeseries; // timeseries associated with each trials, one per channels
	double **fft_vectors; // fft_vectors, computed from time series
}t_sampledata;

typedef struct Sdataset {
	
	/*Labels and information relative to each samples*/
	int *labels; 
	t_sampledata *samplesdata;
	
	/*nb of train and test trials*/
	int nb_trials;
	int nb_train_trials;
	int nb_test_trials;
	
	/*list of train and test trials*/
	int* list_of_train_trials;
	int* list_of_test_trials;

}t_dataset;

/*function call to init a dataset*/
t_dataset *init_dataset(const char* path);
void kill_dataset(t_dataset *pdataset);

int preprocess_data(t_dataset *pdataset);
int split_train_test_sets(t_dataset *pdataset);

int get_train_dataset_ffted(t_dataset *pdataset, double **feature_vectors, int* labels);
int get_test_dataset_timeseries(t_dataset *pdataset, double **timeseries_vectors, int* labels);

#endif
