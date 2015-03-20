/**
 * @file linear_algebra.c
 * @author Frédéric Simard, Atom embedded
 * @date March, 2015
 * @brief This library implements various linear algebra methods used
 * for embedded Machine Learning Systems. At the present the methods 
 * allow for:
 * 
 *   - Cholesky decomposition
 *   - Matrix multiplication
 *   - Matrix transpose
 * 
 *   - Show matrix
 * 
 * It should be noted that the library is row major. Matrix must be organized
 * such that the row elements are contiguous in memory.
 * 
 * Developed using:
 *        http://rosettacode.org/wiki/Cholesky_decomposition
 *
 */
 
#include "linear_algebra.h"
 
 /**
 * double *mtx_chol(double *A, int n) 
 * 
 * @brief Computes the cholesky factorization of a n-square matrix, such that A = LL'
 * refer to: http://rosettacode.org/wiki/Cholesky_decomposition
 * @param A, NxN double array, the matrix to be factorized
 * @param n, the value of N.
 * @return a NxN square array containing the triangular L matrix resulting from the factorization
 */ 
double *mtx_chol(double *A, int n) {
	
#if 1	
	int i,j,k;
	
	/*allocate memory for resulting matrix L*/
    double *L = (double*)calloc(n * n, sizeof(double));
    
    if (L == NULL)
        exit(EXIT_FAILURE);
 
	/*compute all terms of the lower triagular matrix*/
    for(i = 0; i < n; i++){
        for(j = 0; j < (i+1); j++){
			
            double s = 0;
            
            /*sum up the terms*/
            for(k = 0; k < j; k++){
                s += L[i * n + k] * L[j * n + k];
			}
			
			/*check if on the diagonal*/
			if(i==j){
				/*yes, compute the sqrt*/
				L[i * n + j] = sqrt(A[i * n + i] - s);
			}
			else{
				/*no, compute the sqrt*/
				L[i * n + j] = (1.0 / L[j * n + j] * (A[i * n + j] - s));
			}
        }
	}
	
	/*return the cholesky factor*/
    return L;
#endif
    
}

 /**
 * double *mtx_mult(double *A, double *B, int dim_i, int dim_j, int dim_k)
 * 
 * @brief Multiply matrix A and B such that C = AB.
 * @param A, IxJ double array, the left matrix to be multiplied
 * @param B, JxK double array, the right matrix to be multiplied
 * @param dim_i, the value of I.
 * @param dim_j, the value of J.
 * @param dim_k the value of K.
 * @return a IxK array containing resulting matrix C
 */ 
double *mtx_mult(double *A, double *B, int dim_i, int dim_j, int dim_k){
	
	int i,j,k = 0;
	
	/**allocate memory for C*/
	double* C = (double*)calloc(dim_i * dim_k, sizeof(double));
	
	/*all elements of the matrix*/
	for (k = 0; k < dim_k; k++){
	for (i = 0; i < dim_i; i++){
			
			/*init the matrix element*/
			C[i*dim_k + k] = 0.0;
			
			/*dot(line i of A, column k of B), for element C[i,k]*/
			for (j = 0; j < dim_j; j++){
				C[i*dim_k + k] += A[i*dim_j+j]*B[j*dim_k+k];
			}
		}
	}
	
	return C;
	
}

 /**
 * double *mtx_transpose(double *A, int dim_i, int dim_j)
 * 
 * @brief Transpose matrix A -> A'
 * @param A, IxJ double array, the matrix to be transposed
 * @param dim_i, the value of I.
 * @param dim_j, the value of J.
 * @return a IxK square array containing the triangular L matrix resulting from the factorization
 */ 
double *mtx_transpose(double *A, int dim_i, int dim_j){
	
	int i,j = 0;
	
	/*allocate memory for A'*/
	double* A_prime = (double*)calloc(dim_j * dim_i, sizeof(double));
	
	/*transpose the matrix such that A'[j,i] = A[i,j]*/
	for (i = 0; i < dim_i; i++){
		for (j = 0; j < dim_j; j++){
			A_prime[i*dim_j + j] = A[j*dim_i + i];
		}
	}
	
	return A_prime;
}

 
void show_matrix(double *A, int n) {
	int i,j;
	
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++)
            printf("%2.5f ", A[i * n + j]);
        printf("\n");
    }
}
 




