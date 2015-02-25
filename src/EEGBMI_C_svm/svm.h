#ifndef SVM_INTERFACE_H
#define SVM_INTERFACE_H


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "common.h"
#include "misc.h"
#include "model.h"
#include "example.h"
#include "base_example.h"


typedef struct Ssvm_options{
	int nb_samples;
	int nb_features;
}t_svm_options;

typedef struct Ssvm{
	t_svm_options options;
	TinySVM::Model *model;
}t_svm;

t_svm* train_svm(double **samples, int* labels, t_svm_options options);
void kill_svm(t_svm* psvm);

double classify_with_svm(t_svm* psvm, double *samples);

int save_svm(t_svm* psvm, char* name);
t_svm* load_svm(char* name);





#endif
