/**
 * @file filt_downsamp.c
 * @author Frédéric Simard
 * @date December, 2014
 * @brief This file implements a filter-downsampler. The downsampling is of ratio
 *		  2 and the filter is a low-pass with a cutoff freq of 0.5 normalized nyquist frequency.
 *        The filter is the implmentation of an anti-aliasing mechanisms.
 *
 *        As an example, this filter was implemented to downsample a signal from 220Hz to 110Hz (ratio=2).
 *        The Nyquist frequency is of 110Hz and the cutoff freq of the filter is of 55Hz.
 *
 *        This implementation do not allow for customization, but can be used to downsample of a ratio 2
 *        any signals.
 *
 *        The filter's transfer function is given by:
 *
 *		  H(z) = (a0 + a1*z-1 + a2*z-2)/(bo + b1*z-1 + b2*z-2)
 *
 *        Our implementation uses the direct form 2.
 *        
 *        Y(n) = A[0]*X(n) + w(n-1)
 *        w(n-1) = A[1]*X(n) + w(n-2) + B[1]*Y(n) 
 *        w(n-2) = A[2]*X(n) + B[2]*Y(n)
 * 
 *        Parameters have been computed using:
 *        http://www.earlevel.com/main/2010/12/20/biquad-calculator/
 *
 */

/*parameters of the biquad filter*/
const float A[3] = {0.2928920553392428, 0.5857841106784856, 0.2928920553392428};
const float B[2] = {-1.3007020142696517e-16, 0.17156822135697122};


#include "filt_downsamp.h"


/**
 * filt_downsamp_signal(float *input_signal, int nb_samples, float *output_signal)
 * 
 * @brief filter and downsample an input signal and store it in the output signal
 * @param input_signal, input signal to be downsampled
 * @param output_signal, container for the downsampled signal, size must be at least half of input_signal
 */
void filt_downsamp_signal(float *input_signal, int nb_samples, float *output_signal)
{

	register int ii,jj=0;
	register float wn[2]; /*define a set of registers that hold delayed values*/
	register float cur_y = 0; /*define a register for current output*/
	
	/*computes the first two values */
	wn[0] = 0;
	wn[1] = 0;
	
	jj = 0;
 	
 	/*
 	 * Filter
	 * Y(n) = A[0]*X(n) + w(n-1)
	 * w(n-1) = A[1]*X(n) + w(n-2) + B[1]*Y(n) 
	 * w(n-2) = A[2]*X(n) + B[2]*Y(n)
	 * 
	 * where w(n-1)->wn[0]
	 * where w(n-2)->wn[1]
     */
	for(ii=0;ii<nb_samples;ii++){

		/*compute the next output value*/
		cur_y = A[0]*input_signal[ii]+wn[0];
		wn[0] = A[1]*input_signal[ii]+wn[1]-B[0]*cur_y;
		wn[1] = A[2]*input_signal[ii]-B[1]*cur_y;
		
		/*subsample it (one in two)*/						 
		if(ii%2 == 0){
			output_signal[jj] = cur_y;
			jj++;
		}
		
	}
}


