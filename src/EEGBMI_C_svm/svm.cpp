/**
 * @file svm.cpp
 * @author Frédéric Simard
 * @date February, 2015
 * @brief This file implements a wrapper over TinySVM. It has been developed
 *        for my EEG BMI application and is meant to be compiled for the Raspberry Pi.
 *        It offers basic functionnalities to train, classify, save and load a svm.
 *        the main advantage is that this wrapper is compiled as a lib and do not require
 *        the caller to used typedef defines in the svm library.
 * 
 *        Feel free to use, modify and distribute, but please respect authorship
 *        Atom Embedded, 2015.
 */

#include "svm.h"

using namespace std;
using namespace TinySVM;

int convert_sample_2_feature_nodes(double *sample, feature_node *feature_nodes, int nb_features);

/**
 * t_svm* train_svm(double **samples, int* labels, t_svm_options options)
 * 
 * @brief This function initialize and train a svm, using the tiny svm lib
 * @param samples, array of training samples, dim NxF
 * @param labels, labels of the training samples
 * @param options, options for the creation of the svm
 * @return (out), a trained svm is success, NULL otherwise
 */
t_svm* train_svm(double **samples, int* labels, t_svm_options options){
	
	int i;
	
	Param param;
	Example example;
	t_svm* psvm = (t_svm*)malloc(sizeof(t_svm));
	feature_node *feature_nodes;
	
	/*copy the options*/
	psvm->options = options;
	
	/*set the parameters for a default svm*/
	if (!param.set("-l 0 -t 1 -d 2 -c 1")){
		fprintf(stderr, "Error params");
		goto cleanup;
	}
	
	/*add all samples to the training set*/
	for(i=0;i<options.nb_samples;i++){
		
		/*allocate memory for feature node (this is not a memory leak)*/
		feature_nodes = (feature_node*)malloc(sizeof(feature_node)*(options.nb_features+1));
		
		convert_sample_2_feature_nodes(samples[i], feature_nodes, options.nb_features);
		
		if (!example.add((double)labels[i], feature_nodes)) {
			fprintf (stderr, "Train database add() failed");
			goto cleanup;
		}
		
	}
	
	/*learn the alpha and gradients*/
	psvm->model = example.learn (param);

	if (!psvm->model) {
		fprintf (stderr, "Unexpected error occured in model training\n");
		goto cleanup;
	}
	
	/*we're good*/
	return psvm;
	
cleanup:
	/*oups, cleanup*/
	free(psvm);
	return NULL;
}

/**
 * kill_svm(t_svm* psvm)
 * 
 * @brief This function clean the memory for a svm instance
 */
void kill_svm(t_svm* psvm){
	
	delete psvm->model;
	free(psvm);
}

/**
 * double classify_with_svm(t_svm* psvm, double *sample)
 * 
 * @brief This function classify the sample, using the svm received in parameters
 * @param psvm, a trained svm 
 * @param sample, labels of the training samples
 * @return the predicted class of the sample
 */
double classify_with_svm(t_svm* psvm, double *sample){
	
	double sample_class = 0.0;
	feature_node feature_nodes[psvm->options.nb_features+1];
	
	/*convert the sample to a feature node*/
	convert_sample_2_feature_nodes(sample, feature_nodes, psvm->options.nb_features);
	
	/*classify*/
	sample_class = psvm->model->classify(feature_nodes);
	
	return sample_class;
}

/**
 * int save_svm(t_svm* psvm)
 * 
 * @brief This function saves the svm examples/alphas/gradients
 * @param psvm, a trained svm 
 * @return EXIT_SUCCESS
 */
int save_svm(t_svm* psvm){
	
	psvm->model->write ("test_svm_examples.svm_ex");
	psvm->model->writeSVindex ("test_svm_svidx.svm_sv");
	
	return EXIT_SUCCESS;
}

/**
 * t_svm* load_svm()
 * 
 * @brief This function loads a svm examples/alphas/gradients
 * @param psvm, a trained svm 
 * @return the svm loaded by the function
 */
t_svm* load_svm(){
	
	/*init the empty svm*/
	t_svm* psvm = (t_svm*)malloc(sizeof(t_svm));
	psvm->model = new TinySVM::Model();
	
	/*read the saved parameters*/
	psvm->model->read("test_svm_examples.svm_ex");	
	psvm->model->readSVindex("test_svm_svidx.svm_sv");
	
	/*hardcoded options for now*/
	//psvm->options.nb_samples = 180; // this do not reflect the number of saved examples however
	psvm->options.nb_features = 1;
	
	return psvm;
}

/**
 * int convert_sample_2_feature_nodes(double *sample, feature_node **feature_nodes, int nb_features)
 * 
 * @brief This function loads a svm examples/alphas/gradients
 * @param sample, a sample to be converted
 * @param (out)feature_nodes, the feature_nodes returned
 * @param nb_features, the number of features
 * @return EXIT_SUCCESS
 */
int convert_sample_2_feature_nodes(double *sample, feature_node *feature_nodes, int nb_features){
	
	int i;
	
	/*copy the sample into the feature node*/
	for(i=0;i<nb_features;i++){
		feature_nodes[i].index = i;
		feature_nodes[i].value = sample[i];
	}
	
	/*dummy index (used by the svm lib to stop loops)*/
	feature_nodes[i].index = -1;
		
	return EXIT_SUCCESS;
}

