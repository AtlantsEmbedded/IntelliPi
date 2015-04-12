#ifndef LIN_ALGEBRA_H
#define LIN_ALGEBRA_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/***************************/
/* Basic Matrix operations */
/***************************/
/*A(i,j)*B(j,k) = C(i,k)*/
void mtx_mult(double *A, double *B, double *C, int dim_i, int dim_j, int dim_k);

/*A = A'*/
void mtx_transpose(double *A, double *A_prime, int dim_i, int dim_j);

/*A = LL'*/
double *mtx_chol(double *A, int n);

void show_matrix(double *A, int n);

/***************************/
/* Basic Vector operations */
/***************************/
/*a = a+b*/
void vect_add(double* a, double b*, int n);

/*a = a-b*/
void vect_sub(double* a, double b*, int n)

/*a = a*b, where b is a scalar*/
void vect_scalar_multiply(double* a, double b, int n);

/*|a|*/
double vect_norm(double* a, int n)

/*a.b = sum(a[i]*b[i]) over all i*/
double vect_dot_product(double* a, double b*, int n)

/*V => random and |V|==1*/
double* vect_rand_unit(int length)

/***************************/
/* Eigen solver            */
/***************************/
/*A => Tm*/
void mtx_lanczos_procedure(double *A, double *Tm, int n, int m);


#endif




