/**
 * @file linear_algebra.c
 * @author Frédéric Simard, Atlans embedded
 * @date March, 2015
 * @brief This library implements various linear algebra methods used
 * for Embedded Machine Learning Systems. At the present the methods 
 * allow for:
 *
 *   Basic Vector operations
 *   - vector-vector addition
 *   - vector-vector subtraction
 *   - vector-scalar multiply
 *   - vector norm
 *   - vector dot product
 *  
 *   Basic Matrix operations
 *   - Matrix multiplication
 *   - Matrix transpose
 *   - Cholesky decomposition
 *
 *   Methods for Eigen problem solving
 *   - Lanczos algorithm
 * 
 *   Other utils
 *   - Show matrix
 *   - Generate Random unit length vector
 * 
 * It should be noted that this library is row major. Matrix must be organized
 * such that the row elements are contiguous in memory.
 * Ex: a 3x4 matrix should be:
 * 1234
 * 5678
 * 9012
 *
 * References:
 *    Cholesky decomposition:
 *    - http://rosettacode.org/wiki/Cholesky_decomposition
 *    
 *    Lanczos Procedure:
 *    - Cullum J and Willoughby (1981) Computing Eigenvalues of Very Large Symmetric Matrices - 
 *      An Implementation of Lanczos Algorithm with no Reorthogonalization. Computational Physics. 
 *      (44)329-358.
 *    - http://en.wikipedia.org/wiki/Lanczos_algorithm
 *
 *
 */
 
#include "linear_algebra.h"

/**
 * void vect_add(double* a, double b*, int n)
 * 
 * @brief computes the sum of vector a and b, such that a = a+b
 * @param n, length of the vectors
 */ 
void vect_add(double* a, double *b, int n)
{
	int i;
	
	for(i=0;i<n;i++){
		a[i] += b[i];
	}
}

/**
 * void vect_sub(double* a, double b*, int n)
 * 
 * @brief computes the sum of vector a and b, such that a = a-b
 * @param n, length of the vectors
 */ 
void vect_sub(double *a, double *b, int n)
{
	int i;
	
	for(i=0;i<n;i++){
		a[i] -= b[i];
	}
}


/**
 * void vect_scalar_multiply(double* a, double b, int n)
 * 
 * @brief computes the product between the vector a and the scalar b such that
 *        a = a*b
 * @param n, length of the vector
 */ 
void vect_scalar_multiply(double *a, double b, int n)
{
	int i;
	
	for(i=0;i<n;i++){
		a[i] *= b;
	}
}

/**
 * double vect_dot_product(double* a, double b*, int n)
 * 
 * @brief computes the dot (scalar) product between vector a and b
 * @param n, length of the vectors
 * @return the result of the dot product
 */ 
double vect_dot_product(double *a, double *b, int n)
{
	int i;
	double dot_product = 0.0;
	
	for(i=0;i<n;i++){
		dot_product += a[i]*b[i];
	}
	
	return dot_product;
}



/**
 * double vect_norm(double* a, int n)
 * 
 * @brief computes the norm |a| of vector a
 * @param n, length of the vector
 * @return the norm of the vector
 */ 
double vect_norm(double *a, int n)
{
	int i;
	double norm = 0.0;
	
	for(i=0;i<n;i++){
		norm += a[i]*a[i];
	}
	
	norm = sqrt(norm);
	
	return norm;
}

/**
 * void vect_rand_unit(double *a, int length)
 * 
 * @brief Utility function that generates a random unit vector
 * @param length, length of the vector
 * @return the random unit vector
 */ 
void vect_rand_unit(double *a, int length)
{
	int i,j;
	double vect_norm = 0.0;

	/*fill the vector with random numbers*/
	for(i=0;i<length;i++){
		a[i] = (double)rand()/RAND_MAX;
		vect_norm += a[i]*a[i];
	}
	
	/*adjust the norm*/
	/*vect = vect/|vect|*/
	vect_norm = sqrt(vect_norm);
	for(i=0;i<length;i++){	
		a[i] /= vect_norm;
	}
}
 

/**
 * void show_vector(double *a, int n)
 * 
 * @brief Utility function to show the vector in the command window
 * @param a, n-long double array, the vector to be displayed
 * @param n, length of the vector
 */  
void show_vector(double *a, int n)
{
	int i;
	
    for(i = 0; i < n; i++) {
        /*row major!*/
        printf("%2.4f ", a[i]);
    }	
    
    printf("\n");
}
 

 /**
 * void mtx_mult(double *A, double *B, int dim_i, int dim_j, int dim_k)
 * 
 * @brief Multiply matrix A and B such that C = AB.
 * @param A, IxJ double array, the left matrix to be multiplied
 * @param B, JxK double array, the right matrix to be multiplied
 * @param (out)C, IxK array containing resulting matrix
 * @param dim_i, the value of I.
 * @param dim_j, the value of J.
 * @param dim_k the value of K.
 */ 
void mtx_mult(double *A, double *B, double *C, int dim_i, int dim_j, int dim_k){
	
	int i,j,k = 0;
	
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
	
}

 /**
 * void mtx_transpose(double *A, double *A_prime, int dim_i, int dim_j)
 * 
 * @brief Transpose matrix A -> A'
 * @param A, IxJ double array, the matrix to be transposed
 * @param (out)A_prime, a JxI square array containing the transpose of A
 * @param dim_i, the value of I.
 * @param dim_j, the value of J.
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
 /**
 * double *mtx_chol(double *A, double *L, int n) 
 * 
 * @brief Computes the cholesky factorization of a n-square matrix, such that A = LL'
 * refer to: http://rosettacode.org/wiki/Cholesky_decomposition
 * @param A, NxN double array, the matrix to be factorized
 * @return (out)L, NxN square array containing the triangular L matrix resulting from the factorization
 * @param n, the value of N.
 */ 
void mtx_chol(double *A, double *L, int n) {
	
	int i,j,k;
	
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
	    
}




/**
 * void mtx_lanczos_procedure(double *A, double *Tm, int n, int m)
 * 
 * @brief Fonction that implements the first step of the eigenproblem solution
 *        based on lanzos algorithm. This function generates a matrix Tm that contains 
 *        a set (<=) of eigenvalues that approximate those of matrix A. Finding the eigenvalues
 *        in Tm is easier and serves as an optimization method for problems in which only a few 
 *        eigenpairs are required.
 * @param A, the matrix on which the procedure is applied. Needs to be square and Hermitian.
 * @param (out)Tm, a m*m tridiagonal square matrix Tm which contains a set of eigenvalues that approximate those found in A...
 * @param n, the dimensions of the square matrix A
 * @param m, the number of iterations for the lanczos procedure (and the dimensions of the array returned)
 */ 
void mtx_lanczos_procedure(double *A, double *Tm, int n, int m)
{
	int i,j,array_index_i;
	
	double* a = (double*)calloc(m, sizeof(double));
	double* b = (double*)calloc(m, sizeof(double));
	double* v_i = (double*)calloc(n, sizeof(double));
	double* a_times_v_i = (double*)calloc(n, sizeof(double));
	double* b_times_v_i_minus_one = (double*)calloc(n, sizeof(double));
	double* v_i_minus_one = (double*)calloc(n, sizeof(double));
	double* w_i = (double*)calloc(n, sizeof(double));
	double* v1 = (double*)calloc(n, sizeof(double));
	
	double* temp;

	/*get a n-long random vector with norm equal to 1*/
	vect_rand_unit(v1,n);
	
	/*first iteration of the procedure*/
	i = 1;
	array_index_i = i-1;
	
	/*computes w_i*/
	/*w[i] <= A*v[i]*/
	mtx_mult(A, v1, w_i, n, n, 1);
	
	/*computes a_i*/
	/*a[i] <= w[i]*v[i]*/
	a[array_index_i] = vect_dot_product(w_i, v1,n);
	
	/*update w_i*/
	/*w[i] <= w[i]-a[i]*v[i]-b[i]*v[i-1]*/
	/*b[1] = 0*/
	memcpy(a_times_v_i,v_i,n*sizeof(double));
	vect_scalar_multiply(a_times_v_i,a[array_index_i],n);
	vect_sub(w_i, a_times_v_i, n);
	
	/*computes next b_i*/
	/*b[i+1] = ||w[i]||*/
	b[array_index_i+1] = vect_norm(w_i,n);
	
	/*save current v_i*/
	temp = v_i_minus_one;
	v_i_minus_one = v_i;
	
	/*computes next v_i = w_i/b(i+1)*/
	/*v[i+1] = w[i]/b[i+1]*/
	v_i = w_i;
	vect_scalar_multiply(v_i, 1/b[array_index_i+1], n);
	
	/*reuse memory former v_i_minus_one space for w_i*/
	w_i = temp;
	
	/*rest of the iterations of the procedure*/
	for(i=2;i<=m-1;i++){
		array_index_i = i-1;
		
		/*computes w_i*/
		/*w[i] <= A*v[i]*/
		mtx_mult(A, v_i, w_i, n, n, 1);
		
		/*computes a_i*/
		/*a[i] <= w[i]*v[i]*/
		a[array_index_i] = vect_dot_product(w_i, v_i, n);
		
		/*update w_i*/
		/*prepare a[i]*v[i]*/
		memcpy(a_times_v_i,v_i,n*sizeof(double));
		vect_scalar_multiply(a_times_v_i,a[array_index_i],n);
		
		/*prepare b[i]*v[i-1]*/
		memcpy(b_times_v_i_minus_one,v_i_minus_one,n*sizeof(double));
		vect_scalar_multiply(b_times_v_i_minus_one,b[array_index_i],n);
		
		/*w[i] <= w[i]-a[i]*v[i]-b[i]*v[i-1]*/
		vect_sub(w_i, a_times_v_i,n);
		vect_sub(w_i, b_times_v_i_minus_one,n);
		
		/*computes next b_i*/
		/*b[i+1] = ||w[i]||*/
		b[array_index_i+1] = vect_norm(w_i,n);
		
		/*save current v_i*/
		temp = v_i_minus_one;
		v_i_minus_one = v_i;
		
		/*computes next v_i = w_i/b(i+1)*/
		/*v[i+1] = w[i]/b[i+1]*/
		v_i = w_i;
		vect_scalar_multiply(v_i, 1/b[array_index_i+1],n);
		
		/*reuse memory former v_i_minus_one space for w_i*/
		w_i = temp;	
	
	}
	
	/*compute last term a[m]*/
	array_index_i = m-1;
	a[array_index_i] = vect_dot_product(w_i,v_i,n);
	
	/*construct tridiagonal matrix tm*/
	Tm[0] = a[0];
	for(i=1;i<m-1;i++){
		Tm[i*m+i] = a[i]; 
		Tm[i*m+i-1] = b[i]; 
		Tm[i*m+i+1] = b[i]; 
	}
	Tm[m*m-1] = a[m-1];
	
	/*free memory space*/
	free(a);
	free(b);
	free(v_i);
	free(a_times_v_i);
	free(b_times_v_i_minus_one);
	free(v_i_minus_one);
	free(w_i);
} 
 
/**
 * void show_matrix(double *A, int dim_i, int dim_j)
 * 
 * @brief Utility function to show the matrix in the command window
 * @param A, IxJ double array, the matrix to be displayed
 * @param dim_i, the value of I.
 * @param dim_j, the value of J.
 */ 
void show_matrix(double *A, int dim_i, int dim_j) {
	int i,j;
	
    for(i = 0; i < dim_i; i++) {
        for(j = 0; j < dim_j; j++)
			/*row major!*/
            printf("%2.5f ", A[i * dim_j + j]);
        printf("\n");
    }
}
 




