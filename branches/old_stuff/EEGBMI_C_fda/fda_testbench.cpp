

#include "fda.h"


using namespace std;
using namespace Eigen;


void randsample(int array[], int n);

/*temporary, before building as a shared object*/
/*will implement a test bench*/
int main(){
	
	
	/*define a small dataset: data and labels*/
	// create a dataset, with 100 samples per class and 2 classes.
	// Samples have 10 features, one of them is correlated with the class id and
	// a second one is anti-correlated.
	
	int i,j;
	double **samples;
	double *fdaed_samples;
	int labels[200];
	int labels_idx[200];
	int sample_a_id;
	int sample_b_id;
	t_fda_options options;
	t_fda *pfda;
	
	samples = (double**)malloc(sizeof(double*)*200);
	
	for(i=0;i<200;i++){
		samples[i] = (double*)malloc(sizeof(double)*10);
	}
	
	fdaed_samples = (double*)malloc(sizeof(double)*200);
	
	randsample(labels_idx,200);

	for(i=0;i<100;i++){
		sample_a_id = labels_idx[i];
		sample_b_id = labels_idx[i+100];
		labels[sample_a_id] = 1;
		labels[sample_b_id] = -1;
		
		for(j=0;j<10;j++){
			samples[sample_a_id][j] = (double)rand()/((double)RAND_MAX);
			samples[sample_b_id][j] = (double)rand()/((double)RAND_MAX);
		}
		
		samples[sample_a_id][3] = samples[sample_a_id][3]+2*labels[sample_a_id];
		samples[sample_a_id][7] = samples[sample_a_id][7]-2*labels[sample_a_id];
		
		samples[sample_b_id][3] = samples[sample_b_id][3]+2*labels[sample_b_id];
		samples[sample_b_id][7] = samples[sample_b_id][7]-2*labels[sample_b_id];	
	}
	
	/*compute the fda*/
	options.nb_classes = 2;
	options.nb_features = 10;
	pfda = init_fda(samples, labels, 200, options);
	
	transform_data(pfda, samples, fdaed_samples, 200);

	//show the results
	for(i=0;i<200;i++){
		cout << fdaed_samples[i] << " " <<  labels[i] << endl;
	}
	
	kill_fda(pfda);

	return 1;
}

/**
 * randsample(int array[], int n)
 * 
 * @brief (will be move to a util lib soon) return an array of n elements. The array is the series of numbers [0, n-1] randomly permutated a large number of times (n*10)
 * @param n, size of the array and range of the indexes
 * @param (out), array of size n, values will be set in the fonction
 */
void randsample(int array[], int n){
	
	int i;
	int nb_permut = n*10;
	int temp, source, dest; 
	time_t t;
	
	/*seed random generator*/
	srand((unsigned) time(&t));
	
	/*init the array with continuous numbers*/
	for(i=0;i<n;i++){
		array[i] = i;
	}
	
	/*do many permutations*/
	for(i=0;i<nb_permut;i++){
		
		/*pick two samples at random*/
		source = rand() % n;
		dest = rand() % n;
		
		/*permute them*/
		temp = array[source];
		array[source] = array[dest];
		array[dest] = temp;
	}
}
