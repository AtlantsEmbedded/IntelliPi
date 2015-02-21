/**
 * @file svm.cpp
 * @author Frédéric Simard
 * @date February, 2015
 * @brief This file implements a test bench for my TinySVM wrapper. 
 * 		  It initialize a dataset, train the svm, and classify a series of samples.
 *        It then make sure that the svm can be saved and loaded properly. A few things
 *        are hardcoded, but overall it can be scaled up easily. 
 * 
 *        Feel free to use, modify and distribute, but please respect authorship
 *        Atom Embedded, 2015.
 */

#include "svm.h"
#include <time.h>

using namespace std;

void randsample(int array[], int n);

int main(){
	
	
	/* ------------------------------------------------------------------
	 * Define a small dataset: data and labels
	 * - 100 samples per class and 2 classes.
	 * - Samples have 10 features 
	 *     * one of them is correlated with the class id 
	 *     * a second one is anti-correlated.
	 * ------------------------------------------------------------------
	 */
	 
	int i,j;
	
	int tot_nb_samples = 200;
	int nb_train_samples = 180;
	int nb_test_samples = 20;
	int half_nb_samples = tot_nb_samples/2;
	
	int nb_of_features = 10;
	
	double **samples = (double**)malloc(sizeof(double*)*tot_nb_samples);
	int labels[tot_nb_samples];
	int labels_idx[tot_nb_samples]; // used to mix the samples
	double prdcted_labels[nb_test_samples]; // predicted labels (out of svm)
	
	int temp_a_id;
	int temp_b_id;
	
	/*the svms*/
	t_svm_options options;
	t_svm* psvm;
	t_svm* psvm2; //used for save/load test

	/*allocate feature memory for the samples*/
	for(i=0;i<tot_nb_samples;i++){
		samples[i] = (double*)malloc(sizeof(double)*nb_of_features);
	}
	
	/*
	 * Mix the samples
	 */
	 
	 /*Get a set of rand sample (random without duplication)*/
	randsample(labels_idx,tot_nb_samples);

	/*produce the feature vectors*/
	for(i=0;i<half_nb_samples;i++){
		temp_a_id = labels_idx[i];
		temp_b_id = labels_idx[i+half_nb_samples];
		labels[temp_a_id] = 1;
		labels[temp_b_id] = -1;
		
		/*baseline is independent*/
		for(j=0;j<10;j++){
			samples[temp_a_id][j] = (double)rand()/((double)RAND_MAX);
			samples[temp_b_id][j] = (double)rand()/((double)RAND_MAX);
		}
		
		/*both classes are anti-correlated*/
		
		/*class one*/
		samples[temp_a_id][3] = samples[temp_a_id][3]+2*labels[temp_a_id];
		samples[temp_a_id][7] = samples[temp_a_id][7]-2*labels[temp_a_id];
		
		/*class two*/
		samples[temp_b_id][3] = samples[temp_b_id][3]+2*labels[temp_b_id];
		samples[temp_b_id][7] = samples[temp_b_id][7]-2*labels[temp_b_id];	
	}
	
	/*init svm option*/
	options.nb_samples = nb_train_samples;
	options.nb_features = nb_of_features;
	
	/*train the svm*/
	psvm = train_svm(samples, labels, options);
	
	/*classify the test samples and output results*/
	for(i=0;i<nb_test_samples;i++){
		prdcted_labels[i] = classify_with_svm(psvm, samples[nb_train_samples+i]);
		
		cout <<  labels[nb_train_samples+i] << "   " << prdcted_labels[i] << endl;
	}
	
	cout << "save/load and test svm again" << endl;
	
	/*save and load the svm*/
	save_svm(psvm);
	psvm2 = load_svm();
	
	/*classify the test samples and output results again, but this time with the loaded svm*/
	for(i=0;i<nb_test_samples;i++){
		prdcted_labels[i] = classify_with_svm(psvm2, samples[nb_train_samples+i]);
		
		cout <<  labels[nb_train_samples+i] << "   " << prdcted_labels[i] << endl;
	}	

	return EXIT_SUCCESS;
	
}


/**
 * randsample(int array[], int n)
 * 
 * @brief return an array of n elements. The array is the series of numbers [0, n-1] 
 *        randomly permutated a large number of times (n*10).
 *        This function will shortly be moce to a utils lib.
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
