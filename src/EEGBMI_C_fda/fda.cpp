
#include "fda.h"

using namespace std;
using namespace Eigen;

int split_dataset(int *labels, int nb_of_samples, int* label_a_list, int* label_b_list, int* label_a_count, int* label_b_count);
VectorXd compute_features_mean(double **dataset, int *idx_list, int idx_count);
MatrixXd compute_features_covariance_mtx(double **dataset, int *idx_list, int idx_count, VectorXd feature_means);


/*temporary, before building as a shared object*/
/*will implement a test bench*/
int main(){
	
	
	/*define a small dataset: data and labels*/
	
	
	
	
	/*compute the fda and show the results*/
	
	
	
	
	
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
t_fda *init_fda(double **dataset, int *labels, int nb_of_samples){
	
	int i, max_idx;
	double max_value;
	int *lbl_a_list; 
	int *lbl_b_list;
	int lbl_a_count,lbl_b_count;
	t_fda *pfda = (t_fda*)malloc(sizeof(t_fda));
	
	VectorXd feature_means_lbl_a(NB_FEATURES);
	VectorXd feature_means_lbl_b(NB_FEATURES);
	MatrixXd feature_cov_mtx_lbl_a(NB_FEATURES,NB_FEATURES);
	MatrixXd feature_cov_mtx_lbl_b(NB_FEATURES,NB_FEATURES);
	
	MatrixXd A(NB_FEATURES,NB_FEATURES);

	MatrixXd sb(NB_FEATURES,NB_FEATURES);
	MatrixXd sw(NB_FEATURES,NB_FEATURES);

	VectorXd temp_vec(NB_FEATURES);
	
	/*Split the two classes apart*/ 
	split_dataset(labels, 
				  nb_of_samples, 
				  lbl_a_list, 
				  lbl_b_list, 
				  &lbl_a_count, 
				  &lbl_b_count);
	
	/*compute the class-wise mean for each feature*/
	feature_means_lbl_a = compute_features_mean(dataset, 
												lbl_a_list, 
												lbl_a_count);
	                      
	feature_means_lbl_b = compute_features_mean(dataset, 
												lbl_b_list, 
												lbl_b_count);
	
	/*compute class-wise covariance matrices*/
	feature_cov_mtx_lbl_a = compute_features_covariance_mtx(dataset, 
															lbl_a_list, 
															lbl_a_count, 
															feature_means_lbl_a);
															
	feature_cov_mtx_lbl_b = compute_features_covariance_mtx(dataset, 
															lbl_b_list, 
															lbl_b_count, 
															feature_means_lbl_b);
	
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
	
	/*get the real valued part of the eigen values and vectors (the imaginnary part should be 0)*/
	VectorXd eivals = es.eigenvalues().real();
	MatrixXd eivects = es.eigenvectors().real();
	
	/*find the max eigenvalue*/
	max_value = eivals(0);
	max_idx = 0;
	
	for(i=1;i<NB_FEATURES;i++){
		if(eivals(i)>max_value){
			max_value = eivals(i);
			max_idx = i;
		}
	}
	
	/*resize fda struct mtx and vector*/
	pfda->eigen_vectors.resize(NB_FEATURES,NB_FEATURES);
	pfda->eigen_values.resize(NB_FEATURES);
	
	/*copy the eigen vectors, eigen values and max index in the struct*/
	pfda->eigen_vectors = eivects;
	pfda->eigen_values = eivals;
	pfda->max_eigen_value_idx = max_idx;
	
	/*output the results for reference*/
	cout << "Here's the max eigen vector\n" << eivects.col(max_idx) << endl;
	
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
 * int transform_data(t_fda *pfda, double **dataset, double **fdaed_dataset)
 * 
 * @brief (TO BE DONE)this function brings the data contained in the dataset into the fda defined space of representation.
 * @param dataset, NxF double array, where N is the number of samples and F the number of features (must match the define).
 * @param (out)fdaed_dataset, transformed data.
 * @return 1 if success, 0 otherwise
 */ 
int transform_data(t_fda *pfda, double **dataset, double **fdaed_dataset){
	
	
}

/**
 * int split_dataset(int *labels, int nb_of_samples, int* label_a_list, int* label_b_list, int* label_a_count, int* label_b_count)
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
int split_dataset(int *labels, int nb_of_samples, int* label_a_list, int* label_b_list, int* label_a_count, int* label_b_count){
	
	int i;
	int temp_label_a_count = 0;
	int temp_label_b_count = 0;
	
	/*count the number of labels equal to label A*/
	for(i=0;i<nb_of_samples;i++){
		if(labels[i]==LABEL_A){
			temp_label_a_count++;
		}
	}
	
	/*deduce the number of label b*/
	temp_label_b_count = nb_of_samples-temp_label_a_count;
	
	/*create lists*/
	label_a_list = (int*)malloc(sizeof(int)*temp_label_a_count);
	label_b_list = (int*)malloc(sizeof(int)*temp_label_b_count);
	
	/*reuse the same counter*/
	temp_label_a_count= 0;
	temp_label_b_count = 0;
	
	/*set indexes*/
	for(i=0;i<nb_of_samples;i++){
		if(labels[i]==LABEL_A){
			label_a_list[temp_label_a_count] = i;
			temp_label_a_count++;
		}
		else{
			label_b_list[temp_label_b_count] = i;
			temp_label_b_count++;
		}
		
	}
	
	/*return the counts*/
	(*label_a_count) = temp_label_a_count;
	(*label_b_count) = temp_label_b_count;
	
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
	double temp_sum[NB_FEATURES];
	VectorXd mean(NB_FEATURES);
	
	/*init the temp sum to 0.0*/
	for(i=0;i<NB_FEATURES;i++){
		temp_sum[i] = 0.0;
	}
	
	/*compute the feature-wise sum*/
	for(i=0;i<idx_count;i++){
		for(j=0;j<NB_FEATURES;j++){
			temp_sum[j] += dataset[idx_list[i]][j];
		}
	}
	
	/*compute the feature-wise mean*/
	for(i=0;i<NB_FEATURES;i++){
		mean(i) = temp_sum[i]/(double)idx_count;
	}
	
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
	double temp_sum[NB_FEATURES][NB_FEATURES];
	MatrixXd cov_mtx(NB_FEATURES,NB_FEATURES);
	
	/*
	 * Compute elements of the diagonal and the top matrix
	 */
	for(j=0;j<NB_FEATURES;j++){
		for(k=j;k<NB_FEATURES;k++){
			temp_sum[j][k] = 0.0;
		}
	}
	 
	for(i=0;i<idx_count;i++){
		for(j=0;j<NB_FEATURES;j++){
			for(k=j;k<NB_FEATURES;k++){
				temp_sum[j][k] += (dataset[idx_list[i]][j] - feature_means(j))*
								  (dataset[idx_list[i]][k] - feature_means(k));
				temp_sum[j][k] /= (double)idx_count;
			}
		}
	}
	
	/*and copy top half to bottom half of the mtx*/
	for(j=0;j<NB_FEATURES;j++){
		for(k=j;k<NB_FEATURES;k++){
			cov_mtx(j,k) = temp_sum[j][k];
			
			/*ignore the diagonal*/
			if(j!=k)
				cov_mtx(k,j) = temp_sum[j][k];
		}
	}
	
}
