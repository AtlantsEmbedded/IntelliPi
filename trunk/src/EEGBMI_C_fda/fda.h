#ifndef FDA_H
#define FDA_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Cholesky>

#define LABEL_A 1
#define LABEL_B 2
#define NB_FEATURES 2400

typedef struct Sfda{
	Eigen::MatrixXd eigen_vectors;
	Eigen::VectorXd eigen_values;
	int max_eigen_value_idx;
}t_fda;


t_fda *init_fda(double** dataset, int *labels, int nb_of_samples);
void kill_fda(t_fda *pfda);

t_fda *transform_data(t_fda *pfda, double** dataset);

//save_fda
//load_fda

#endif
