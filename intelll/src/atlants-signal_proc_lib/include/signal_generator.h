#ifndef PINK_NOISE_H
#define PINK_NOISE_H

/**
 * void get_signal_pink(double* signal, int sample_length)
 * @brief generate a pink noise signal of length n. Wrapper on pink_noise by John Burkardt
 * @param signal (out), n-long array that will contain the signal
 * @param n (in), length of the signal
 */ 
void get_pink_signal(double* signal, int n);


/**
 * void get_signal_sin(double* signal, int sample_length, double norm_frequency, double diff_factor)
 * @brief generate a pure or diffused sinus signal of length n.
 * @param signal (out), n-long array that will contain the signal
 * @param n (in), length of the signal
 * @param norm_freq (in), central frequency, normalized to nyquist frequency of the signal
 * @param phase_disp (in), 0 return a pure sinus with a Dirac freq power function. 
 *                          >0 will increasingly add a brownian noise to the phase and disperse
 *                          the power around the central frequency
 */ 
void get_sinus_signal(double* signal, int n, double norm_freq, double phase_disp);



#endif
