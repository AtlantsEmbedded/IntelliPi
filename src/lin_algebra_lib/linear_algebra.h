#ifndef LIN_ALGEBRA_H
#define LIN_ALGEBRA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "includes/f2c.h"
#include "includes/clapack.h"

/***************************/
/* Basic Vector operations */
/***************************/
/*a = a+b*/
void vect_add(double *a, double *b, int n);

/*a = a-b*/
void vect_sub(double *a, double *b, int n);

/*a = a*b, where b is a scalar*/
void vect_scalar_multiply(double *a, double b, int n);

/*dot product*/
/*a.b = sum(a[i]*b[i]) over all i*/
double vect_dot_product(double *a, double *b, int n);

/*cross product*/
/*axb = */

/*vector norm*/
/*|a|*/
double vect_norm(double *a, int n);

/*V => random and |V|==1*/
void vect_rand_unit(double *a, int length);

/*show the vector in command window*/
void show_vector(double *a, int n);
void convert_double_to_real_array(double* double_array, real* real_array, int n);
/***************************/
/* Basic Matrix operations */
/***************************/
/*C(i,k) = sum_over_j(A(i,j)*B(j,k))*/
void mtx_mult(double *A, double *B, double *C, int dim_i, int dim_j, int dim_k);

/*A = A'*/
void mtx_transpose(double *A, double *A_prime, int dim_i, int dim_j);

/*A = LL'*/
void mtx_chol(double *A, double *L, int n);

void show_matrix(double *A, int dim_i, int dim_j);

/***************************/
/* Linear equation solvers */
/***************************/
/*Solve system of linear equations of the type: */
/*AX = B*/
/*in which x is the unknown.*/
/*A must be PSD*/
void lin_solve_PSD(double *A, double *X, double *B, int dim_i, int dim_j);

/*Solve system of linear equations of the type: */
/*AX = B*/
/*in which x is the unknown.*/
/*A is trianguler (lower or upper)*/
void lin_solve_triangular_equ(double *tri_mtx, double *Z, double *B, int dim_i, int dim_j, char lower);

/***************************/
/* Eigen solver            */
/***************************/
/*A => Tm*/
void mtx_lanczos_procedure(double *A, double *a, double *b, int n, int m);

/*Get eigenvalues of T*/
void mtx_mrrr(double *a,double *b, double *eigvalues, int n);

void mtx_ident(double *I,int n);

void stat_mean(double *a, double *mean, int dim_i, int dim_j);

void mtx_deriv_mean(double *b, double *a, double *mean, int dim_i, int dim_j);

void compute_eigen_problem(double *eigenvalues, double Identity, int dim_i, int dim_j);
#endif




