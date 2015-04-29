#ifndef LIN_ALGEBRA_H
#define LIN_ALGEBRA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
/* Eigen solver            */
/***************************/
/*A => Tm*/
void mtx_lanczos_procedure(double *A, double *Tm, int n, int m);


#endif




