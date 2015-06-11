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

void test_basic_vector_op(void);
void test_basic_mtx_op(void);
void test_linear_solver(void);
void test_eigen_solver(void);
void test_pca(void);
int main() {


	//test_basic_vector_op();
	
	//test_basic_mtx_op();
    
	//test_linear_solver();

    //test_eigen_solver();
    test_pca();
    
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

void test_linear_solver(void)
{
	int i;
	int dim_i = 3;
	int dim_j = 3;
	
	/*Definition of the variables of problem 1: lower triangular matrix linear system solver*/
	double A_1[9] = {0.6790, 0.0000, 0.0000,
					 0.7580, 0.6550, 0.0000,
					 0.7430, 0.1710, 0.2770};
	
	double x_1[3] = {0.0460,
					 0.0970,
					 0.8230};
    
	double b_1[3] = {0.031234,
					 0.098403,
					 0.278736};
					 
					 
	/*Definition of the variables of problem 2: upper triangular matrix linear system solver*/
	double A_2[9] = {0.6950, 0.0340, 0.7660,
					 0.0000, 0.4390, 0.7950,
					 0.0000, 0.0000, 0.1870};
	
	double x_2[3] = {0.4900,
					 0.4460,
					 0.6460};
    
	double b_2[3] = {0.850550,
					 0.709364,
					 0.120802};				 
					 
					 
	/*Definition of the variables of problem 3: symmetric matrix linear system solver*/
	double A_3[9] = {0.7510, 0.1400, 0.1300,
					 0.1400, 0.2550, 0.2140,
					 0.1300, 0.2140, 0.5060};
	
	double x_3[3] = {0.8140,
					 0.2440,
					 0.9290};
    
	double b_3[3] = {0.766244,
					 0.374986,
					 0.628110};				 
	
    /*------------*/
    /* TEST 1     */
    /*------------*/
    printf("Test 1: lower triangular matrix linear equation solver\n");
    printf("Expected: ");
	for(i=0;i<dim_i;i++){
		printf("%.4f\t",x_1[i]);
	}
	printf("\n");
	
	/*Test the lower triangular matrix linear equation solver*/
	lin_solve_triangular_equ(A_1, x_1, b_1, dim_i, dim_j, 1);
	
	printf("Obtained: ");
	for(i=0;i<dim_i;i++){
		printf("%.4f\t",x_1[i]);
	}
	printf("\n");
	
    /*------------*/
    /* TEST 2     */
    /*------------*/
    printf("Test 2: upper triangular matrix linear equation solver\n");
    printf("Expected: ");
	for(i=0;i<dim_i;i++){
		printf("%.4f\t",x_2[i]);
	}
	printf("\n");
	
	/*Test the lower triangular matrix linear equation solver*/
	lin_solve_triangular_equ(A_2, x_2, b_2, dim_i, dim_j, 0);
	
	printf("Obtained: ");
	for(i=0;i<dim_i;i++){
		printf("%.4f\t",x_2[i]);
	}
	printf("\n");

    /*------------*/
    /* TEST 3     */
    /*------------*/
    printf("Test 3: Symmetric matrix linear equation solver\n");
    printf("Expected: ");
	for(i=0;i<dim_i;i++){
		printf("%.4f\t",x_3[i]);
	}
	printf("\n");
	
	/*Test the lower triangular matrix linear equation solver*/
	lin_solve_PSD(A_3, x_3, b_3, dim_i, dim_j);
	
	printf("Obtained: ");
	for(i=0;i<dim_i;i++){
		printf("%.4f\t",x_3[i]);
	}
	printf("\n");

}


void test_eigen_solver(void)
{
	int i = 0;
	int n = 3;
	int m = 3;

	double matrix_D[9] = {3.5712, 1.4007, 2.5214,
					      1.4007, 84.9129, 2.7030,
					      2.5214, 2.7030, 93.3993};
					      
	double lancz_trans_mtx[9];
	
	double a[3];
	double b[3];
	double eigvalues[3];
									  
	double exp_lancz_trans_mtx[9] = {65.0446, 40.8835, 0,
									 40.8835, 31.2460, 7.1235,
									 0, 7.1235, 85.5928};

	/*test lanczos procedure*/
	printf("Tm = lancz(D)'\n");
	printf("Matrix D'\n");	
	show_matrix(matrix_D, n, n);

	mtx_lanczos_procedure(matrix_D, a, b, n, m);
	
	
	/*construct tridiagonal matrix tm*/
	lancz_trans_mtx[0] = a[0];
	for(i=1;i<=m-1;i++){
		lancz_trans_mtx[i*m+i] = a[i]; 
		lancz_trans_mtx[i*m+i-m] = b[i-1]; 
		lancz_trans_mtx[i*m+(i-1)] = b[i-1]; 
	}
	
	printf("\n");
	printf("Expected Lanczos'\n");	
	show_matrix(exp_lancz_trans_mtx, m, m);
	printf("Obtained Lanczos'\n");	
	show_matrix(lancz_trans_mtx, m, m);
	
	/*call the MRRR routine*/
	mtx_mrrr(a, b, eigvalues, n);
	
	printf("\n");
	printf("expected eigvalues: 3.4789 84.1301 94.2744\n");	
	printf("obtained eigvalues: ");	
	
	for(i=0;i<3;i++){
		printf("%.4f ",eigvalues[i]);	
	}
	printf("\n\n");
	
}

void test_pca(void){
	
	
	int DIM_I = 6;
	int DIM_J=6;
	int i;
	double a[36]= {	3.5712, 1.4007, 2.5214,35.7680,12.5698,34.5678,
					1.4007, 84.9129, 2.7030, 64.5638,4.5645,56.4523,
					2.5214, 2.7030, 93.3993,32.4563,56.4322,24.4678,
					35.7680,64.5638,32.4563,43.2345,21.3456,32.5476,
					12.5698,4.5645,56.4322,21.3456,78.4356,65.4356,
					34.5678,56.4523,24.4678,32.5476,65.4356,21.4567
					};
	double lancz_trans_mtx[36];				
	double *mean = (double*)calloc(sizeof(double),DIM_J);
	double *b = (double*)calloc(sizeof(double),(DIM_J*DIM_I));
	double *b_prime = (double*)calloc(sizeof(double),(DIM_J*DIM_I));
	double *cov= (double*)calloc(sizeof(double),(DIM_J*DIM_I));
	double *eigenvalues=(double*)calloc(sizeof(double),(DIM_J));
	double *Identity = (double*)calloc(sizeof(double),(DIM_J*DIM_I));
	double *mtx_diag= (double*)calloc(sizeof(double),(DIM_J*DIM_I));
	double *mtx_off_diag= (double*)calloc(sizeof(double),(DIM_J*DIM_I));
	double *ident_eigen= (double*)calloc(sizeof(double),(DIM_J*DIM_I));
	show_matrix(a,DIM_I,DIM_J);
	
	
		
	stat_mean(a,mean,DIM_I,DIM_J);
	
	printf("\nexpected mean matrix \n"
	"15.06650 35.76620 35.33000 38.31930 39.79720 39.15460");
	printf("\n\nMean matrix \n");
	
	show_matrix(mean,1,DIM_J);
	printf("\nexpected derivation from the mean matrix\n"
	"-11.49528 -34.36550 -32.80860 -2.55130 -27.22742 -4.58683\n"
	"-13.66578 49.14670 -32.62700 26.24450 -35.23272 17.29767\n"
	"-12.54508 -33.06320 58.06930 -5.86300 16.63498 -14.68683\n"
	"20.70152 28.79760 -2.87370 4.91520 -18.45162 -6.60703\n"
	"-2.49668 -31.20170 21.10220 -16.97370 38.63838 26.28097\n"
	"19.50132 20.68610 -10.86220 -5.77170 25.63838 -17.69793\n");
			
	printf("\n Derivation from the mean matrix \n");
	
	mtx_deriv_mean(b,a,mean,DIM_I,DIM_J);
	show_matrix(b,DIM_I,DIM_J);
	printf("\n");

	printf("Expected transpose matrix \n""-11.49528 -13.66578 -12.54508 20.70152 -2.49668 19.50132\n" 
"-34.36550 49.14670 -33.06320 28.79760 -31.20170 20.68610\n" 
"-32.80860 -32.62700 58.06930 -2.87370 21.10220 -10.86220\n" 
"-2.55130 26.24450 -5.86300 4.91520 -16.97370 -5.77170\n" 
"-27.22742 -35.23272 16.63498 -18.45162 38.63838 25.63838\n" 
"-4.58683 17.29767 -14.68683 -6.60703 26.28097 -17.69793\n"); 
	printf("\n\n Transpose matrix \n");
	
	mtx_transpose(b,b_prime,DIM_I,DIM_J);
	show_matrix(b_prime,DIM_J,DIM_I);
	
	printf("\nExpected multiplication prior to covariance matrix (b*b')\n"
	"3158.41375 351.58407 -995.33304 -613.17658 -720.63631 -1180.85189\n" 
"351.58407 5896.00026 -4342.15094 1890.97606 -3540.04502 -256.36445\n" 
"-995.33304 -4342.15094 5149.39843 -1617.44056 2644.62062 -839.09450\n" 
"-613.17658 1890.97606 -1617.44056 1674.38695 -1980.86959 646.12372\n"
"-720.63631 -3540.04502 2644.62062 -1980.86959 3896.80272 -299.87242\n" 
"-1180.85189 -256.36445 -839.09450 646.12372 -299.87242 1930.05954\n");
	
	printf("\n\n Multiplication prior to covariance matrix (b*b')\n");
	
	mtx_mult(b,b_prime,cov,DIM_I,DIM_J,DIM_I);
	show_matrix(cov,DIM_I,DIM_I);
	
	for (i=0;i<DIM_I*DIM_I;i++)
		cov[i] /= DIM_I;
	
	printf("\nExpected covarianc matrix\n"
	"526.40229 58.59735 -165.88884 -102.19610 -120.10605 -196.80865\n" 
"58.59735 982.66671 -723.69182 315.16268 -590.00750 -42.72741\n" 
"-165.88884 -723.69182 858.23307 -269.57343 440.77010 -139.84908\n" 
"-102.19610 315.16268 -269.57343 279.06449 -330.14493 107.68729\n" 
"-120.10605 -590.00750 440.77010 -330.14493 649.46712 -49.97874\n" 
"-196.80865 -42.72741 -139.84908 107.68729 -49.97874 321.67659\n");

	printf("\nCovariance matrix\n");
	
	show_matrix(cov,DIM_I,DIM_I);

	//a[0]*= 1000;	
	
	//mtx_lanczos_procedure(a,mtx_diag, mtx_off_diag,DIM_I,6);
	

	//show_matrix(Identity,DIM_I,DIM_I);
	//printf("\n Show the eigenvalues \n");
	//mtx_mrrr(mtx_diag, mtx_off_diag,eigenvalues,6);
	//show_matrix(eigenvalues,1,6);
	
	//mtx_lanczos_procedure(a,mtx_diag, mtx_off_diag,DIM_I,5);
	

	//show_matrix(Identity,DIM_I,DIM_I);
	//printf("\n Show the eigenvalues \n");
	//mtx_mrrr(mtx_diag, mtx_off_diag,eigenvalues,6);
	//show_matrix(eigenvalues,1,6);
	

	
	//mtx_lanczos_procedure(a,mtx_diag, mtx_off_diag,DIM_I,3);
	

	//show_matrix(Identity,DIM_I,DIM_I);
	//printf("\n Show the eigenvalues \n");
	//mtx_mrrr(mtx_diag, mtx_off_diag,eigenvalues,3);
	//show_matrix(eigenvalues,1,3);
	
	
	//	printf("\nElements of the diagonal\n");
		
	//show_matrix(mtx_diag,1,DIM_J);
	
		
	//	printf("\nElements off the diagonal\n");
	//show_matrix(mtx_off_diag,1,DIM_J);	
			//printf("\nIdentity matrix \n");
	
	//mtx_ident(Identity,DIM_I);
	
	//mtx_mult(eigenvalues,Identity,ident_eigen,1,3,3);
	//printf("\n Show the multiplication of eigen values with the matrix identity \n");
	//show_matrix(ident_eigen,3,3);
	
	//vect_sub(a,ident_eigen,(DIM_I*DIM_J));
	//printf("\n Show the substraction of a with the matrix identity* eigenvalues \n");
	//show_matrix(a,3,3);
	
	
	}

