/**
 * @file 2_signals_1fft.c
 * @author Fr�d�ric Simard
 * @date January, 2015
 * @brief This program illustrate how we can compute the fft of two signals
 * as a single fft operation. To do this, the two signals are combined together
 * one as the real part of the signal to be ffted, and one as the imaginary part:
 * 
 * x(n) = x1(n)+ix2(n)
 *
 * Then we compute the fft
 * 
 * X(k) = fft(x(n))
 * 
 * And finally we need to manipulate the results to recover the fft of the two signals
 *
 * X1(k) = 1/2*[X(k)+X*(N-k)]
 * X2(k) = 1/(j2)*[X(k)-X*(N-k)]
 * 
 * Frederic Simard, Atom Embedded, www.atomsproducts.com, 2015
 * based on: Matusiak R (2001) Implementing Fast Fourier Transform Algorithms of 
 * Real-Valued Sequences With the TMS320 DSP Platotform. Application Report. SPRA291
 *
 * See below for original copyrights and licensing details of the fft and convolution test library.
 *
 * Copyright (c) 2014 Project Nayuki
 * http://www.nayuki.io/page/free-small-fft-in-multiple-languages
 * 
 * (MIT License)
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * - The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * - The Software is provided "as is", without warranty of any kind, express or
 *   implied, including but not limited to the warranties of merchantability,
 *   fitness for a particular purpose and noninfringement. In no event shall the
 *   authors or copyright holders be liable for any claim, damages or other
 *   liability, whether in an action of contract, tort or otherwise, arising from,
 *   out of or in connection with the Software or the use or other dealings in the
 *   Software.
 */ 

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "fft.h"


// Private function prototypes
static void test_2signals_fft(int n);
static void naive_dft(const double *inreal, const double *inimag, double *outreal, double *outimag, int inverse, int n);
static double log10_rms_err(const double *xreal, const double *ximag, const double *yreal, const double *yimag, int n);
static double *random_reals(int n);
static void *memdup(const void *src, size_t n);
static double *zero_reals(int n);

static double amplitude(double real, double imag);

static double max_log_error = -INFINITY;


/**
 * main(int argc, char **argv[])
 * 
 * @brief this program test the fast-fourier transform for 
 *        2*N real-valued signal, under various conditions
 * @param argc
 * @param argv
 * @return 0 for success, non-zero for failure
 */ 
 int main(int argc, char **argv) {
	int i;
	int prev;
	srand(time(NULL));
	
	test_2signals_fft(128);
	
#if 0
	/*Test power-of-2 size FFTs*/
	/*from N=2^0 to 2^12*/
	for (i = 0; i <= 12; i++)
		test_real_valued_fft(1 << i);
	
	/*Test small size FFTs*/
	/*from N=0 to 30*/
	for (i = 0; i < 30; i++)
		test_real_valued_fft(i);
	
	// Test diverse size FFTs
	prev = 0;
	for (i = 0; i <= 100; i++) {
		int n = (int)lround(pow(1500, i / 100.0));
		if (n > prev) {
			test_real_valued_fft(n);
			prev = n;
		}
	}

	
	printf("\n");
	printf("Max log err = %.1f\n", max_log_error);
	printf("Test %s\n", max_log_error < -10 ? "passed" : "failed");
#endif
	return 0;

}

/**
 * test_2signals_fft(int n)
 * 
 * @brief this function implements the test the complex fast-fourier transform of 2 real-valued signal at the same time.
 * This is achieved by packing one signal in the real domain and the other in the imaginary part.
 * @param n, the length of the fft to be computed.
 * @return none
 */ 
static void test_2signals_fft(int n) {

	int i,k;
	double *signal_1, *signal_2;
	double *inputreal, *inputimag;
	double *X1ref_real, *X1ref_imag;
	double *X2ref_real, *X2ref_imag;
	
	double *signal_1_cpy, *signal_2_cpy;
	double *X1_real, *X1_imag;
	double *X2_real, *X2_imag;
	
	/*Create a real-valued signal g(n)*/
	signal_1 = malloc(n * sizeof(double));
	signal_2 = malloc(n * sizeof(double));
	
	for(k=0;k<n;k++){
		signal_1[k] = sin(M_PI * 4 * k / n);
		signal_2[k] = sin(M_PI * 10 * k / n);
	}
		
	/*
	 **************************************************
	 * Reference: naive DFT
	 * This test is performed by padding the imaginary part of the signal with 0s
	 **************************************************
	*/

	/*perform the 2*N naive-dft by padding the imaginary part with 0s*/
	inputreal = memdup(signal_1, n * sizeof(double));
	inputimag = zero_reals(n);
	X1ref_real = malloc(n * sizeof(double));
	X1ref_imag = malloc(n * sizeof(double));
	naive_dft(inputreal, inputimag, X1ref_real, X1ref_imag, 0, n);
	
	inputreal = memdup(signal_2, n * sizeof(double));
	X2ref_real = malloc(n * sizeof(double));
	X2ref_imag = malloc(n * sizeof(double));
	naive_dft(inputreal, inputimag, X2ref_real, X2ref_imag, 0, n);
	
	/*Terminate the test*/
	free(inputreal);
	free(inputimag);
	
	/*
	 **************************************************
	 * Test: complex FFT on 2 signals
	 * This test rearrange the data to perform the fft on a complex signal formed from
	 * the two original signals
	 **************************************************
	*/
	
	/*form the complex signal x(n) = x1(n)+j*x2(n)*/
	/*The same buffer is used as input/output*/
	signal_1_cpy = malloc(n * sizeof(double));
	signal_2_cpy = malloc(n * sizeof(double));
	
	for(i=0;i<n;i++){
		signal_1_cpy[i] = signal_1[i];
		signal_2_cpy[i] = signal_2[i];
	}
	
	X1_real = malloc(n * sizeof(double));
	X1_imag = malloc(n * sizeof(double));
	X2_real = malloc(n * sizeof(double));
	X2_imag = malloc(n * sizeof(double));
	
	fft_2signals(signal_1_cpy, signal_2_cpy, 
                 X1_real, X1_imag, 
                 X2_real, X2_imag,
                 n);
			
	printf("X1 ref\t X1\t X2 ref\t X2\n");		
			
	for(k=0;k<n;k++){
		printf("%0.3f\t %0.3f\t %0.3f\t %0.3f\n",amplitude(X1ref_real[k],X1ref_imag[k]),
												 amplitude(X1_real[k],X1_imag[k]),
												 amplitude(X2ref_real[k],X2ref_imag[k]),
												 amplitude(X2_real[k],X2_imag[k]));
	}
	
	
	/*Compare the results*/
	printf("Signal 1: fftsize=%4d  logerr=%5.1f\n", n, log10_rms_err(X1ref_real, X1ref_imag, X1_real, X1_imag, n));
	printf("Signal 2: fftsize=%4d  logerr=%5.1f\n", n, log10_rms_err(X2ref_real, X2ref_imag, X2_real, X2_imag, n));
	
	free(signal_1);
	free(signal_2);
	free(X1_real);
	free(X2_real);
	free(X1_imag);
	free(X2_imag);
	free(X1ref_real);
	free(X2ref_real);
	free(X1ref_imag);
	free(X2ref_imag);
	free(signal_1_cpy);
	free(signal_2_cpy);

}



/**
 * naive_dft(const double *inreal, const double *inimag, double *outreal, double *outimag, int inverse, int n)
 * 
 * @brief this function implements the reference naive fourier transform, by using the true definition.
  *       It is not optimal, but gives the correct result (this function was implemented by the original author)
 * @param inreal, inimag, complex signal to be processed
 * @param outreal, outimag, fourier transform of the signal
 * @param inverse, 1 to compute inverse transform, 0 for the forward transform
 * @param n, length of the signal
 * @return none
 */ 
static void naive_dft(const double *inreal, const double *inimag, double *outreal, double *outimag, int inverse, int n) {
	double coef = (inverse ? 2 : -2) * M_PI;
	int k;
	for (k = 0; k < n; k++) {  // For each output element
		double sumreal = 0;
		double sumimag = 0;
		int t;
		for (t = 0; t < n; t++) {  // For each input element
			double angle = coef * ((long long)t * k % n) / n;
			sumreal += inreal[t]*cos(angle) - inimag[t]*sin(angle);
			sumimag += inreal[t]*sin(angle) + inimag[t]*cos(angle);
		}
		outreal[k] = sumreal;
		outimag[k] = sumimag;
	}
}


/**
 * log10_rms_err(const double *xreal, const double *ximag, const double *yreal, const double *yimag, int n)
 * 
 * @brief this function implements the root mean square error criterion to compare two signals.
  *       (this function was implemented by the original author)
 * @param xreal, ximag, complex signal 1
 * @param yreal, yimag, complex signal 2
 * @param n, length of the signal
 * @return the rms of the difference between the two signals
 */ 
static double log10_rms_err(const double *xreal, const double *ximag, const double *yreal, const double *yimag, int n) {
	double err = 0;
	int i;
	for (i = 0; i < n; i++)
		err += (xreal[i] - yreal[i]) * (xreal[i] - yreal[i]) + (ximag[i] - yimag[i]) * (ximag[i] - yimag[i]);
	
	err /= n > 0 ? n : 1;
	err = sqrt(err);  // Now this is a root mean square (RMS) error
	err = err > 0 ? log10(err) : -99.0;
	if (err > max_log_error)
		max_log_error = err;
	return err;
}


/**
 * random_reals(int n)
 * 
 * @brief this function returns a vector of real elements.
  *       (this function was implemented by the original author)
 * @param n, length of the signal
 * @return 1 if success, 0 otherwise
 */ 
static double *random_reals(int n) {
	double *result = malloc(n * sizeof(double));
	int i;
	for (i = 0; i < n; i++)
		result[i] = (rand() / (RAND_MAX + 1.0)) * 2 - 1;
	return result;
}

/**
 * zero_reals(int n)
 * 
 * @brief this function returns a vector of 0s.
 * @param n, length of the signal
 * @return 1 if success, 0 otherwise
 */ 
static double *zero_reals(int n) {
	double *result = malloc(n * sizeof(double));
	int i;
	for (i = 0; i < n; i++)
		result[i] = 0.0;
	return result;
}


/**
 * memdup(int n)
 * 
 * @brief this function duplicates a memory array, size and values.
  *       (this function was implemented by the original author)
 * @param src, data to be duplicated
 * @param n, length of the signal
 * @return 1 if success, 0 otherwise
 */ 
static void *memdup(const void *src, size_t n) {
	void *dest = malloc(n);
	if (dest != NULL)
		memcpy(dest, src, n);
	return dest;
}

static double amplitude(double real, double imag){
	return sqrt(real*real+imag*imag);
}
