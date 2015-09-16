/**
 * @file pink_noise.c
 * @author Frederic Simard (frederic.simard.1@outlook.com) | Atlants Embedded
 * @brief pink_noise signal generator. 
 *        Pink noise is a type of noise of which the power spectrum is proportional to 1/f.
 *        Many natural systems exhibits pink noise properties such as natural image movies,
 *        EEG recorded from brain activity. More information about the pink noise can be found here:
 * 		  http://www.scholarpedia.org/article/1/f_noise#Mathematics_of_1.2Ff_noise
 *        
 * 		  This specific implementation uses the Voss-McCartney algorithm:
 *        http://www.firstpr.com.au/dsp/pink-noise/
 * 
 *        This file implements a wrapper over pink_noise by John Burkardt,
 *        https://people.sc.fsu.edu/~jburkardt/c_src/pink_noise/pink_noise.html
 *        and is used and distributed under the LGPL license.
 */

# include <stdlib.h>
# include <stdio.h>

# include "signal_generator.h"

#define PINK_GEN_NB 31

void cdelay2 ( int D, int *q );
double ran1f ( int B, double u[], int q[] );
double ranh ( int D, double *u, int *q );
void wrap2 ( int M, int *q );

/**
 * void get_signal_pink(double* signal, int sample_length)
 * @brief generate a pink noise signal of length n. Wrapper on pink_noise by John Burkardt
 * @param signal (out), n-long array that will contain the signal
 * @param n (in), length of the signal
 */ 
void get_pink_signal(double* signal, int n)
{
	int i;
	int q[PINK_GEN_NB];
	double u[PINK_GEN_NB];
	
	/*generate a set of random numbers, with 0 mean*/
	for ( i = 0; i < PINK_GEN_NB; i++ ){
		
		u[i] = (double)rand()/(double)(RAND_MAX)-0.5;
		//2.0*(double)rand()/(double)(RAND_MAX)-1.0;
	}
	
	for ( i = 0; i < PINK_GEN_NB; i++ ){
		
		q[i] = 0;
	}

	for ( i = 0; i < n; i++ ){
		
		signal[i] = ran1f ( PINK_GEN_NB, u, q );
	}
	  
  
	return;
}

/*
  Purpose:

	CDELAY2 is a circular buffer implementation of M-fold delay.

  Example:

	Suppose we call CDELAY2 12 times, always with M = 3, and with
	Q having the input value 3 on the first call.  Q will go through
	the following sequence of values over the 12 calls:

	I   M  Qin  Qout

	1   3   3   2
	2   3   2   1
	3   3   1   0
	4   3   0   3
	5   3   3   2
	6   3   2   1
	7   3   1   0
	8   3   0   3
	9   3   3   2
   10   3   2   1
   11   3   1   0
   12   3   0   3

  Licensing:

	This code is distributed under the GNU LGPL license. 

  Modified:

	30 May 2010

  Author:

	Original C version by Sophocles Orfanidis.
	This C version by John Burkardt.

  Reference:

	Sophocles Orfanidis,
	Introduction to Signal Processing,
	Prentice-Hall, 1995,
	ISBN: 0-13-209172-0,
	LC: TK5102.5.O246.

  Parameters:

	Input, int M, the maximum value that Q can have.

	Input/output, int *Q, a counter which is decremented on every call.
	However, the value "after" 0 is M.  
*/
void cdelay2 ( int m, int *q )
{
	/*Decrement the offset.*/
	*q = *q - 1;
	/*Q = - 1 wraps to Q = M.*/
	wrap2 ( m, q );

	return;
}

/*
  Purpose:

    RAN1F is a 1/F random number generator.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    30 May 2010

  Author:

    Original C version by Sophocles Orfanidis.
    This C version by John Burkardt.

  Reference:

    Sophocles Orfanidis,
    Introduction to Signal Processing,
    Prentice-Hall, 1995,
    ISBN: 0-13-209172-0,
    LC: TK5102.5.O246.

  Parameters:

    Input, int B, the number of signals to combine.
    For this algorithm, B cannot be more than 31!

    Input/output, double U[B], the signals to combine.  It is expected
    that each of the initial values of U will be drawn from a distribution
    with zero mean.

    Input/output, int Q[B], a set of counters that determine when each
    entry of U is to be updated.

    Output, double RAN1F, the value.
*/
double ran1f ( int b, double u[], int q[] )
{
	int i;
	int j;
	double y;

	if ( 31 < b ){
		
		fprintf ( stderr, "\n" );
		fprintf ( stderr, "RAN1F - Fatal error!\n" );
		fprintf ( stderr, "  32 <= B, too many signals.\n" );
		exit ( 1 );
	}

	y = 0.0;
	j = 1;
	
	for ( i = 0; i < b; i++ ){
		y = y + ranh ( j, u+i, q+i );
		j = j * 2;
	}
	
	if ( 0 < b ){
		y = y / b;
	}

	return y;
}

/*
  Purpose:

    RANH is a hold random number generator of period D.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    31 May 2010

  Author:

    Original C version by Sophocles Orfanidis.
    This C version by John Burkardt.

  Reference:

    Sophocles Orfanidis,
    Introduction to Signal Processing,
    Prentice-Hall, 1995,
    ISBN: 0-13-209172-0,
    LC: TK5102.5.O246.

  Parameters:

    Input, int D, the hold period.  D must be at least 1.

    Input/output, double *U, a value to be held until Q has decremented
    to 0, when Q will be reset to D, and U will be randomly reset.

    Input/output, int *Q, a counter which is decremented by 1 on each call
    until reaching 0.

    Output, double RANH, the input value of U.
*/
double ranh ( int d, double *u, int *q )
{
	double y;

	if ( d < 1 ){
		
		fprintf ( stderr, "\n" );
		fprintf ( stderr, "RANH - Fatal error!\n" );
		fprintf ( stderr, "  D < 1.\n" );
		exit ( 1 );
	}
	
	/*Hold this sample for D calls.*/
	y = *u;
	
	/*Decrement Q and wrap mod D.*/
	cdelay2 ( d - 1, q );
	
	/*Every D calls, get a new U with zero mean.*/
	if ( *q == 0 )
	{
		*u = 2.0 * ( double ) rand ( ) / ( double ) ( RAND_MAX ) - 1.0;
	}
	return y;
}

/*
  Purpose:

    WRAP2 is a circular wrap of the pointer offset Q.

  Discussion:

    Input values of Q between 0 and M are "legal".
    Values of Q below 0 are incremented by M + 1 until they are legal.
    Values of Q above M are decremented by M + 1 until they become legal.
    The legal value is the output value of the function.

  Example:

    M  Qin  Qout

    3  -5   3
    3  -4   0
    3  -3   1
    3  -2   2
    3  -1   3
    3   0   0
    3   1   1
    3   2   2
    3   3   3
    3   4   0
    3   5   1
    3   6   2
    3   7   3
    3   8   0

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    30 May 2010

  Author:

    Original C version by Sophocles Orfanidis.
    This C version by John Burkardt.

  Reference:

    Sophocles Orfanidis,
    Introduction to Signal Processing,
    Prentice-Hall, 1995,
    ISBN: 0-13-209172-0,
    LC: TK5102.5.O246.

  Parameters:

    Input, int M, the maximum acceptable value for outputs.
    M must be at least 0.

    Input/output, int *Q, the value to be wrapped.
*/
void wrap2 ( int m, int *q ){

	if ( m < 0 ){
		
		fprintf ( stderr, "\n" );
		fprintf ( stderr, "WRAP2 - Fatal error!\n" );
		fprintf ( stderr, "  M < 0.\n" );
		exit ( 1 );
	}
	
	/*When Q = M + 1, it wraps to Q = 0.*/
	while ( m < *q ){ 
		
		*q = *q - m - 1;
	}
	
	/*When Q = - 1, it wraps to Q = M.*/
	while ( *q < 0 ){
		
		*q = *q + m + 1;
	}
	
	return;
}
