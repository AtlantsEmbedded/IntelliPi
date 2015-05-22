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
 *   - Multiple Relatively Robust Representation
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

void convert_double_to_real_array(double* double_array, real* real_array, int n);
void convert_real_to_double_array(real* real_array, double* double_array, int n);

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
 * @brief Function that implements the first step of the eigenproblem solution
 *        based on lanzos algorithm. This function generates a matrix Tm that contains 
 *        a set (<=) of eigenvalues that approximate those of matrix A. Finding the eigenvalues
 *        in Tm is easier and serves as an optimization method for problems in which only a few 
 *        eigenpairs are required.
 * @param A, the matrix on which the procedure is applied. Needs to be square and Hermitian.
 * @param (out)Tm, a m*m tridiagonal square matrix Tm which contains a set of eigenvalues that approximate those found in A...
 * @param n, the dimensions of the square matrix A
 * @param m, the number of iterations for the lanczos procedure (and the dimensions of the array returned)
 */ 
void mtx_lanczos_procedure(double *A, double *a, double *b, int n, int m)
{
	int i,j,array_index_i;
	
	double* v_i = (double*)calloc(n, sizeof(double));
	double* a_times_v_i = (double*)calloc(n, sizeof(double));
	double* b_times_v_i_minus_one = (double*)calloc(n, sizeof(double));
	double* v_i_minus_one = (double*)calloc(n, sizeof(double));
	double* w_i = (double*)calloc(n, sizeof(double));
	double* v1 = (double*)calloc(n, sizeof(double));
	
	double* temp;

	/*get a n-long random vector with norm equal to 1*/
	//vect_rand_unit(v1,n);
	for(i=0;i<n;i++){
	    v1[i] = 1/sqrt(n);
	}
	
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
	/*note: b[1] = 0*/
	memcpy(a_times_v_i,v1,n*sizeof(double));
	vect_scalar_multiply(a_times_v_i,a[array_index_i],n);
	vect_sub(w_i, a_times_v_i, n);
	
	/*computes next b_i*/
	/*b[i+1] = ||w[i]||*/
	b[array_index_i+1] = vect_norm(w_i,n);

	/*copy v1 to v_i*/
	memcpy(v_i,v1,n*sizeof(double));
		
	/*save v[i] to be used asv[i-1]*/
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
	mtx_mult(A, v_i, w_i, n, n, 1);
	a[array_index_i] = vect_dot_product(w_i,v_i,n);
	
	/*translate data representation in the b matrix to match the CLAPACK standard*/
	for(i=0;i<(n-1);i++){
		b[i] = b[i+1];
	}
	b[n-1] = 0;
	
	free(v1);
	free(v_i);
	free(a_times_v_i);
	free(b_times_v_i_minus_one);
	free(v_i_minus_one);
	free(w_i);
	
} 

/**
 * void mtx_mrrr(double *a,double *b, double *eigvalues, int n)
 * 
 * @brief This function finds the eigenvalues of matrix T (represented by diagonal a and offdiagonal b) 
 *        using the multiple relatively robust representation algorithm.
 *        It's a wrapper around the stegr routine found in LAPACK.
 * @param a, the diagonal of tridiagonal matrix T
 * @param b, the off-diagonal of tridiagonal matrix T (size should n-1)
 * @param (out)eigvalues, the eigenvalues of the matrix
 * @param n, the dimensions of the square matrix T
 */ 
void mtx_mrrr(double *a,double *b, double *eigvalues, int n)
{

/*  JOBZ    (input) CHARACTER*1 */
/*          = 'N':  Compute eigenvalues only; */
/*          = 'V':  Compute eigenvalues and eigenvectors. */
char jobz = 'N';

/*  RANGE   (input) CHARACTER*1 */
/*          = 'A': all eigenvalues will be found. */
/*          = 'V': all eigenvalues in the half-open interval (VL,VU] */
/*                 will be found. */
/*          = 'I': the IL-th through IU-th eigenvalues will be found. */
char range = 'A';

/*  N       (input) INTEGER */
/*          The order of the matrix.  N >= 0. */

integer n_prime = (integer)n;

/*  D       (input/output) REAL array, dimension (N) */
/*          On entry, the N diagonal elements of the tridiagonal matrix */
/*          T. On exit, D is overwritten. */
real *d = (real*)malloc(sizeof(real)*n);
convert_double_to_real_array(a,d,n);


/*  E       (input/output) REAL array, dimension (N) */
/*          On entry, the (N-1) subdiagonal elements of the tridiagonal */
/*          matrix T in elements 1 to N-1 of E. E(N) need not be set on */
/*          input, but is used internally as workspace. */
/*          On exit, E is overwritten. */

real *e = (real*)malloc(sizeof(real)*(n-1));
convert_double_to_real_array(b,e,n-1);

/*  VL      (input) REAL */
/*  VU      (input) REAL */
/*          If RANGE='V', the lower and upper bounds of the interval to */
/*          be searched for eigenvalues. VL < VU. */
/*          Not referenced if RANGE = 'A' or 'I'. */
real *vl = NULL;
real *vu = NULL;

/*  IL      (input) INTEGER */
/*  IU      (input) INTEGER */
/*          If RANGE='I', the indices (in ascending order) of the */
/*          smallest and largest eigenvalues to be returned. */
/*          1 <= IL <= IU <= N, if N > 0. */
/*          Not referenced if RANGE = 'A' or 'V'. */
integer il = 0;
integer iu = 0;

/*  ABSTOL  (input) REAL */
/*          Unused.  Was the absolute error tolerance for the */
/*          eigenvalues/eigenvectors in previous versions. */
real *abstol = NULL;

/*  M       (output) INTEGER */
/*          The total number of eigenvalues found.  0 <= M <= N. */
/*          If RANGE = 'A', M = N, and if RANGE = 'I', M = IU-IL+1. */
integer m = 0;

/*  W       (output) REAL array, dimension (N) */
/*          The first M elements contain the selected eigenvalues in */
/*          ascending order. */
real *w = (real*)malloc(sizeof(real)*n);

/*  Z       (output) REAL array, dimension (LDZ, max(1,M) ) */
/*          If JOBZ = 'V', and if INFO = 0, then the first M columns of Z */
/*          contain the orthonormal eigenvectors of the matrix T */
/*          corresponding to the selected eigenvalues, with the i-th */
/*          column of Z holding the eigenvector associated with W(i). */
/*          If JOBZ = 'N', then Z is not referenced. */
/*          Note: the user must ensure that at least max(1,M) columns are */
/*          supplied in the array Z; if RANGE = 'V', the exact value of M */
/*          is not known in advance and an upper bound must be used. */
/*          Supplying N columns is always safe. */
real *z= NULL;

/*  LDZ     (input) INTEGER */
/*          The leading dimension of the array Z.  LDZ >= 1, and if */
/*          JOBZ = 'V', then LDZ >= max(1,N). */
integer ldz = 1;

/*  ISUPPZ  (output) INTEGER ARRAY, dimension ( 2*max(1,M) ) */
/*          The support of the eigenvectors in Z, i.e., the indices */
/*          indicating the nonzero elements in Z. The i-th computed eigenvector */
/*          is nonzero only in elements ISUPPZ( 2*i-1 ) through */
/*          ISUPPZ( 2*i ). This is relevant in the case when the matrix */
/*          is split. ISUPPZ is only accessed when JOBZ is 'V' and N > 0. */
integer *isuppz= NULL;

/*  WORK    (workspace/output) REAL array, dimension (LWORK) */
/*          On exit, if INFO = 0, WORK(1) returns the optimal */
/*          (and minimal) LWORK. */
real *work= (real*)malloc(sizeof(real)*12*n);

/*  LWORK   (input) INTEGER */
/*          The dimension of the array WORK. LWORK >= max(1,18*N) */
/*          if JOBZ = 'V', and LWORK >= max(1,12*N) if JOBZ = 'N'. */
/*          If LWORK = -1, then a workspace query is assumed; the routine */
/*          only calculates the optimal size of the WORK array, returns */
/*          this value as the first entry of the WORK array, and no error */
/*          message related to LWORK is issued by XERBLA. */
integer lwork= 12*n;

/*  IWORK   (workspace/output) INTEGER array, dimension (LIWORK) */
/*          On exit, if INFO = 0, IWORK(1) returns the optimal LIWORK. */
integer *iwork = (integer*)malloc(sizeof(integer)*8*n);

/*  LIWORK  (input) INTEGER */
/*          The dimension of the array IWORK.  LIWORK >= max(1,10*N) */
/*          if the eigenvectors are desired, and LIWORK >= max(1,8*N) */
/*          if only the eigenvalues are to be computed. */
/*          If LIWORK = -1, then a workspace query is assumed; the */
/*          routine only calculates the optimal size of the IWORK array, */
/*          returns this value as the first entry of the IWORK array, and */
/*          no error message related to LIWORK is issued by XERBLA. */
integer liwork= 8*n;

/*  INFO    (output) INTEGER */
/*          On exit, INFO */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value */
/*          > 0:  if INFO = 1X, internal error in SLARRE, */
/*                if INFO = 2X, internal error in SLARRV. */
/*                Here, the digit X = ABS( IINFO ) < 10, where IINFO is */
/*                the nonzero error code returned by SLARRE or */
/*                SLARRV, respectively. */
integer info = 0;

/*call the CLAPACK MRRR routine*/
sstegr_(&jobz, &range, &n_prime, d, 
	e, vl, vu, &il, &iu, abstol, 
	&m, w, z, &ldz, isuppz, work, &lwork, iwork, &liwork, &info);

/*copy back into the eigvalues array*/
convert_real_to_double_array(w,eigvalues,n);

/*free the memory*/
free(iwork);
free(work);
free(e);
free(d);

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
 


/**
 * void convert_double_to_real_array(double* double_array, real* real_array, int n)
 * 
 * @brief Utility function to convert between IntelliPi double standard and CLAPACK real
 * @param double_array, (real) array to be converted
 * @param real_array, (double) converted array
 * @param n, number of elements to be converted
 */ 
void convert_double_to_real_array(double* double_array, real* real_array, int n)
{ 
	int i=0;
	
	for(i=0;i<n;i++)
	{
		real_array[i] = (real)double_array[i];
	}
}


/**
 * void convert_real_to_double_array(real* real_array, double* double_array, int n)
 * 
 * @brief Utility function to convert between CLAPACK real and IntelliPi double standard 
 * @param real_array, (real) array to be converted
 * @param double_array, (double) converted array
 * @param n, number of elements to be converted
 */ 
void convert_real_to_double_array(real* real_array, double* double_array, int n)
{ 
	int i=0;
	
	for(i=0;i<n;i++)
	{
		double_array[i] = (double)real_array[i];
	}
}


