/* 
 * real-valued signal N long-FFT (C)
 *
 * This program test my specific needs for the fft. The fft implementation
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

static double max_log_error = -INFINITY;


/* Main and test functions */

int main(int argc, char **argv) {
	int i;
	int prev;
	srand(time(NULL));
	
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
	return 0;
}


static void test_real_valued_fft(int n) {

	int i,k;
	double *real_valued_signal;
	double *inputreal, *inputimag;
	double *refoutreal, *refoutimag;
	double *tempoutreal, *tempoutimag;
	double *actualoutreal, *actualoutimag;
	double *Ar, *Ai, *Br, *Bi;
	
	/*Create a real-valued signal g(n)*/
	real_valued_signal = random_reals(2*n);
	
	/*
	 **************************************************
	 * Reference: naive DFT
	 * This test is performed by padding the imaginary part of the signal with 0s
	 **************************************************
	*/
	
	/*perform the 2*N naive-dft by padding the imaginary part with 0s*/
	inputreal = memdup(real_valued_signal,2 * n);
	inputimag = zero_reals(2 * n);
	refoutreal = malloc(2 * n * sizeof(double));
	refoutimag = malloc(2 * n * sizeof(double));
	naive_dft(inputreal, inputimag, refoutreal, refoutimag, 0, 2*n);
	
	/*rearrange the result to get the true G(k)*/
	
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
	inputreal = malloc(n * sizeof(double));
	inputimag = malloc(n * sizeof(double));
	
	for(i=0;i<n;i++){
		inputreal(i) = real_valued_signal(2*i);
		inputimag(i) = real_valued_signal(2*i+1);
	}
	
	/*run the N-fft*/
	tempoutreal = memdup(inputreal, n * sizeof(double));
	tempoutimag = memdup(inputimag, n * sizeof(double));
	transform(tempoutreal, tempoutimag, n);
	
	/* Rearrange the result X(k) to get the desired G(k)
	 * Equation: G(k) = X(k)*A(k) + X*(N-k)B(k)
	 *
	 * which can be expressed as:
	 * Gr(k) = Xr(k)Ar(k) - Xi(k)Ai(k) + Xr(N-k)Br(k) + Xi(N-k)Bi(k) 
	 * Gi(k) = Xi(k)Ar(k) + Xr(k)Ai(k) + Xr(N-k)Bi(k) - Xi(N-k)Br(k)
	 * with k = 0,1,...,N-1 and X(N) = X(0)
	 *
	 * Gr(N) = Xr(0) - Xi(0)
	 * Gi(N) = 0
	 *
	 * Symmetric part of the fft
	 * Gr(2N-k) = Gr(k)
	 * Gi(2N-k) = Gi(k)
	 *
	 * With Ar, Ai, Br and Bi equal to:
	 * Ar(k) = –sin(pi*k/N)
	 * Ai(k) = –cos(pi*k/N)
	 * Br(k) = sin(pi*k/N)
	 * Bi(k) = cos(pi*k/N)
	 * with k=0->N-1
	*/
	/*allocate the memory for G(k)*/
	actualoutreal = malloc(2 * n * sizeof(double));
	actualoutimag = malloc(2 * n * sizeof(double));
	
	/*and for the A and B factors*/
	Ar = malloc(n * sizeof(double));
	Ai = malloc(n * sizeof(double));
	Br = malloc(n * sizeof(double));
	Bi = malloc(n * sizeof(double));
	
	/*Buid the Ar, Ai, Br and Bi tables*/
	for(k=1;k<n;k++){
		Ar = -sin(M_PI * k * n);
		Ai = -cos(M_PI * k * n);
		Br = sin(M_PI * k * n);
		Bi = cos(M_PI * k * n);
	}
	
	/*start with k = 0->N-1*/
	for(k=0;k<n;k++){		
		/* Gr(k) = Xr(k)Ar(k) 
				   - Xi(k)Ai(k) 
				   + Xr(N-k)Br(k) 
				   + Xi(N-k)Bi(k) */
		actualoutreal(k) =  tempoutreal(k)*Ar(k) 
							- tempoutimag(k)*Ai(k)
							+ tempoutreal(N-k)*Br(k)
							+ tempoutimag(N-k)*Bi(k);
							 
		/* Gi(k) = Xi(k)Ar(k) 
				   + Xr(k)Ai(k) 
				   + Xr(N-k)Bi(k) 
				   - Xi(N-k)Br(k)*/
		actualoutimag(k) =  tempoutimag(k)*Ar(k) 
							+ tempoutreal(k)*Ai(k)
							+ tempoutreal(N-k)*Bi(k)
							- tempoutimag(N-k)*Br(k);
	}
	
	/*then k = N->2*N*/
	actualoutreal(n) = tempoutreal(0)-tempoutrimag(0);
	actualoutimag(n) = 0;
	
	for(k=1;k<n;k++){		
		actualoutreal(2*n-k) = actualoutreal(k); 
		actualoutimag(2*n-k) = actualoutimag(k);
	}
	
	/*Compare the results*/
	printf("fftsize=%4d  logerr=%5.1f\n", n, log10_rms_err(refoutreal, refoutimag, actualoutreal, actualoutimag, n));
	
	free(real_valued_signal);
	free(Ar);
	free(Ai);
	free(Br);
	free(Bi);
	free(inputreal);
	free(inputimag);
	free(refoutreal);
	free(refoutimag);
	free(actualoutreal);
	free(actualoutimag);
}



/* Naive reference computation functions */

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


/* Utility functions */

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


static double *random_reals(int n) {
	double *result = malloc(n * sizeof(double));
	int i;
	for (i = 0; i < n; i++)
		result[i] = (rand() / (RAND_MAX + 1.0)) * 2 - 1;
	return result;
}

static double *zero_reals(int n) {
	double *result = malloc(n * sizeof(double));
	int i;
	for (i = 0; i < n; i++)
		result[i] = 0;
	return result;
}


static void *memdup(const void *src, size_t n) {
	void *dest = malloc(n);
	if (dest != NULL)
		memcpy(dest, src, n);
	return dest;
}
