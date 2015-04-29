/**
 * @file lin_alg_testbench.c
 * @author Frédéric Simard, Atlans embedded
 * @date March, 2015
 * @brief This file validates the functions of the linear algebra library.
 *        Empirical test values were validated using Matlab.
 * 
 *        This file also serves as an examples file for anyone interested
 *        in making use of our linear algebra library.
 *
 */

#include "linear_algebra.h"

void test_vector_op(void);
void test_mtx_op(void);
void test_eigen_solver(void);

int main() {


	//test_basic_vector_op();
	
	//test_basic_mtx_op();
    
    test_eigen_solver();
    
    
    return 0;
}



void test_basic_vector_op(void)
{
	
	/******************************/
	/* Test the vector operations */
	/******************************/

	/*create two vectors and show them*/
	int vector_length = 5;
	double vector_a[5] = {2.5, -10.9, 15.8, 12.2, 7.9};
	double vector_b[5] = {2.5, 0.2, 21.33, 70.1, -0.2};
	double vector_c[5];
	double result = 0.0;
	
	/*seed the random generator*/
	srand(time(NULL));
	
	printf("The two vectors a and b\n");
	show_vector(vector_a, vector_length);
	show_vector(vector_b, vector_length);

	/*vector addition*/
	printf("c = a + b\n");
	/*copy a*/
	memcpy(vector_c, vector_a, vector_length*sizeof(double));
	/*compute*/
	vect_add(vector_c, vector_b, vector_length);
	/*show expected and obtained results*/
	printf("Expected: 5.0000 -10.7000 37.1300 82.3000 7.7000\n");
	printf("Result: ");
	show_vector(vector_c, vector_length);

	/*vector subtraction*/
	printf("c = a - b\n");
	/*copy a*/
	memcpy(vector_c, vector_a, vector_length*sizeof(double));
	/*compute*/
	vect_sub(vector_c, vector_b, vector_length);
	/*show expected and obtained results*/
	printf("Expected: 0.0000 -11.1000 -5.5300 -57.9000 8.1000\n");
	printf("Result: ");
	show_vector(vector_c, vector_length);

	/*vector scalar multiplication*/
	printf("c = a * 5\n");
	/*copy a*/
	memcpy(vector_c, vector_a, vector_length*sizeof(double));
	/*compute*/
	vect_scalar_multiply(vector_c, 5, vector_length);
	/*show expected and obtained results*/
	printf("Expected: 12.5000 -54.5000 79.0000 61.0000 39.5000\n");
	printf("Result: ");
	show_vector(vector_c, vector_length);

	/*vector dot product*/
	printf("c = a .* b\n");
	/*compute*/
	result = vect_dot_product(vector_a, vector_b, vector_length);
	/*show expected and obtained results*/
	printf("Expected: 1194.72\n");
	printf("Result: %2.2f\n",result);

	/*vector cross product*/
	/*To be completed*/

	/*vector norm*/
	printf("c = |a|\n");
	/*compute*/
	result = vect_norm(vector_a, vector_length);
	/*show expected and obtained results*/
	printf("Expected: 24.2064\n");
	printf("Result: %2.4f\n",result);

	/*vector rand unit*/
	printf("c is a random vector\n");
	printf("|c| == 1\n");
	/*compute*/
	vect_rand_unit(vector_c, vector_length);
	/*show expected and obtained results*/
	printf("The vector: ");
	show_vector(vector_c, vector_length);
	result = vect_norm(vector_c, vector_length);
	printf("Expected norm: 1.0000\n");
    printf("Obtained norm: %2.4f\n",result);
	
}

void test_basic_mtx_op(void)
{
	
	/******************************/
	/* Test the matrix operations */
	/******************************/
	int dim_i = 3;
	int dim_j = 3;
	int dim_k = 4;
	double matrix_A[9] = {0.9572, 0.1419, 0.7922,
						  0.4854, 0.4218, 0.9595,
						  0.8003, 0.9157, 0.6557};
	double matrix_B[12] = {0.8147, 0.9134, 0.2785, 0.9649,
						   0.9058, 0.6324, 0.5469, 0.1576,
						   0.1270, 0.0975, 0.9575, 0.9706};

	double matrix_D[9] = {3.5712, 1.4007, 2.5214,
					      1.4007, 84.9129, 2.7030,
					      2.5214, 2.7030, 93.3993};						   
						   
	double matrix_C[12];
	double matrix_B_prime[12];
	double matrix_L[9];
	double matrix_L_prime[9];
	double result = 0.0;	

	printf("Show Matrix A\n");	
	show_matrix(matrix_A, dim_i, dim_j);	
	printf("Show Matrix B\n");	
	show_matrix(matrix_B, dim_j, dim_k);	

	/*test for matrix multiplication*/
	printf("C = A * B\n");	
	mtx_mult(matrix_A, matrix_B, matrix_C, dim_i, dim_j, dim_k);
	show_matrix(matrix_C, dim_i, dim_k);	
	
	/*test for matrix transpose*/
	printf("B' = B\n");
	mtx_transpose(matrix_B, matrix_B_prime, dim_j, dim_k);	
	show_matrix(matrix_B_prime, dim_k, dim_j);	

	/*test for cholesky decomposition*/
	printf("Show Symmetric Matrix D\n");
	show_matrix(matrix_D, dim_i, dim_j);	
	
	printf("L = chol(D)\n");	
	mtx_chol(matrix_D, matrix_L, dim_i);
	show_matrix(matrix_L, dim_i, dim_i);
	
	printf("D = L*L'\n");	
	mtx_transpose(matrix_L, matrix_L_prime, dim_i, dim_i);	
	mtx_mult(matrix_L, matrix_L_prime, matrix_D, dim_i, dim_i, dim_i);
	show_matrix(matrix_D, dim_i, dim_i);

	/*test validation tag*/
	printf("Matrix test results validated using Matlab: \n");
	printf("All good! 05/15, FS\n");	
	
}


void test_eigen_solver(void)
{
	int n = 3;
	int m = 3;

	double matrix_D[9] = {3.5712, 1.4007, 2.5214,
					      1.4007, 84.9129, 2.7030,
					      2.5214, 2.7030, 93.3993};
					      
	double lancz_trans_mtx[9];
									  
	double exp_lancz_trans_mtx[9] = {65.0446, 40.8835, 0,
									 40.8835, 31.2460, 7.1235,
									 0, 7.1235, 85.5928};

	/*test lanczos procedure*/
	printf("Tm = lancz(D)'\n");
	printf("Matrix D'\n");	
	show_matrix(matrix_D, n, n);

	mtx_lanczos_procedure(matrix_D, lancz_trans_mtx, n, m);
	
	printf("Expected Lanczos'\n");	
	show_matrix(exp_lancz_trans_mtx, m, m);
	printf("Obtained Lanczos'\n");	
	show_matrix(lancz_trans_mtx, m, m);
}
