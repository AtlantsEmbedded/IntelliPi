/**
 * @file stats.c
 * @author Billy Labelle, Atlans embedded
 * @date July, 2015
 * @brief This library contains various utility functions to analyse data through statistics
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
 *    Linear system solver, based on cholesky decomposition
 *    - http://www.seas.ucla.edu/~vandenbe/103/lectures/chol.pdf
 *    
 *    Lanczos Procedure:
 *    - Cullum J and Willoughby (1981) Computing Eigenvalues of Very Large Symmetric Matrices - 
 *      An Implementation of Lanczos Algorithm with no Reorthogonalization. Computational Physics. 
 *      (44)329-358.
 *    - http://en.wikipedia.org/wiki/Lanczos_algorithm
 *
 *    MRRR for eigenvalues
 *    - Dhillon IS, Beresford PN and Vomel C (2006) The Design and Implementation of the MRRR Algorithm. 
 *      ACM Transactions on Mathematical Software, 32(4):533-560.
 *
 *
 
 
 
 
 * void mtx_deriv_mean(double *b, double *a, double *mean, int dim_i, int dim_j)
 * @brief Function to calculate the matrix of derivation from the mean
 * @param double *b , the resulting derivation from the mean matrix
 * @param double *a , the matrix contraining the original elements
 * @param double *mean, the matrix containing the mean of the columns of *a
 * @param int dim_i, the number of rows in the matrix
 * @param int dim_j, the number of colomns in the matrix
 */ 
 #include "stats.h"
 
void remove_mean_col(double *b, double *a, double *mean, int dim_i, int dim_j){
	 
	 int i,j;
	 for (i=0;i<dim_i;i++){
        for (j=0;j<dim_j;j++){
            b[i*dim_j+j] = a[i*dim_j+j]-mean[j];
            
	}
	 
	 
	 
		}
}





/**
 * void stat_mean(double *a, double *mean, int dim_i, int dim_j)
 * @brief Function to calculate the mean matrix
 * @param double *a , the matrix containing the original matrix
 * @param double *mean , the matrix contraining the mean of each column of *a
 * @param int dim_i, the number of rows in the matrix
 * @param int dim_j, the number of colomns in the matrix
 */
void stat_mean(double *a, double *mean, int dim_i, int dim_j){
   int i,j,n;
   
   
  //printf("\n\n%f\n\n",mean[2]); 
  
   n=0;
/*Sum over each column*/
for(i=0;i<dim_i;i++){
     n = i*dim_j;
     for (j=0;j<dim_j;j++){
            mean[j] += a[j+n];
     }
 }


  //printf("\n\n%f\n\n",mean[2]); 

/*Divide to get average*/
for (j=0;j<dim_j;j++){
    mean[j] /= dim_i;
   
}
  //printf("\n\n%f\n\n",mean[2]); 
  
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
 * double randn()
 * @brief Utility function to generate a normally distributed number
 */ 
double randn(){
	double Twopi = (6.2831853071795864769252867665590057683943387987502); /* 2 * pi */
	double randmax = RAND_MAX;
	double random = rand();
	double randnorm;
	random/=randmax;
	 randnorm= (sqrt(-2.0*log(random)));
	randnorm*=cos(Twopi*random);
	
	
	return randnorm;
}
/**
 * void randn_mtx(double *mtx, int dim_i, int dim_j)
 * 
 * @brief Utility function to generate a matrix containing normally 
 * distributed numbers
 * @param mtx, IxJ double array, the matrix containing the normally
 * distributed numbers
 * @param dim_i, the value of I.
 * @param dim_j, the value of J.
 */ 
void randn_mtx(double *mtx, int dim_i, int dim_j){
	int i,j;
	for (i=0;i<dim_i;i++){
		for (j=0;j<dim_j;j++){
			mtx[i*dim_j+j]=randn();
			
			}
		
		}
	}

void modify_mean_stddev(double *mtx,double new_mean,double new_stddev,int dim_i, int dim_j){
	int i,j;
	for (i=0;i<dim_i;i++){
		for (j=0;j<dim_j;j++){
			mtx[i*dim_i+j]= (mtx[i*dim_j+j]*new_stddev)+new_mean;
			
			}
		}
	
	}	
