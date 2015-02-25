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
#include "feature_scaler.h"

#include "svm.h"
#include "fft.h"
#include "fda.h"

using namespace std;
//using namespace Eigen;

#define DEBUG_LOG 0
#define COMPUTE_FDA 0
#define COMPUTE_SCALER 0
#define COMPUTE_SVM 0


//int eegbmi_train(double** train_dataset, int nb_train_samples,TinySVM::Model *model);
//int eegbmi_train(double** train_dataset, int nb_train_samples);
//int eegbmi_classify();
int debug_save_samples(char* filename, double **samples, int *labels, int nb_samples, int nb_features);

const char* DATASET_PATH = "ascii_files_3/";

int main()
{
	int i,j;
	t_dataset *pdataset;
	double** train_samples;
	int* train_labels;
	
	double** fdaed_samples;
	
	double max_value;
	
	t_feat_scaler *pfft_scaler;
	t_feat_scaler *pfda_scaler;
	
	t_fda_options fda_options;
	t_fda *pfda;

	t_svm_options svm_options;
	t_svm* psvm;
	
	/*Load the database*/
	pdataset = init_dataset(DATASET_PATH);
	/*pre-process it*/
	preprocess_data(pdataset);

	/*split the dataset in train and test samples*/
	split_train_test_sets(pdataset);

	/***********************************
	 * Training
	/**********************************/
	
	/*
	 *Load the training dataset
	 */
	 
	/*allocate memory*/
	train_samples = (double**)malloc(sizeof(double*)*pdataset->nb_train_trials);
	train_labels = (int*)malloc(sizeof(int)*pdataset->nb_train_trials);
	fdaed_samples = (double**)malloc(sizeof(double*)*pdataset->nb_train_trials);

	for(i=0;i<pdataset->nb_train_trials;i++){
		train_samples[i] = (double*)malloc(sizeof(double)*FFTED_VECTOR_LENGTH);
		fdaed_samples[i] = (double*)malloc(sizeof(double)*2);
	}

	/*get the ffted training dataset*/
	get_train_dataset_ffted(pdataset, train_samples, train_labels);
	
#if DEBUG_LOG
	debug_save_samples("ffted_samples.dbg", train_samples, train_labels, pdataset->nb_train_trials, FFTED_VECTOR_LENGTH);
#endif	
	
	/*
	 * Rescale the data
	 */
	
#if COMPUTE_SCALER
	/*init the ffted dataset scaler*/
	pfft_scaler = init_feat_scaler(train_samples, pdataset->nb_train_trials, FFTED_VECTOR_LENGTH);
	save_feat_scaler(pfft_scaler,"fft_scaler.scl");
#else
	pfft_scaler = load_feat_scaler("fft_scaler.scl");
#endif	
	/*scale de features*/
	scale_features(pfft_scaler, train_samples, pdataset->nb_train_trials, FFTED_VECTOR_LENGTH, SCALING);
	
#if DEBUG_LOG
	debug_save_samples("ffted_samples_normed.dbg", train_samples, train_labels, pdataset->nb_train_trials, FFTED_VECTOR_LENGTH);
#endif

	/*
	 * FDA the data
	 */
	
#if COMPUTE_FDA

    /*init the fda option*/
	fda_options.nb_classes = 2;
	fda_options.nb_features = FFTED_VECTOR_LENGTH;
	/*init and compute the fda*/
	pfda = init_fda(train_samples, train_labels, pdataset->nb_train_trials, fda_options);
	/*save it*/
	save_fda(pfda, "fda_dataset1.fdadta");
	
#else

	/*load the fda*/
	pfda = load_fda("fda_dataset1.fdadta");
	
#endif

	/*transform the training set*/
	transform_data(pfda, train_samples, fdaed_samples, pdataset->nb_train_trials);
	
#if DEBUG_LOG
	debug_save_samples("fdaed_samples.dbg", fdaed_samples, train_labels, pdataset->nb_train_trials, 1);
#endif
	
	/*pad in 1s as second feature, SVM kind of don't like 1 feature dataset... (TO BE INVESTIGATED)*/
	for(i=0;i<pdataset->nb_train_trials;i++){
		fdaed_samples[i][1] = 1.0;
	}
	
	/*
	 * Rescale the data
	 */
	
#if COMPUTE_SCALER
	/*init fda scaler*/
	pfda_scaler = init_feat_scaler(fdaed_samples, pdataset->nb_train_trials, 2);
	save_feat_scaler(pfda_scaler,"fda_scaler.scl");
#else
	pfda_scaler = load_feat_scaler("fda_scaler.scl");
#endif	

	/*scale fdaed data*/
	scale_features(pfda_scaler, fdaed_samples, pdataset->nb_train_trials, 2, SCALING);

#if DEBUG_LOG
	debug_save_samples("fdaed_samples_normed.dbg", fdaed_samples, train_labels, pdataset->nb_train_trials, 1);
#endif

	/*
	 * Train the SVM
	 */
	 	
#if COMPUTE_SVM
	/*init svm options*/
	svm_options.nb_samples = pdataset->nb_train_trials;
	svm_options.nb_features = 2;
	/*train the svm*/
	psvm = train_svm(fdaed_samples, train_labels, svm_options);
	save_svm(psvm,"svm");
#else
	psvm = load_svm("svm");
#endif

#if 1
	/***********************************
	 * Run-time
	/**********************************/

	/*Load test sample(s) timeseries*/
	
	/*fft them*/
	
	/*classify them one by one*/

	/*compute performance*/

#endif

	kill_feat_scaler(pfft_scaler);
	kill_feat_scaler(pfda_scaler);
	kill_svm(psvm);
	kill_fda(pfda);
	kill_dataset(pdataset);
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


int debug_save_samples(char* filename, double **samples, int *labels, int nb_samples, int nb_features){

	int i,j;
	FILE *pfile = fopen(filename,"w");
	
	fprintf(pfile,"%i\n",nb_samples);
	fprintf(pfile,"%i\n",nb_features);

	for(i=0;i<nb_samples;i++){
		
		fprintf(pfile,"%i ",labels[i]);
		
		for(j=0;j<nb_features;j++){
			
			fprintf(pfile,"%lf ",samples[i][j]);
		}
		fprintf(pfile,"\n");
	}
	
	fclose(pfile);

	return EXIT_SUCCESS;	
	
}
