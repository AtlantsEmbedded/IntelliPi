


#include "linear_algebra.h"



int main() {
    int n = 3;
    double *C;
    double *L;
    double *L_prime;
    double A[] = {25, 15, -5,
                   15, 18,  0,
                   -5,  0, 11};
    L = mtx_chol(A, n);
    show_matrix(L, n);
    printf("\n");
    L_prime = mtx_transpose(L, n, n);
    
    show_matrix(L_prime, n);
    printf("\n");
    
    C = mtx_mult(L, L_prime, n, n, n);
    
    show_matrix(C, n);
    printf("\n");
    
    free(L);
    free(L_prime);
    free(C);
 
#if 0
    n = 4;
    double m2[] = {18, 22,  54,  42,
                   22, 70,  86,  62,
                   54, 86, 174, 134,
                   42, 62, 134, 106};
    double *c2 = mtx_chol(m2, n);
    show_matrix(c2, n);
    free(c2);
 #endif
    return 0;
}




