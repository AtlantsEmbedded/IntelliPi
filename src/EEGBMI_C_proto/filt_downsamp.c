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
 *        Our implementation formulates it like this:
 *        
 *        Y(z) = 1/B[0] * (A[0]*X(z) + A[1]*X(z-1) + A[2]*X(z-2) - B[1]*Y(z-1) - B[2]*Y(z-2) )
 *
 *        Parameters have been computed using:
 *        http://www.earlevel.com/main/2010/12/20/biquad-calculator/
 *
 */

/*parameters of the biquad filter*/
const float num[3] = {0.33597142261668933, 0.6719428452333787, 0.33597142261668933};
const float denum[3] = {0.1764599748662749, 0.1674257156004825, 0};

#include "filt_downsamp.h"


/**
 * filt_downsamp_signal(float *input_signal, int nb_samples, float *output_signal)
 * 
 * @brief filter and downsample an input signal and store it in the output signal
 * @param input_signal, input signal to be downsampled
 * @param output_signal, container for the downsampled signal
 */
void filt_downsamp_signal(float *input_signal, int nb_samples, float *output_signal)
{

	register int ii,jj=0;
	register float temp[3]; /*define a set of registers that hold past values of the output in native SR*/
	/*temp[0] = z-2, temp[1] = z-1, temp[2] = z*/

	/*computes the first two values */
	temp[0] = 1/denum[0]*num[0]*input_signal[0];
	temp[1] = 1/denum[0] * (num[0]*input_signal[1] + num[1]*input_signal[0] - denum[1]*temp[0]);
	
	output_signal[0] = temp[0];
	jj = 1;
 	
 	/*
 	 * Filter
 	 * Y(z) = 1/B[0] * (A[0]*X(z) + A[1]*X(z-1) + A[2]*X(z-2) - B[1]*Y(z-1) - B[2]*Y(z-2) )	
     */
	for(ii=2;ii<nb_samples;ii++){

		/*compute the next output value*/
		temp[2] = 1/denum[0] * (num[0]*input_signal[ii]
								 + num[1]*input_signal[ii-1]
								 + num[2]*input_signal[ii-2] 
								 - denum[1]*temp[1] 
								 - denum[2]*temp[0]);
		
		/*subsample it (one in two)*/						 
		if(ii%2 == 0){
			output_signal[jj] = temp[2];
			jj++;
		}
		
		/*shift values back in time*/
		temp[0] = temp[1];
		temp[1] = temp[2];
	}
	
}


