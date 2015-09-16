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
	if (!param.set("-l 0 -t 0 -c 0.2")){
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
int save_svm(t_svm* psvm, char* name){
	
	FILE* pfile;
	char buffer[80];
	
	strcpy(buffer,name);
	strcat(buffer,".svm_ex");
	
	psvm->model->write (buffer);
	
	strcpy(buffer,name);
	strcat(buffer,".svm_sv");
	
	psvm->model->writeSVindex(buffer);
	
	strcpy(buffer,name);
	strcat(buffer,".svm_opt");
	
	pfile = fopen(buffer,"wb");
	
	if(!pfile){
		printf("Error: svm save option file cannot be opened\n");
		return EXIT_FAILURE;
	}
	
	fwrite(&(psvm->options.nb_samples),sizeof(int),1,pfile);
	fwrite(&(psvm->options.nb_features),sizeof(int),1,pfile);
	
	return EXIT_SUCCESS;
}

/**
 * t_svm* load_svm()
 * 
 * @brief This function loads a svm examples/alphas/gradients
 * @param psvm, a trained svm 
 * @return the svm loaded by the function
 */
t_svm* load_svm(char* name){
	
	FILE* pfile;
	char buffer[80];
	
	/*init the empty svm*/
	t_svm* psvm = (t_svm*)malloc(sizeof(t_svm));
	psvm->model = new TinySVM::Model();
	
	strcpy(buffer,name);
	strcat(buffer,".svm_ex");
	
	/*read the saved parameters*/
	psvm->model->read(buffer);
	
	strcpy(buffer,name);
	strcat(buffer,".svm_sv");
		
	psvm->model->readSVindex(buffer);
	
	strcpy(buffer,name);
	strcat(buffer,".svm_opt");
		
	pfile = fopen(buffer,"wb");
	
	if(!pfile){
		printf("Error: svm save option file cannot be opened\n");
		return NULL;
	}
	
	fread(&(psvm->options.nb_samples),sizeof(int),1,pfile); // this do not reflect the number of saved examples to be fixed
	fread(&(psvm->options.nb_features),sizeof(int),1,pfile);
	
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

