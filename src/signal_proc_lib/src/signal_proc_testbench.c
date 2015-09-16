
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "signal_generator.h"
#include "fft.h"


#define NB_REPEATS 100
#define SIGNAL_LENGTH 220
#define SAMPLING_FREQ 220

static void test_real_valued_fft(int n);
static void test_signal_generator();

int main ( void )
{
	
	//test_real_valued_fft(SIGNAL_LENGTH);
	test_signal_generator();

	return 0;
}




static void test_signal_generator(){
	
	int i,j=0;
	int onesided_fft_length = (SIGNAL_LENGTH/2+1);
	double* signal1 = (double*)malloc(sizeof(double)*SIGNAL_LENGTH);
	double* signal2 = (double*)malloc(sizeof(double)*SIGNAL_LENGTH);
	double* fft_test1 = (double*)malloc(sizeof(double)*(SIGNAL_LENGTH/2+1));
	double* fft_test2 = (double*)malloc(sizeof(double)*(SIGNAL_LENGTH/2+1));
	double* fft_test3 = (double*)malloc(sizeof(double)*(SIGNAL_LENGTH/2+1));
	double* fft_test4 = (double*)malloc(sizeof(double)*(SIGNAL_LENGTH/2+1));
	double* fft_temp = (double*)malloc(sizeof(double)*(SIGNAL_LENGTH/2+1));
	double temp_sum = 0.0;
	double norm_freq;
	double* freq_scale;
	double df;
	
	
	/*
	 * Test 1: pure sinus (10Hz)
	 */
	norm_freq = (2*10)/(double)SAMPLING_FREQ; 
	/*generate signal*/
	get_sinus_signal(signal1, SIGNAL_LENGTH, norm_freq, 0);
	
	/*remove DC*/
	temp_sum = 0.0;
	for(i=0;i<SIGNAL_LENGTH;i++){
		temp_sum += signal1[i];
	}
	
	temp_sum /= SIGNAL_LENGTH;
	
	for(i=0;i<SIGNAL_LENGTH;i++){
		signal1[i] -= temp_sum;
	}
	
	/*compute fft*/	
	abs_fft(signal1,fft_test1,SIGNAL_LENGTH);
	
	/*
	 * Test 2: pink noise 
	 */
	
	for(j=0;j<onesided_fft_length;j++){
		fft_test2[j] = 0;
	} 
	 
	/*generate the required number of trials*/
	for(i=0;i<NB_REPEATS;i++){
		
		/*generate a signal*/
		get_pink_signal(signal1, SIGNAL_LENGTH);
		
		/*remove DC*/
		temp_sum = 0.0;
		for(j=0;j<SIGNAL_LENGTH;j++){
			temp_sum += signal1[j];
		}
		
		temp_sum /= SIGNAL_LENGTH;
		
		for(j=0;j<SIGNAL_LENGTH;j++){
			signal1[j] -= temp_sum;
		}
		
		/*compute fft*/
		abs_fft(signal1,fft_temp,SIGNAL_LENGTH);

		/*sum up to compute average fft*/
		for(j=0;j<onesided_fft_length;j++){
			fft_test2[j] += fft_temp[j];
		}
	}

	/*average fft*/
	for(j=0;j<onesided_fft_length;j++){
		fft_test2[j] /= NB_REPEATS;
	}
	 
	/*
	 * Test 3: pink noise + pure sinus (10Hz)
	 */
	 
	for(j=0;j<onesided_fft_length;j++){
		fft_test3[j] = 0;
	} 
	 
	/*generate the required number of trials*/
	for(i=0;i<NB_REPEATS;i++){
		
		/*generate a signal*/
		get_pink_signal(signal1, SIGNAL_LENGTH);
		get_sinus_signal(signal2, SIGNAL_LENGTH, norm_freq, 0);
		
		/*remove DC*/
		temp_sum = 0.0;
		for(j=0;j<SIGNAL_LENGTH;j++){
			signal1[j] += signal2[j];
			temp_sum += signal1[j];
		}
		
		temp_sum /= SIGNAL_LENGTH;
		
		for(j=0;j<SIGNAL_LENGTH;j++){
			signal1[j] -= temp_sum;
		}
		
		/*compute fft*/
		abs_fft(signal1,fft_temp,SIGNAL_LENGTH);

		/*sum up to compute average fft*/
		for(j=0;j<onesided_fft_length;j++){
			fft_test3[j] += fft_temp[j];
		}
	}

	/*average fft*/
	for(j=0;j<onesided_fft_length;j++){
		fft_test3[j] /= NB_REPEATS;
	}
	 
	/*
	 * Test 4: pink noise + dispersed sinus (~10Hz)
	 */
	 
	for(j=0;j<onesided_fft_length;j++){
		fft_test4[j] = 0;
	} 
	 
	/*generate the required number of trials*/
	for(i=0;i<NB_REPEATS;i++){
		
		/*generate a signal*/
		get_pink_signal(signal1, SIGNAL_LENGTH);
		get_sinus_signal(signal2, SIGNAL_LENGTH, norm_freq, 0.025);
		
		/*remove DC*/
		temp_sum = 0.0;
		for(j=0;j<SIGNAL_LENGTH;j++){
			signal1[j] += signal2[j];
			temp_sum += signal1[j];
		}
		
		temp_sum /= SIGNAL_LENGTH;
		
		for(j=0;j<SIGNAL_LENGTH;j++){
			signal1[j] -= temp_sum;
		}
		
		/*compute fft*/
		abs_fft(signal1,fft_temp,SIGNAL_LENGTH);

		/*sum up to compute average fft*/
		for(j=0;j<onesided_fft_length;j++){
			fft_test4[j] += fft_temp[j];
		}
	}

	/*average fft*/
	for(j=0;j<onesided_fft_length;j++){
		fft_test4[j] /= NB_REPEATS;
	}
	
	/*get information about the fft*/
	freq_scale = (double*)malloc(onesided_fft_length * sizeof(double));
	get_fft_infos(freq_scale, &df, SIGNAL_LENGTH, SAMPLING_FREQ);
	
	printf("\n\n");
	printf("Test: signal generator\n");
	printf("Test1: pure sinus 10 Hz\n");
	printf("Test2: pink noise\n");
	printf("Test3: pink noise + pure sinus 10 Hz\n");
	printf("Test4: pink noise + dispersed sinus ~10 Hz\n");
	printf("\n");
	
	printf("Test conditions:\n");
	printf("Sampling frequency: %i (Hz)\n",SAMPLING_FREQ);
	printf("Nb of samples: %i \n",SIGNAL_LENGTH);
	printf("Recording length: %i (ms)\n",SIGNAL_LENGTH/SAMPLING_FREQ*1000);
	
	printf("\n");
	printf("freq(Hz)\ttest1 \ttest2 \ttest3 \ttest4\n");
	
	for(i=0;i<onesided_fft_length;i++){
		printf("%0.1f \t\t%0.3f \t%0.3f \t%0.3f \t%0.3f\n",freq_scale[i],fft_test1[i],fft_test2[i],fft_test3[i],fft_test4[i]);
	}
	
	free(signal1);
	free(signal2);
	free(fft_test1);
	free(fft_test2);
	free(fft_test3);
	free(fft_test4);
	free(fft_temp);
	free(freq_scale);
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
	double signal1_avg = 0.0;
	double signal2_avg = 0.0;
	double *real_valued_signal1, *real_valued_signal2;
	double *inputreal, *inputimag;
	double *refoutreal, *refoutimag;
	double *signal1_abs_fft_ref, *signal2_abs_fft_ref;
	double *signal1_abs_fft, *signal2_abs_fft;
	double* freq_scale;
	double df;
	int onesided_fft_length = n/2+1;
	
	/*Create a real-valued signal g(n)*/
	real_valued_signal1 = zero_reals(n);
	real_valued_signal2 = zero_reals(n);
	
	/*generate two signals to be ffted*/
	for(k=0;k<n;k++){
		real_valued_signal1[k] = sin(2*M_PI *4*k / n); /*first one is a sinus of 4Hz*/
		real_valued_signal2[k] = sin(2*M_PI *8*k / n); /*second one is a sinus of 8Hz*/
		signal1_avg += real_valued_signal1[k]; /*sums up signals to remove DC component*/
		signal2_avg += real_valued_signal2[k]; /*sums up signals to remove DC component*/
	}
	/*compute average of the signal*/
	signal1_avg /= n;
	signal2_avg /= n;
	
	/*remove average*/
	for(k=0;k<n;k++){
		real_valued_signal1[k] -= signal1_avg;
		real_valued_signal2[k] -= signal2_avg;
	}
	
	/***************************************************
	 * Reference: naive DFT
	 ***************************************************/
	signal1_abs_fft_ref = (double*)malloc(onesided_fft_length * sizeof(double));
	signal2_abs_fft_ref = (double*)malloc(onesided_fft_length * sizeof(double));
	
	/*perform the naive-dft of the first signal*/
	inputreal = (double*)memdup(real_valued_signal1, n*sizeof(double));
	/*pad the imaginary part with 0s*/
	inputimag = zero_reals(n);
	
	/*allocate memory for complex output*/
	refoutreal = (double*)malloc(n*sizeof(double));
	refoutimag = (double*)malloc(n*sizeof(double));
	
	/*perform naive DFT*/
	naive_dft(inputreal, inputimag, refoutreal, refoutimag, 0, n);
	
	/*compute absolute values of one-sided fft*/
	for(k=0;k<onesided_fft_length;k++){
		signal1_abs_fft_ref[k] = 2*(sqrt(refoutreal[k]*refoutreal[k]+refoutimag[k]*refoutimag[k])/n);
	}
	
	/*Terminate the test*/
	free(inputreal);
	
	/*Perform the naive fft of the second signal in the same way*/
	inputreal = (double*)memdup(real_valued_signal2, n*sizeof(double));
	naive_dft(inputreal, inputimag, refoutreal, refoutimag, 0, n);
	
	for(k=0;k<onesided_fft_length;k++){
		signal2_abs_fft_ref[k] = 2*(sqrt(refoutreal[k]*refoutreal[k]+refoutimag[k]*refoutimag[k])/n);
	}
	
	/*Clean up what we dont need*/
	free(inputreal);
	free(inputimag);
	free(refoutreal);
	free(refoutimag);
	
	/***************************************************
	 * Reference: test one-sided absolute values fft of two signals
	 ***************************************************/
	
	/*form the complex signal x(n) = g1(n)+j*g2(n)*/
	inputreal = (double*)malloc(n * sizeof(double));
	inputimag = (double*)malloc(n * sizeof(double));
	
	for(i=0;i<n;i++){
		inputreal[i] = real_valued_signal1[i];
		inputimag[i] = real_valued_signal2[i];
	}
	
	//naive_dft(inputreal, inputimag, refoutreal, refoutimag, 0, n);
	
	/*run the N-fft*/
	signal1_abs_fft = (double*)malloc(onesided_fft_length * sizeof(double));
	signal2_abs_fft = (double*)malloc(onesided_fft_length * sizeof(double));
	
	abs_fft_2signals(inputreal,inputimag,signal1_abs_fft,signal2_abs_fft,n);
	
	/*get information about the fft*/
	freq_scale = (double*)malloc(onesided_fft_length * sizeof(double));
	get_fft_infos(freq_scale, &df, n, SAMPLING_FREQ);
	
	/*present the test report*/
	
	printf("\n\n");
	printf("Test: FFT of two signals\n");
	printf("\n");
	
	printf("Test conditions:\n");
	printf("Sampling frequency: %i (Hz)\n",SAMPLING_FREQ);
	printf("Nb of samples: %i \n",n);
	printf("Recording length: %i (ms)\n",n/SAMPLING_FREQ*1000);
	
	printf("\n");
	printf("freq(Hz)\tref1 \tout1 \tref2 \tout2\n");
	
	for(k=0;k<onesided_fft_length;k++){
		printf("%0.1f \t\t%0.3f \t%0.3f \t%0.3f \t%0.3f\n",freq_scale[k],signal1_abs_fft_ref[k],signal1_abs_fft[k],signal2_abs_fft_ref[k],signal2_abs_fft[k]);
	}
	
	
	/*clean up*/
	free(inputreal);
	free(inputimag);
	free(real_valued_signal1);
	free(real_valued_signal2);
	free(signal1_abs_fft);
	free(signal2_abs_fft);
	free(signal1_abs_fft_ref);
	free(signal2_abs_fft_ref);
	free(freq_scale);
}










