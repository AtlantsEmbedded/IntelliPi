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

using namespace std;
using namespace Eigen;


int eegbmi_train(double** train_dataset, int nb_train_samples,TinySVM::Model *model);
int eegbmi_classify();

const char* DATASET_PATH = "ascii_files_3/";

int main()
{
	t_dataset *pdataset;

	/*Load the database*/
	pdataset = init_dataset(DATASET_PATH);
	/*pre-process it*/
	preprocess_data(pdataset);


/***********************************
 * Training
/**********************************/

/*Load train samples*/

//eegbmi_train()

/***********************************
 * Run-time
/**********************************/

/*Load test sample(s)*/

//eegbmi_classify()





void kill_dataset(t_dataset *pdataset);

return EXIT_SUCCESS;

}



int eegbmi_train(double** train_dataset, int nb_train_samples,TinySVM::Model *model){
	
	int i = 0;

	/*Load the training dataset*/


	/*Use the training dataset to find the fda transform*/


#if 0
	/*Train the svm classifier*/	
	/*Initialise a TinySVM::Param, with default options*/
	TinySVM::Param param;
	
	/*Initialise a TinySVM::Example*/
	TinySVM::Example example;
	
	/*load the training dataset*/
	
	for(i=0;i<nb_train_samples;i++){
		
		/*Match sample to SVM lib standard*/
		
		
		if (!example.add(label, &feature_node)) {
			fprintf (stderr, "Train database add() failed");
			exit (EXIT_FAILURE);
		}
	}
	
	/*Train TinySVM::model*/
	TinySVM::Model *model = example.learn (param);
	if (!model) {
		fprintf (stderr, "%s: Unexpected error occurs\n", argv[0]);
		exit (EXIT_FAILURE);
	}	
#endif
	
	return EXIT_SUCCESS;
}


int eegbmi_classify(){


	/*pre-process it(them)*/


	/*represent it(them) in fisher space*/


	/*classify it(them)*/	
	
	return EXIT_SUCCESS;
}
