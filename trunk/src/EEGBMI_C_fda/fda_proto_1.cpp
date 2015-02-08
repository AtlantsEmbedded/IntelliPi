#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Cholesky>

using namespace std;
using namespace Eigen;
int main()
{

register int i,j;
double temp_sum_1,temp_sum_2;
/*initialize two matrices*/
MatrixXd x1(5,2);
MatrixXd x2(5,2);

MatrixXd cov_x1(2,2);
MatrixXd cov_x2(2,2);
MatrixXd A(2,2);

MatrixXd sb(2,2);
MatrixXd sw(2,2);

VectorXd mean_x1(2);
VectorXd mean_x2(2);
VectorXd temp_vec(2);


x1 << 4, 1, 2, 4, 2, 3, 3, 6, 4, 4;
x2 << 9, 10, 6, 8, 9, 5, 8, 7, 10, 8;
mean_x1 = x1.colwise().sum()/x1.rows();
mean_x2 = x2.colwise().sum()/x2.rows();

/*Compute covariance matrices, one element at a time*/

/*(0,0)*/
temp_sum_1 = 0.0;
temp_sum_2 = 0.0;

for(i=0;i<x1.rows();i++){
	temp_sum_1 += (x1(i,0)-mean_x1(0))*(x1(i,0)-mean_x1(0));
	temp_sum_2 += (x2(i,0)-mean_x2(0))*(x2(i,0)-mean_x2(0));
}

cov_x1(0,0) = temp_sum_1/x1.rows();
cov_x2(0,0) = temp_sum_2/x2.rows(); 

/*(0,1) and (1,0)*/
temp_sum_1 = 0.0;
temp_sum_2 = 0.0;

for(i=0;i<x1.rows();i++){
	temp_sum_1 += (x1(i,0)-mean_x1(0))*(x1(i,1)-mean_x1(1));
	temp_sum_2 += (x2(i,0)-mean_x2(0))*(x2(i,1)-mean_x2(1));
}

cov_x1(0,1) = temp_sum_1/x1.rows();
cov_x2(0,1) = temp_sum_2/x2.rows(); 
cov_x1(1,0) = cov_x1(0,1);
cov_x2(1,0) = cov_x2(0,1);


/*(1,1)*/
temp_sum_1 = 0.0;
temp_sum_2 = 0.0;

for(i=0;i<x1.rows();i++){
	temp_sum_1 += (x1(i,1)-mean_x1(1))*(x1(i,1)-mean_x1(1));
	temp_sum_2 += (x2(i,1)-mean_x2(1))*(x2(i,1)-mean_x2(1));
}

cov_x1(1,1) = temp_sum_1/x1.rows();
cov_x2(1,1) = temp_sum_2/x2.rows(); 

/*Compute SB*/
temp_vec = (mean_x1-mean_x2);
sb = temp_vec*temp_vec.transpose();
sw = cov_x1+cov_x2;

A = sw.ldlt().solve(sb);

cout << "Here is the mean of x1:\n" << mean_x1 << endl;
cout << "Here is the cov matx of x1:\n" << cov_x1 << endl;
cout << "Here is sb:\n" << sb << endl;
cout << "And sw:\n" << sw << endl;
cout << "And A:\n" << A << endl;

EigenSolver<MatrixXd> es(A);
if (es.info() != Success) abort();
cout << "The eigenvalues of A are:\n" << es.eigenvalues() << endl;
cout << "Here's a matrix whose columns are eigenvectors of A \n"
<< "corresponding to these eigenvalues:\n"
<< es.eigenvectors() << endl;

}
