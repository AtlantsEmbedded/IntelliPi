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
int main() {

test_stats();
    
    return 0;
}

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

