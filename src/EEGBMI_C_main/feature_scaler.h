#ifndef FEAT_SCALER_H
#define FEAT_SCALER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

typedef enum Escaling_options{
	
	STANDARTISATION = 0, /*this option removes the mean and divides by the standard deviation*/
	SCALING = 1 /*this option divides by the max absolute value (if max is 0, the divider is set to 1)*/
	
}scaling_option;

typedef struct Sfeat_scaler{
	int nb_features;
	double *feat_mean;
	double *feat_std;
	double *feat_max;
}t_feat_scaler;

t_feat_scaler *init_feat_scaler(double **feature_array, int nb_samples, int nb_features);

int kill_feat_scaler(t_feat_scaler *pfeat_scaler);

int scale_features(t_feat_scaler *pfeat_scaler, double **feature_array, int nb_samples, int nb_features, scaling_option option);

int save_feat_scaler(t_feat_scaler *pfeat_scaler, char* filename);
t_feat_scaler *load_feat_scaler(char* filename);



#endif
