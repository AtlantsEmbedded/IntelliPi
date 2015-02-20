#ifndef FDA_H
#define FDA_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Cholesky>
#include <Eigen/Eigenvalues>

#define LABEL_A 1
#define LABEL_B -1
#define NB_FEATURES 2400


typedef struct Sfda_options{
	int nb_classes; //not supported, yet
	int nb_features;
}t_fda_options;

typedef struct Sfda{
	
	t_fda_options options;
	int max_eigen_value_idx;
	
	Eigen::MatrixXd eigen_vectors;
	Eigen::VectorXd eigen_values;
	
}t_fda;

t_fda *init_fda(double** dataset, int *labels, int nb_of_samples, t_fda_options options);
void kill_fda(t_fda *pfda);

int transform_data(t_fda *pfda, double **dataset, double *fdaed_dataset, int nb_samples);

//save_fda
//load_fda

#endif
