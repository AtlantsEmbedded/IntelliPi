#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Cholesky>

#include <Eigen/Cholesky>


#include "common.h"
#include "misc.h"
#include "model.h"
#include "example.h"
#include "base_example.h"
#include "dataset.h"

#include "svm.h"
#include "fft.h"
#include "fda.h"

using namespace std;
//using namespace Eigen;


//int eegbmi_train(double** train_dataset, int nb_train_samples,TinySVM::Model *model);
//int eegbmi_train(double** train_dataset, int nb_train_samples);
//int eegbmi_classify();

const char* DATASET_PATH = "ascii_files_3/";

int main()
{
	int i,j;
	t_dataset *pdataset;
	double** train_samples;
	int* train_labels;
	
	double* fdaed_samples;
	
	t_fda_options fda_options;
	t_fda *pfda;

	t_svm_options svm_options;
	t_svm* psvm;
	
	/*Load the database*/
	pdataset = init_dataset(DATASET_PATH);
	/*pre-process it*/
	preprocess_data(pdataset);

	split_train_test_sets(pdataset);

	/***********************************
	 * Training
	/**********************************/

	/*Load the training dataset*/
	train_samples = (double**)malloc(sizeof(double*)*pdataset->nb_train_trials);
	train_labels = (int*)malloc(sizeof(int)*pdataset->nb_train_trials);
	
	fdaed_samples = (double*)malloc(sizeof(double)*pdataset->nb_train_trials);

	for(i=0;i<pdataset->nb_train_trials;i++){
		train_samples[i] = (double*)malloc(sizeof(double)*FFTED_VECTOR_LENGTH);
	}

	/*get the training dataset*/
	get_train_dataset_ffted(pdataset, train_samples, train_labels);

//	for(i=0;i<pdataset->nb_train_trials;i++){
//		for(j=0;j<100;j++){
//			printf("%f\n",train_samples[i][j]);
//		}
//	}
	
    /*init the fda*/
	fda_options.nb_classes = 2;
	fda_options.nb_features = FFTED_VECTOR_LENGTH;
	pfda = init_fda(train_samples, train_labels, pdataset->nb_train_trials, fda_options);


	/*transform the training set*/
	transform_data(pfda, train_samples, fdaed_samples, pdataset->nb_train_trials);

	printf("Show fdaed samples");
	for(i=0;i<pdataset->nb_train_trials;i++){
		printf("%i:%f\n",i,fdaed_samples[i]);
	}
	
	
	svm_options.nb_samples = pdataset->nb_train_trials;
	svm_options.nb_features = 1;
	
	/*train the svm*/
	psvm = train_svm(fdaed_samples, train_labels, svm_options);

#if 0
	/***********************************
	 * Run-time
	/**********************************/

	/*Load test sample(s)*/

	//eegbmi_classify()




	kill_svm(psvm);
	kill_fda(pfda);
	kill_dataset(pdataset);
#endif
	return EXIT_SUCCESS;

}

/**
 * eegbmi_train(double** samples, int labels, int nb_samples, TinySVM::Model *model)
 * 
 * @brief 
 * @param 
 * @param 
 * @param 
 * @return 
 */
//int eegbmi_train(double** samples, int labels, int nb_samples, TinySVM::Model *model){
int eegbmi_train(double** samples, int labels, int nb_samples){
	
	int i = 0;
	
	/*Use the training dataset to find the fda transform*/









	
	return EXIT_SUCCESS;
}

/**
 * eegbmi_classify()
 * 
 * @brief 
 * @param 
 * @param 
 * @param 
 * @return 
 */
int eegbmi_classify(){


	/*pre-process it(them)*/


	/*represent it(them) in fisher space*/


	/*classify it(them)*/	
	
	return EXIT_SUCCESS;
}
