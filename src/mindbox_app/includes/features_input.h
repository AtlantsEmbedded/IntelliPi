#ifndef FEATURES_INPUT_H
#define FEATURES_INPUT_H



typedef struct feature_vect_s{
	int nb_features;
	double* feature_vect;
}feature_vect_t;

int get_sample(feature_vect_t* sample);

/*Build a struct of function pointers*/




#endif
