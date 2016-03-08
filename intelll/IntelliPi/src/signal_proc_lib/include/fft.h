/* 
 * Free FFT and convolution (C)
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

/**
 * int fft_2signals(int n)
 * 
 * @brief this function implements the complex fast-fourier transform of 2 real-valued signal at the same time.
 * This is achieved by packing one signal in the real domain and the other in the imaginary part.
 * @param signal1 (in), the first signal.
 * @param signal2 (in), the second signal.
 * @param X1_real (out), the real part of the fft of the first signal.
 * @param X1_imag (out), the imaginary part of the fft of the first signal.
 * @param X2_real (out), the real part of the fft of the second signal.
 * @param X2_imag (out), the imaginary part of the fft of the second signal.
 * @param n, the length of the fft to be computed.
 * @return none
 */ 
int fft_2signals(double* signal_1, double* signal_2, 
                 double* X1_real, double* X1_imag, 
                 double* X2_real, double* X2_imag,
                 size_t n);
                 

/**
 * int abs_fft(int n)
 * 
 * @brief this function implements the fast-fourier transform of a signal and return the absolute values
 * of the one-sided fft.
 * @param signal (in), the n-long signal.
 * @param abs_onesided_fft (out), abs value of one-sided fft of the signal(length n/2+1).
 * @param n, the length of the signal to be computed.
 * @return 1 if success, 0 otherwise
 */                
int abs_fft(double* signal,
            double* abs_onesided_fft, 
            size_t n);                 
                 

/**
 * int abs_fft_2signals(int n)
 * 
 * @brief this function implements the fast-fourier transform of 2 real-valued signal at the same time
 * and returns the absolute values of the one-sided fft transform of each signals.
 * @param signal1 (in), the first signal.
 * @param signal2 (in), the second signal.
 * @param X1 (out), abs value of one-sided fft of signal 1 (length n/2+1).
 * @param X2 (out), abs value of one-sided fft of signal 2 (length n/2+1).
 * @param n, the length of the fft to be computed.
 * @return none
 */                
int abs_fft_2signals(double* signal_1, double* signal_2, 
                 double* X1, 
                 double* X2,
                 size_t n);
                 
                 
                 

/**
 * void get_signal_sin(double* signal, int sample_length, double norm_frequency, double diff_factor)
 * @brief Returns customary information about a fft transform, given a signal length and a sampling frequency.
 * @param freq_scale (out), n/2+1-long array that will contain the signal
 * @param df (out), delta between two adjancent frequencies
 * @param n (in), length of the signal that was ffted
 * @param fs (in), sampling frequency
 */ 
void get_fft_infos(double* freq_scale, double* df, int n, double fs);
                 
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
void naive_dft(const double *inreal, const double *inimag, double *outreal, double *outimag, int inverse, int n);                 

/* 
 * Computes the discrete Fourier transform (DFT) of the given complex vector, storing the result back into the vector.
 * The vector can have any length. This is a wrapper function. Returns 1 (true) if successful, 0 (false) otherwise (out of memory).
 */
int transform(double real[], double imag[], size_t n);

/* 
 * Computes the inverse discrete Fourier transform (IDFT) of the given complex vector, storing the result back into the vector.
 * The vector can have any length. This is a wrapper function. This transform does not perform scaling, so the inverse is not a true inverse.
 * Returns 1 (true) if successful, 0 (false) otherwise (out of memory).
 */
int inverse_transform(double real[], double imag[], size_t n);

/* 
 * Computes the discrete Fourier transform (DFT) of the given complex vector, storing the result back into the vector.
 * The vector's length must be a power of 2. Uses the Cooley-Tukey decimation-in-time radix-2 algorithm.
 * Returns 1 (true) if successful, 0 (false) otherwise (n is not a power of 2, or out of memory).
 */
int transform_radix2(double real[], double imag[], size_t n);

/* 
 * Computes the discrete Fourier transform (DFT) of the given complex vector, storing the result back into the vector.
 * The vector can have any length. This requires the convolution function, which in turn requires the radix-2 FFT function.
 * Uses Bluestein's chirp z-transform algorithm. Returns 1 (true) if successful, 0 (false) otherwise (out of memory).
 */
int transform_bluestein(double real[], double imag[], size_t n);

/* 
 * Computes the circular convolution of the given real vectors. Each vector's length must be the same.
 * Returns 1 (true) if successful, 0 (false) otherwise (out of memory).
 */
int convolve_real(const double x[], const double y[], double out[], size_t n);

/* 
 * Computes the circular convolution of the given complex vectors. Each vector's length must be the same.
 * Returns 1 (true) if successful, 0 (false) otherwise (out of memory).
 */
int convolve_complex(const double xreal[], const double ximag[], const double yreal[], const double yimag[], double outreal[], double outimag[], size_t n);


double *zero_reals(int n);
void *memdup(const void *src, size_t n);
