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
#define COMPUTE_SVM 1


int debug_save_samples(char* filename, double **samples, int *labels, int nb_samples, int nb_features);

const char* DATASET_PATH = "ascii_files_3/";
const char* CONFIGFILE_PATH = "config_files/";

int main()
{
	int i,j,k;
	t_dataset *pdataset;
	double** train_samples;
	int* train_labels;
	
	double** fdaed_samples;
	
	double max_value;
	
	t_feat_scaler *pfft_scaler;
	
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
	pfft_scaler = load_feat_scaler("config_files/fft_scaler.scl");
#endif	
	/*scale de features*/
	scale_features(pfft_scaler, train_samples, pdataset->nb_train_trials, FFTED_VECTOR_LENGTH, STANDARTISATION);
	
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
	pfda = load_fda("config_files/fda_dataset1.fdadta");
	
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

	/*scale fdaed data*/
	//scale_features(pfda_scaler, fdaed_samples, pdataset->nb_train_trials, 2, SCALING);

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
	save_svm(psvm,"config_files/svm");
#else
	psvm = load_svm("config_files/svm");
#endif

#if 1
	/***********************************
	 * Run-time
	/**********************************/

	int test_label;
	
	int nb_success = 0;
	double predicted_label;
	double predicted_label_prime;
	double **test_sample = (double**)malloc(sizeof(double*)*NB_CHANNELS);
	double **ffted_test_sample = (double**)malloc(sizeof(double*)*NB_CHANNELS);
	
	double *ffted_vector = (double*)malloc(sizeof(double)*FFTED_VECTOR_LENGTH);
	int offset;
	
	double fdaed_test_sample;
	
	for(i=0;i<NB_CHANNELS;i++){
		test_sample[i] = (double*)malloc(sizeof(double)*TIMESERIES_LENGTH);
		ffted_test_sample[i] = (double*)malloc(sizeof(double)*TIMESERIES_LENGTH);
	}

	
	/*simulate single sample classification*/
	for(i=0;i<1000;i++){
	
		printf("Sample: %i : ",i);
		/*Load test sample(s) timeseries*/
		get_test_sample_timeseries(pdataset, test_sample, &test_label);
	
		for(j=0;j<NB_CHANNELS/2;j++){
			/*fft the 4 channels*/
			abs_fft_2signals(test_sample[j*2], 
							 test_sample[j*2+1], 
							 ffted_test_sample[j*2], 
							 ffted_test_sample[j*2+1],
							 TIMESERIES_LENGTH);
		}
		
		/*form the vector*/
		
		for(j=0;j<NB_CHANNELS/2;j++){
			offset = j*TIMESERIES_LENGTH/2;
			
			/*take only the first half of the fft vector*/
			for(k=0;k<TIMESERIES_LENGTH/2;k++){
				ffted_vector[k+offset] = ffted_test_sample[j][k];
			}
		}
		
		/*scale the fft*/
		scale_features(pfft_scaler, &ffted_vector, 1, FFTED_VECTOR_LENGTH, STANDARTISATION);
		
		/*fda the features*/
		/*transform the training set*/
		double* pfdaed_test_sample = &fdaed_test_sample;
		
		transform_data(pfda, &ffted_vector, &pfdaed_test_sample, 1);
	
		/*classify it*/
		predicted_label = classify_with_svm(psvm, &fdaed_test_sample);
		
		if(predicted_label<-3.5 | predicted_label>-1.2){
			predicted_label_prime = 1;
		}
		else{
			predicted_label_prime = -1;
		}
		
		/*Show output*/
		printf("labels: P:%f D:%i: ",predicted_label, test_label);
		
		if((predicted_label_prime>0 & test_label>0) | (predicted_label_prime<0 & test_label<0)){
			printf("Success!\n",predicted_label, test_label);
			nb_success++;
		}
		else{
			printf("Failed!\n",predicted_label, test_label);
		}
		
		
	
	}
	
	printf("%f\n",(double)nb_success/1000);

#endif

	kill_feat_scaler(pfft_scaler);
	kill_svm(psvm);
	kill_fda(pfda);
	kill_dataset(pdataset);
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
