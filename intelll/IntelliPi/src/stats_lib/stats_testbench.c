/**
 * @file stats_testbench.c
 * @author Billy Labelle, Atlans embedded
 * @date July, 2015
 * @brief This file validates the functions of the stats library.
 *        Empirical test values were validated using Matlab.
 * 
 *        This file also serves as an examples file for anyone interested
 *        in making use of our stats library.
 *
 */

#include "stats.h"
void show_matrix(double *A, int dim_i, int dim_j);
void test_stats();
void test_randn();
void write_data(double *mtx,int dim_i,int dim_j);
int main(void) {

//test_stats();
test_randn();
    return 0;
}
/**
 * void test_stats()
 * @brief Utility function to test the functions related to basic operations on matrixes
*/
void test_stats(){
	
	int DIM_I = 6;
	int DIM_J=6;
	double a[36]= {	3.5712, 1.4007, 2.5214,35.7680,12.5698,34.5678,
					1.4007, 84.9129, 2.7030, 64.5638,4.5645,56.4523,
					2.5214, 2.7030, 93.3993,32.4563,56.4322,24.4678,
					35.7680,64.5638,32.4563,43.2345,21.3456,32.5476,
					12.5698,4.5645,56.4322,21.3456,78.4356,65.4356,
					34.5678,56.4523,24.4678,32.5476,65.4356,21.4567
				  };
				
	double *mean = (double*)calloc(sizeof(double),DIM_J);
	double *b = (double*)calloc(sizeof(double),(DIM_J*DIM_I));
	
	
	//Shows the original matrix
	show_matrix(a,DIM_I,DIM_J);
	
	
	//Calculates the mean of the columns of the original matrix
	stat_mean(a,mean,DIM_I,DIM_J);
	
	//Shows the expected mean matrix
	printf("\nexpected mean matrix \n"
	"15.06650 35.76620 35.33000 38.31930 39.79720 39.15460");
	
	//Shows the mean matrix calculated
	printf("\n\nMean matrix \n");
	show_matrix(mean,1,DIM_J);
	
	//Shows the expected matrix with the mean removed
	printf("\nExpected matrix with the mean removed\n"
	"-11.49528 -34.36550 -32.80860 -2.55130 -27.22742 -4.58683\n"
	"-13.66578 49.14670 -32.62700 26.24450 -35.23272 17.29767\n"
	"-12.54508 -33.06320 58.06930 -5.86300 16.63498 -14.68683\n"
	"20.70152 28.79760 -2.87370 4.91520 -18.45162 -6.60703\n"
	"-2.49668 -31.20170 21.10220 -16.97370 38.63838 26.28097\n"
	"19.50132 20.68610 -10.86220 -5.77170 25.63838 -17.69793\n");
	
	//Calculates the matrix with the mean removed
	remove_mean_col(b,a,mean,DIM_I,DIM_J);
	
	//Shows the mean with the matrix removed	
	printf("\n Derivation from the mean matrix \n");
	show_matrix(b,DIM_I,DIM_J);
}
/**
 * void randn_mtx(double *mtx, int dim_i, int dim_j)
 * @brief Utility function to test the operations 
 * related to the generation of random normally
 * distributed numbers and to write these values
 * in a text file
**/
void test_randn(){
	int DIM_I = 900;
	int DIM_J=900;
	double sample;
	double *mtx;
	double *mean;
	int i,j;
	mtx = (double*)calloc(DIM_I*DIM_J,sizeof(double));
	mean = (double*)calloc(DIM_J,sizeof(double));

	/*Test to generated a normally distributed number*/
	sample = randn();
	printf("\n\n%lf\n",sample);
	
	/*Test to generate a matrix of normally distributed 
	 * random numbers*/
	randn_mtx(mtx,DIM_I,DIM_J);
	show_matrix(mtx,DIM_I,DIM_J);
	
	/*Test to modify the mean and the standard
	 * deviation of the matrix above*/
	modify_mean_stddev(mtx,5.0,2.0,DIM_I,DIM_J);
		
	printf("\n\n");
	show_matrix(mtx,DIM_I,DIM_J);
	
	/*Test to check if the values are written
	 * in the text file*/
	write_data(mtx,DIM_I,DIM_J);
	
	/*Test to see if the mean has been correctly
	 * modified*/
	stat_mean(mtx,mean,DIM_I,DIM_J);
	show_matrix(mean,1,DIM_J);
}
/**
 * void write_data(double *mtx, int dim_i, int dim_j)
 * @brief Utility function to write the values of the matrix
 * in a text file.
 * @param mtx, IxJ double array, the matrix containing the values
 * @param dim_i, the value of I.
 * @param dim_j, the value of J.
 */ 
void write_data(double *mtx,int dim_i,int dim_j){
	FILE *f_in;
	int i,j;
	f_in=fopen("data_normally_distributed.txt","w");
	
	for (i=0;i<dim_i;i++){
		for (j=0;j<dim_j;j++){
			fprintf(f_in,"%lf ",mtx[i*dim_i+j]);
			
		}
		fprintf(f_in,"\n","");
	}
		
	
}
