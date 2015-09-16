/**
 * @file simple_parametric_signals.c
 * @author Frederic Simard (frederic.simard.1@outlook.com) | Atlants Embedded
 * @brief This file implements a series of simple parametric signal generator.
 */

# include <stdlib.h>
# include <stdio.h>
# include <math.h>

# include "signal_generator.h"

double randn();

/**
 * void get_sinus_signal(double* signal, int sample_length, double norm_frequency, double diff_factor)
 * @brief generate a sinus signal of length n.
 * @param signal (out), n-long array that will contain the signal
 * @param n (in), length of the signal
 * @param norm_freq (in), central frequency, normalized to nyquist frequency (fs/2) of the signal ex: norm_freq=0.1 for 11Hz at fs=220Hz
 * @param phase_disp (in), =0 return a pure sinus with a Dirac freq power function. 
 *                          >0 will increasingly add a brownian noise to the phase and disperse
 *                          the power around the central frequency
 */ 
void get_sinus_signal(double* signal, int n, double norm_freq, double phase_disp){
	
	int i=0;
	double phase_noise = 0.0;
	
	if(phase_disp==0){
		
		for(i=0;i<n;i++){
			signal[i] = sin(norm_freq*M_PI*(double)i);
		}
	}
	else{
		for(i=0;i<n;i++){
			signal[i] = sin(norm_freq*M_PI*i+phase_noise);
			phase_noise += randn()*phase_disp*2*M_PI;
		}
	}
}


/**
 * double randn()
 * @brief Utility function to generate a normally distributed number
 */ 
double randn(){
	double Twopi = (6.2831853071795864769252867665590057683943387987502); /* 2 * pi */
	double randmax = RAND_MAX;
	double random = rand();
	double randnorm;
	
	/*Calculates a normally distributed random number*/
	random/=randmax;
	randnorm= (sqrt(-2.0*log(random)));
	randnorm*=cos(Twopi*random);
	return randnorm;
}
