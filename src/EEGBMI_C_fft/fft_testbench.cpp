/**
 * @file fft_testbench.c
 * @author Frédéric Simard
 * @date January, 2015
 * @brief This program test my specific needs for the fft. The fft implementation
 * satisfy my requirements in all but one respect. My data points are real-valued,
 * but the fft implementation perform the operation on complex data points.
 *
 * The naive approach is to pad the imaginary part with 0s. While this result in
 * a symmetric, but correct fft, it performs the operation on a 2*N long sequence, where
 * 2*N correspond to the length of the real-valued sequence.
 *
 * It is possible, however, to split the real-valued data to obtain a N long complex-sequence.
 * This approach greatly improve the efficiency of the operation, but require a few manipulations
 * before and after the fft to proceed this way. This program implements and test this approach.
 *
 * Frederic Simard, Atom Embedded, www.atomsproducts.com, 2015
 * based on: Matusiak R (2001) Implementing Fast Fourir Transform Algorithms of 
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
static void test_real_valued_fft(int n);
static void naive_dft(const double *inreal, const double *inimag, double *outreal, double *outimag, int inverse, int n);
static double log10_rms_err(const double *xreal, const double *ximag, const double *yreal, const double *yimag, int n);
static double *random_reals(int n);
static void *memdup(const void *src, size_t n);
static double *zero_reals(int n);

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
	
	test_real_valued_fft(200);
	
	//printf("\n");
	//printf("Max log err = %.1f\n", max_log_error);
	//printf("Test %s\n", max_log_error < -10 ? "passed" : "failed");
	return 0;
}

/**
 * test_real_valued_fft(int n)
 * 
 * @brief this function implements the test the fast-fourier transform for 2*N real-valued signal
 * @param n, the length of the fft to be computed. half the length of real-valued signal processes
 * @return none
 */ 
static void test_real_valued_fft(int n) {

	int i,k;
	double signal_avg = 0.0;
	double *real_valued_signal, *real_valued_signal2;
	double *inputreal, *inputimag;
	double *refoutreal, *refoutimag;
	double *X1_abs, *X2_abs;
	
	/*Create a real-valued signal g(n)*/
	real_valued_signal = zero_reals(n);
	real_valued_signal2 = zero_reals(n);
	
	for(k=0;k<n;k++){
		real_valued_signal[k] = sin(M_PI *4*k / n);
		real_valued_signal2[k] = sin(M_PI *8*k / n);
		signal_avg += real_valued_signal[k]/n;
	}
	
	for(k=0;k<n;k++){
		real_valued_signal[k] -= signal_avg;
	}
	
	/*
	 **************************************************
	 * Reference: naive DFT
	 * This test is performed by padding the imaginary part of the signal with 0s
	 **************************************************
	*/
	
	/*perform the 2*N naive-dft by padding the imaginary part with 0s*/
	inputreal = (double*)memdup(real_valued_signal, n* sizeof(double));
	inputimag = zero_reals(n);
	refoutreal = (double*)malloc(n * sizeof(double));
	refoutimag = (double*)malloc(n * sizeof(double));
	naive_dft(inputreal, inputimag, refoutreal, refoutimag, 0, n);
	
	for(k=0;k<n;k++){
		printf("%f\n",inputimag[k]);
	}
	
	/*Terminate the test*/
	free(inputreal);
	free(inputimag);
	
	/*
	 **************************************************
	 * Test: N-FFT on complex signal
	 * This test rearrange the data to perform the fft on a complex signal formed from
	 * the original real-valued signal.
	 **************************************************
	*/
	
	/*form the complex signal x(n) = g1(n)+j*g2(n)*/
	inputreal = (double*)malloc(n * sizeof(double));
	inputimag = (double*)malloc(n * sizeof(double));
	
	for(i=0;i<n;i++){
		inputreal[i] = real_valued_signal[i];
		inputimag[i] = real_valued_signal2[i];
	}
	
	//naive_dft(inputreal, inputimag, refoutreal, refoutimag, 0, n);
	
	/*run the N-fft*/
	X1_abs = (double*)malloc(n * sizeof(double));
	X2_abs = (double*)malloc(n * sizeof(double));
	
	abs_fft_2signals(inputreal,inputimag,X1_abs,X2_abs,n);
	
	for(k=0;k<n;k++){
		printf("%0.4f %0.4f %0.4f\n",sqrt(refoutreal[k]*refoutreal[k]+refoutimag[k]*refoutimag[k]),X1_abs[k],X2_abs[k]);
	}
	
	/*Compare the results*/
	//printf("fftsize=%4d  logerr=%5.1f\n", n, log10_rms_err(refoutreal, refoutimag, actualoutreal, actualoutimag, 2*n));
	
	free(real_valued_signal);
	
	free(inputreal);
	free(inputimag);
	free(refoutreal);
	free(refoutimag);
	free(X1_abs);
	free(X2_abs);
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
	double *result = (double*)malloc(n * sizeof(double));
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
	double *result = (double*)malloc(n * sizeof(double));
	int i;
	for (i = 0; i < n; i++)
		result[i] = 0;
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
	void *dest = (double*)malloc(n);
	if (dest != NULL)
		memcpy(dest, src, n);
	return dest;
}
