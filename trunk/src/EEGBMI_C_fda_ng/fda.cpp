
#include "fda.h"

using namespace std;
//using namespace Eigen;

//int split_dataset(int *labels, int nb_of_samples, int** label_a_list, int** label_b_list, int* label_a_count, int* label_b_count);
//VectorXd compute_features_mean(double **dataset, int *idx_list, int idx_count, int nb_features);
//MatrixXd compute_features_covariance_mtx(double **dataset, int *idx_list, int idx_count, VectorXd feature_means, int nb_features);



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
	
	printf("FDA function not supported in fda_ng\n");

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
 * @brief this function brings the data contained in the dataset into the fda defined space of representation.
 * @param dataset, NxF double array, where N is the number of samples and F the number of features (must match the define).
 * @param (out)fdaed_dataset, transformed data.
 * @param nb_samples, number of samples.
 * @return EXIT_SUCCESS if success, EXIT_FAILURE otherwise
 */ 
int transform_data(t_fda *pfda, double **dataset, double **fdaed_dataset, int nb_samples){
	
	int i,j;
	double temp_sum;
	double eign_vec[pfda->options.nb_features]; 
	
	for(i=0;i<pfda->options.nb_features;i++){
		eign_vec[i] = pfda->eigen_vectors[i][pfda->max_eigen_value_idx];
	}
	
	for(i=0;i<nb_samples;i++){
		
		temp_sum = 0.0;
		for(j=0;j<pfda->options.nb_features;j++){
			temp_sum += dataset[i][j]*eign_vec[j];
		}	
		
		fdaed_dataset[i][0]=temp_sum;
	}
	
	return EXIT_SUCCESS;
}

/**
 * int save_fda(t_fda *pfda, char* filename)
 * 
 * @brief Saves the FDA parameters in a binary file.
 * @param pfda, FDA to be saved
 * @param filename, filename to be used.
 * @return EXIT_SUCCESS if success, EXIT_FAILURE otherwise
 */ 
int save_fda(t_fda *pfda, char* filename){
	
	printf("FDA function not supported in fda_ng\n");
	
	return EXIT_SUCCESS;
}

/**
 * t_fda *load_fda(char* filename)
 * 
 * @brief Loads the FDA parameters from a binary file.
 * @param filename, filename to be used.
 * @return a FDA ready to be used
 */ 
t_fda *load_fda(char* filename){

	int i,j;	
	t_fda *pfda = (t_fda*)malloc(sizeof(t_fda));
	FILE* pfile;
	double temp_d;
	int temp_i;
	
	pfile = fopen(filename,"rb");
	
	if(!pfile){
		printf("Error: fda save file cannot be opened\n");
		return NULL;
	}
	
	fread(&(pfda->options.nb_classes),sizeof(int),1,pfile);
	fread(&(pfda->options.nb_features),sizeof(int),1,pfile);
	
	pfda->eigen_vectors = (double**)malloc(sizeof(double*)*pfda->options.nb_features);
	for(i=0;i<pfda->options.nb_features;i++){
		pfda->eigen_vectors[i] = (double*)malloc(sizeof(double)*pfda->options.nb_features);
	}
		
	pfda->eigen_values = (double*)malloc(sizeof(double)*pfda->options.nb_features);
	
	for(i=0;i<pfda->options.nb_features;i++){
		for(j=0;j<pfda->options.nb_features;j++){
			fread(&temp_d,sizeof(double),1,pfile);
			pfda->eigen_vectors[i][j] = temp_d;
		}
	}
	
	for(i=0;i<pfda->options.nb_features;i++){
		fread(&temp_d,sizeof(double),1,pfile);
		pfda->eigen_values[i]= temp_d;
	}

	fread(&(pfda->max_eigen_value_idx),sizeof(int),1,pfile);	
	
	fclose(pfile);	
	
	return pfda;
}


#if 0

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
 * VectorXd compute_features_mean(double **dataset, int *idx_list, int nb_samples, int nb_features)
 * 
 * @brief (Private) This function computes the mean over all samples of a given label for each feature.
 * @param dataset, NxF double array, where N is the number of samples and F the number of features (must match the define).
 * @param idx_list, list of index belonging to that class.
 * @param idx_count, the number of indexes to be considered.
 * @return an F-sized vector containing the mean of each feature
 */ 
VectorXd compute_features_mean(double **dataset, int *idx_list, int idx_count, int nb_features){
	
	int i,j;
	double temp_sum[nb_features];
	VectorXd mean(nb_features);
	
	/*init the temp sum to 0.0*/
	for(i=0;i<nb_features;i++){
		temp_sum[i] = 0.0;
	}
	
	/*compute the feature-wise sum*/
	for(i=0;i<idx_count;i++){
		for(j=0;j<nb_features;j++){
			
			temp_sum[j] += dataset[idx_list[i]][j];
		}
	}
	
	/*compute the feature-wise mean*/
	for(i=0;i<nb_features;i++){
		mean(i) = temp_sum[i]/(double)idx_count;
	}
	
	return mean;
	
}

/**
 * MatrixXd compute_features_covariance_mtx(double **dataset, int *idx_list, int idx_count, VectorXd feature_means)
 * 
 * @brief (Private) This function computes the mean over all samples of a given label for each feature.
 * @param dataset, NxF double array, where N is the number of samples and F the number of features (must match the define).
 * @param idx_list, list of index belonging to that class.
 * @param idx_count, the number of indexes to be considered.
 * @param feature_means, F-sized vector containing the mean of each feature
 * @return an FxF-sized matrix containing the covariance of each feature
 */ 
MatrixXd compute_features_covariance_mtx(double **dataset, int *idx_list, int idx_count, VectorXd feature_means, int nb_features){
	
	int i,j,k;
	double **temp_sum;
	
	/*size require it to be dynamic*/
	temp_sum = (double**)malloc(sizeof(double*)*nb_features);
	MatrixXd cov_mtx(nb_features,nb_features);
	
	/*
	 * Compute elements of the diagonal and the top matrix
	 */
	for(j=0;j<nb_features;j++){
		temp_sum[j] = (double*)malloc(sizeof(double)*nb_features);
		for(k=j;k<nb_features;k++){
			temp_sum[j][k] = 0.0;
		}
	}
	
	cout << "fda: Cov Mtx step 1" << endl;
	 
	for(i=0;i<idx_count;i++){
		for(j=0;j<nb_features;j++){
			for(k=j;k<nb_features;k++){
				temp_sum[j][k] += (dataset[idx_list[i]][j] - feature_means(j))*
								  (dataset[idx_list[i]][k] - feature_means(k));
				temp_sum[j][k] /= (double)idx_count;
			}
		}
	}
	
	cout << "fda: Cov Mtx step 2" << endl;
	
	/*and copy top half to bottom half of the mtx*/
	for(j=0;j<nb_features;j++){
		for(k=j;k<nb_features;k++){
			cov_mtx(j,k) = temp_sum[j][k];
			
			/*ignore the diagonal*/
			if(j!=k)
				cov_mtx(k,j) = temp_sum[j][k];
		}
	}
	
	/*clean up memory*/
	for(j=0;j<nb_features;j++){
		free(temp_sum[j]);
	}
	free(temp_sum);
	
	return cov_mtx;
}

#endif
