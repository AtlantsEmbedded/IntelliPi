/**
 * @file main.c
 * @author Frédéric Simard
 * @date December, 2014
 * @brief This program implements the prototype/proof of concept of a EEG BMI device.
 *
 *        It does the following:
 *	      1) Reads the data from an ASCII file: train and test sets
 *		  2) Filter and downsample the trial samples (this stage is implemented in the wrapper)
 *		  3) Computes the spectrogram of each trials
 *		  4) Train the SVM using the training set
 * 		  5) Test the SVM on the test data
 *  
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "wrapper.h"
#include "filt_downsamp.h"

//#include </SVM_TinySVM-0.09/src/tinysvm.h>

/**
 * main(int argc, char **argv[])
 * 
 * @brief
 * @param argc
 * @param argv
 * @return 0 for success, non-zero for failure
 */ 
int main(int argc, char **argv[])
{
	/*registers used as iterators in loops*/
	register int ii = 0;
	register int jj = 0;
	register int kk = 0;

	t_wrapper_prop init_prop;
	t_wrapper* ptrain_data;
	t_wrapper* ptest_data;
	
	t_trial this_trial;
	
	float signal[20] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	float dwn_samp_signal[10];

	/*
	 * Reads the training and test data files
	 */
	
	strcpy(init_prop.filename, "training_set.data");
	init_prop.nb_trials = 300;
	ptrain_data = init_wrapper(init_prop);

	strcpy(init_prop.filename, "test_set.data");
	init_prop.nb_trials = 100;
	ptest_data = init_wrapper(init_prop);
	
	get_trial(ptrain_data, this_trial.features, &(this_trial.label), 299);
	printf("\n%f\n",this_trial.features[1099]);

	/*
	 * Filter and Downsample test
	 */
	
	filt_downsamp_signal(&signal, 20, &dwn_samp_signal);

	printf("\n");
	for(ii=0;ii<10;ii++){
		printf("%f\n",dwn_samp_signal[ii]);
	}




#if 0
   TinySVM::Example e;
   TinySVM::Param p;

   if (! e.read("train.svmdata")) throw;
   if (! p.set("-t 1 -d 2 -c 1")) throw;

   TinySVM::Model *m = e.learn(p);

   m->write("test_model1.svm_model");

   delete m;

#endif



	/*inform user that all is done and quit*/
	printf("Program Over!\n");
	return 0;

}
