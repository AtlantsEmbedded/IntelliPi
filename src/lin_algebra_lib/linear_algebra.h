#ifndef LIN_ALGEBRA_H
#define LIN_ALGEBRA_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double *mtx_chol(double *A, int n);

double *mtx_mult(double *A, double *B, int dim_i, int dim_j, int dim_k);

double *mtx_transpose(double *A, int dim_i, int dim_j);

void show_matrix(double *A, int n);

#endif




