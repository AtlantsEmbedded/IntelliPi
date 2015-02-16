
#include "fda.h"

using namespace std;
using namespace Eigen;

#if 0
typedef struct Sfda{
	Eigen::MatrixXd eigen_vectors(NB_FEATURES, NB_FEATURES);
	Eigen::VectorXd eigen_values(NB_FEATURES);
	int max_eigen_value_idx;
}t_fda;
#endif

void split_dataset(int *labels, int nb_of_samples, int* label_a_list, int* label_b_list, int* label_a_count, int* label_b_count);
VectorXd compute_features_mean(double **dataset, int *idx_list, int nb_samples);
MatrixXd compute_features_covariance_mtx(double **dataset, int *idx_list, int nb_samples, VectorXd feature_means);


/*temporary, before building as a shared object*/
int main(){
	return 1;
}

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
	
	//es.eigenvectors();
	//es.eigenvalues();
	
	/*find the maximal eigenvalue*/
	//max_idx = 0;
	//max_value = es.eigenvalues()[0];
	//for(i=1;i<NB_FEATURES;i++){
	//	if(es.eigenvalues()[i]>max_value){
	//		max_idx = i;
	//		max_value = es.eigenvalues()[i];
	//	}
	//}
	
	cout << "Here's the max eigen vector\n" << es.eigenvectors().col(0) << endl;
	
	/*clean up*/
	free(lbl_a_list);
	free(lbl_b_list);
	
	return pfda;
}


void kill_fda(t_fda *pfda){
	
}




t_fda *transform_data(t_fda *pfda, double **dataset){
	
	
}


void split_dataset(int *labels, int nb_of_samples, int* label_a_list, int* label_b_list, int* label_a_count, int* label_b_count){
	
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
}

VectorXd compute_features_mean(double **dataset, int *idx_list, int nb_samples){
	
	int i,j;
	double temp_sum[NB_FEATURES];
	VectorXd mean(NB_FEATURES);
	
	/*init the temp sum to 0.0*/
	for(i=0;i<NB_FEATURES;i++){
		temp_sum[i] = 0.0;
	}
	
	/*compute the feature-wise sum*/
	for(i=0;i<nb_samples;i++){
		for(j=0;j<NB_FEATURES;j++){
			temp_sum[j] += dataset[idx_list[i]][j];
		}
	}
	
	/*compute the feature-wise mean*/
	for(i=0;i<NB_FEATURES;i++){
		mean(i) = temp_sum[i]/(double)nb_samples;
	}
	
}

MatrixXd compute_features_covariance_mtx(double **dataset, int *idx_list, int nb_samples, VectorXd feature_means){
	
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
	 
	for(i=0;i<nb_samples;i++){
		for(j=0;j<NB_FEATURES;j++){
			for(k=j;k<NB_FEATURES;k++){
				temp_sum[j][k] += (dataset[idx_list[i]][j] - feature_means(j))*
								  (dataset[idx_list[i]][k] - feature_means(k));
				temp_sum[j][k] /= (double)nb_samples;
			}
		}
	}
	
	/*and copy top half to bottom half of the mtx at the same time*/
	for(j=0;j<NB_FEATURES;j++){
		for(k=j;k<NB_FEATURES;k++){
			cov_mtx(j,k) = temp_sum[j][k];
			
			if(j!=k)
				cov_mtx(k,j) = temp_sum[j][k];
		}
	}
	
}
