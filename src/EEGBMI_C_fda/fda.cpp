
#include "fda.h"

using namespace std;
using namespace Eigen;

int split_dataset(int *labels, int nb_of_samples, int** label_a_list, int** label_b_list, int* label_a_count, int* label_b_count);
VectorXd compute_features_mean(double **dataset, int *idx_list, int idx_count);
MatrixXd compute_features_covariance_mtx(double **dataset, int *idx_list, int idx_count, VectorXd feature_means);

void randsample(int array[], int n);

/*temporary, before building as a shared object*/
/*will implement a test bench*/
int main(){
	
	
	/*define a small dataset: data and labels*/
	// create a dataset, with 100 samples per class and 2 classes.
	// Samples have 10 features, one of them is correlated with the class id and
	// a second one is anti-correlated.
	
	int i,j;
	double **samples;
	double *fdaed_samples;
	int labels[200];
	int labels_idx[200];
	int sample_a_id;
	int sample_b_id;
	t_fda_options options;
	t_fda *pfda;
	
	samples = (double**)malloc(sizeof(double*)*200);
	
	for(i=0;i<200;i++){
		samples[i] = (double*)malloc(sizeof(double)*10);
	}
	
	fdaed_samples = (double*)malloc(sizeof(double)*200);
	
	randsample(labels_idx,200);

	for(i=0;i<100;i++){
		sample_a_id = labels_idx[i];
		sample_b_id = labels_idx[i+100];
		labels[sample_a_id] = 1;
		labels[sample_b_id] = -1;
		
		for(j=0;j<10;j++){
			samples[sample_a_id][j] = (double)rand()/((double)RAND_MAX);
			samples[sample_b_id][j] = (double)rand()/((double)RAND_MAX);
		}
		
		samples[sample_a_id][3] = samples[sample_a_id][3]+2*labels[sample_a_id];
		samples[sample_a_id][7] = samples[sample_a_id][7]-2*labels[sample_a_id];
		
		samples[sample_b_id][3] = samples[sample_b_id][3]+2*labels[sample_b_id];
		samples[sample_b_id][7] = samples[sample_b_id][7]-2*labels[sample_b_id];	
	}
	
	/*compute the fda*/
	options.nb_classes = 2;
	options.nb_features = 10;
	pfda = init_fda(samples, labels, 200, options);
	
	transform_data(pfda, samples, fdaed_samples, 200);

	//show the results
	for(i=0;i<200;i++){
		cout << fdaed_samples[i] << " " <<  labels[i] << endl;
	}
	
	kill_fda(pfda);

	return 1;
}

/**
 * t_fda *init_fda(double **dataset, int *labels, int nb_of_samples)
 * 
 * @brief this function computes the fisher discriminant analysis on a dataset. It returns the
 * necessary information to transform further data into the fda space.
 * @param dataset, NxF double array, where N is the number of samples and F the number of features (must match the define).
 * @param labels, N sized integer array, contains the label associated with each samples (must match the define).
 * @param nb_of_samples, the value of N.
 * @return an initialized fda struct, which contains the necessary information to transform samples
 */ 
t_fda *init_fda(double **dataset, int *labels, int nb_of_samples, t_fda_options options){
	
	int i, max_idx;
	double max_value;
	int *lbl_a_list; 
	int *lbl_b_list;
	int lbl_a_count,lbl_b_count;
	t_fda *pfda = (t_fda*)malloc(sizeof(t_fda));
	
	pfda->options = options;
	
	VectorXd feature_means_lbl_a(options.nb_features);
	VectorXd feature_means_lbl_b(options.nb_features);
	MatrixXd feature_cov_mtx_lbl_a(options.nb_features,options.nb_features);
	MatrixXd feature_cov_mtx_lbl_b(options.nb_features,options.nb_features);
	
	MatrixXd A(options.nb_features,options.nb_features);

	MatrixXd sb(options.nb_features,options.nb_features);
	MatrixXd sw(options.nb_features,options.nb_features);

	VectorXd temp_vec(options.nb_features);
	
	/*Split the two classes apart*/ 
	split_dataset(labels, 
				  nb_of_samples, 
				  &lbl_a_list, 
				  &lbl_b_list, 
				  &lbl_a_count, 
				  &lbl_b_count);
	
	         
	/*compute the class-wise mean for each feature*/
	feature_means_lbl_a = compute_features_mean(dataset, 
												lbl_a_list, 
												lbl_a_count);
      	       
	feature_means_lbl_b = compute_features_mean(dataset, 
												lbl_b_list, 
												lbl_b_count);
	
	cout << "check1" << endl;
	/*compute class-wise covariance matrices*/
	feature_cov_mtx_lbl_a = compute_features_covariance_mtx(dataset, 
															lbl_a_list, 
															lbl_a_count, 
															feature_means_lbl_a);
															
	feature_cov_mtx_lbl_b = compute_features_covariance_mtx(dataset, 
															lbl_b_list, 
															lbl_b_count, 
															feature_means_lbl_b);
	cout << "check2" << endl;
	/*Compute Sb*/
	temp_vec = (feature_means_lbl_a-feature_means_lbl_b);
	sb = temp_vec*temp_vec.transpose();
	
	/*Compute Sw*/
	sw = feature_cov_mtx_lbl_a+feature_cov_mtx_lbl_b;
	
	/*Compute A = Sw^-1*Sb*/
	A = sw.ldlt().solve(sb);
	
	/*Compute eigen vectors and values*/
	EigenSolver<MatrixXd> es(A);
	if (es.info() != Success) abort();
	
	cout << "check3" << endl;
	
	/*get the real valued part of the eigen values and vectors (the imaginnary part should be 0)*/
	VectorXd eivals = es.eigenvalues().real();
	MatrixXd eivects = es.eigenvectors().real();
	
	/*find the max eigenvalue*/
	max_value = eivals(0);
	max_idx = 0;
	
	for(i=1;i<options.nb_features;i++){
		if(eivals(i)>max_value){
			max_value = eivals(i);
			max_idx = i;
		}
	}
	
	/*resize fda struct mtx and vector*/
	pfda->eigen_vectors.resize(options.nb_features,options.nb_features);
	pfda->eigen_values.resize(options.nb_features);
	
	/*copy the eigen vectors, eigen values and max index in the struct*/
	pfda->eigen_vectors = eivects;
	pfda->eigen_values = eivals;
	pfda->max_eigen_value_idx = max_idx;
	
	
	/*output the results for reference*/
	//cout << "Here's the max eigen vector\n" << eivects.col(max_idx) << endl;
	
	cout << "check4" << endl;
	
	
	/*clean up*/
	free(lbl_a_list);
	free(lbl_b_list);
		
	return pfda;
}

/**
 * kill_fda(t_fda *pfda)
 * 
 * @brief simple clean up function.
 * @param pfda, the fda to be freed.
 */ 
void kill_fda(t_fda *pfda){
	free(pfda);
}


/**
 * int transform_data(t_fda *pfda, double **dataset, double **fdaed_dataset, int nb_samples)
 * 
 * @brief (TO BE DONE)this function brings the data contained in the dataset into the fda defined space of representation.
 * @param dataset, NxF double array, where N is the number of samples and F the number of features (must match the define).
 * @param (out)fdaed_dataset, transformed data.
 * @return 1 if success, 0 otherwise
 */ 
int transform_data(t_fda *pfda, double **dataset, double *fdaed_dataset, int nb_samples){
	
	int i,j;
	double temp_sum;
	VectorXd eign_vec(pfda->options.nb_features);
	
	eign_vec = pfda->eigen_vectors.col(pfda->max_eigen_value_idx);
	
	for(i=0;i<nb_samples;i++){
		
		temp_sum = 0.0;
		for(j=0;j<pfda->options.nb_features;j++){
			temp_sum += dataset[i][j]*eign_vec(j);
		}	
		
		fdaed_dataset[i]=temp_sum;
	}
}

/**
 * int split_dataset(int *labels, int nb_of_samples, int** label_a_list, int** label_b_list, int* label_a_count, int* label_b_count)
 * 
 * @brief (Private) This function builds the lists of samples belonging to each labels.
 * @param labels, N sized integer array, contains the label associated with each samples (must match the define).
 * @param nb_of_samples, the value of N.
 * @param label_a_list, list of index belonging to A.
 * @param label_b_list, list of index belonging to B.
 * @param label_a_count, number of samples labeled A.
 * @param label_b_count, number of samples labeled B.
 * @return 1 if success, 0 otherwise
 */ 
int split_dataset(int *labels, int nb_of_samples, int** label_a_list, int** label_b_list, int* label_a_count, int* label_b_count){
	
	int i;
	int temp_label_a_count = 0;
	int temp_label_b_count = 0;
	int *temp_label_a_list; 
	int *temp_label_b_list;
	
	/*count the number of labels equal to label A*/
	for(i=0;i<nb_of_samples;i++){
		if(labels[i]==LABEL_A){
			temp_label_a_count++;
		}
	}
	
	/*deduce the number of label b*/
	temp_label_b_count = nb_of_samples-temp_label_a_count;
	
	cout << temp_label_b_count << endl;
	cout << temp_label_a_count << endl;
	
	/*create lists*/
	temp_label_a_list = (int*)malloc(sizeof(int)*temp_label_a_count);
	temp_label_b_list = (int*)malloc(sizeof(int)*temp_label_b_count);
	
	/*reuse the same counter*/
	temp_label_a_count= 0;
	temp_label_b_count = 0;
	
	/*set indexes*/
	for(i=0;i<nb_of_samples;i++){
		if(labels[i]==LABEL_A){
			temp_label_a_list[temp_label_a_count] = i;
			temp_label_a_count++;
		}
		else{
			temp_label_b_list[temp_label_b_count] = i;
			temp_label_b_count++;
		}
		
	}
	
	/*return the counts*/
	(*label_a_count) = temp_label_a_count;
	(*label_b_count) = temp_label_b_count;
	
	(*label_a_list) = temp_label_a_list;
	(*label_b_list) = temp_label_b_list;
	
	return EXIT_SUCCESS;
}

/**
 * VectorXd compute_features_mean(double **dataset, int *idx_list, int nb_samples)
 * 
 * @brief (Private) This function computes the mean over all samples of a given label for each feature.
 * @param dataset, NxF double array, where N is the number of samples and F the number of features (must match the define).
 * @param idx_list, list of index belonging to that class.
 * @param idx_count, the number of indexes to be considered.
 * @return an F-sized vector containing the mean of each feature
 */ 
VectorXd compute_features_mean(double **dataset, int *idx_list, int idx_count){
	
	int i,j;
	double temp_sum[10];
	VectorXd mean(10);
	
	/*init the temp sum to 0.0*/
	for(i=0;i<10;i++){
		temp_sum[i] = 0.0;
	}
	
	/*compute the feature-wise sum*/
	for(i=0;i<idx_count;i++){
		for(j=0;j<10;j++){
			
			cout << idx_list[i] << endl;
			
			temp_sum[j] += dataset[idx_list[i]][j];
		}
	}
	
	/*compute the feature-wise mean*/
	for(i=0;i<10;i++){
		mean(i) = temp_sum[i]/(double)idx_count;
	}
	
	return mean;
	
}

/**
 * VectorXd compute_features_mean(double **dataset, int *idx_list, int nb_samples)
 * 
 * @brief (Private) This function computes the mean over all samples of a given label for each feature.
 * @param dataset, NxF double array, where N is the number of samples and F the number of features (must match the define).
 * @param idx_list, list of index belonging to that class.
 * @param idx_count, the number of indexes to be considered.
 * @param feature_means, F-sized vector containing the mean of each feature
 * @return an FxF-sized matrix containing the covariance of each feature
 */ 
MatrixXd compute_features_covariance_mtx(double **dataset, int *idx_list, int idx_count, VectorXd feature_means){
	
	int i,j,k;
	double temp_sum[10][10];
	MatrixXd cov_mtx(10,10);
	
	/*
	 * Compute elements of the diagonal and the top matrix
	 */
	for(j=0;j<10;j++){
		for(k=j;k<10;k++){
			temp_sum[j][k] = 0.0;
		}
	}
	 
	for(i=0;i<idx_count;i++){
		for(j=0;j<10;j++){
			for(k=j;k<10;k++){
				temp_sum[j][k] += (dataset[idx_list[i]][j] - feature_means(j))*
								  (dataset[idx_list[i]][k] - feature_means(k));
				temp_sum[j][k] /= (double)idx_count;
			}
		}
	}
	
	/*and copy top half to bottom half of the mtx*/
	for(j=0;j<10;j++){
		for(k=j;k<10;k++){
			cov_mtx(j,k) = temp_sum[j][k];
			
			/*ignore the diagonal*/
			if(j!=k)
				cov_mtx(k,j) = temp_sum[j][k];
		}
	}
	
	return cov_mtx;
}


/**
 * randsample(int array[], int n)
 * 
 * @brief return an array of n elements. The array is the series of numbers [0, n-1] randomly permutated a large number of times (n*10)
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
