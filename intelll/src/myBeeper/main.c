/**
 * @file
 * @author
 * @date
 * @brief 
 */ 
#include <stdlib.h>
#include <stdio.h>
#include <fft.h>

/**
 * main()
 * @brief
 * @param
 * @param
 * @return
 */ 
int main(int argc, char **argv)
{

	size_t b;
	double j = 1.543605908721;
	double *i = &j;

	printf("\n example with zero_reals function:\n");

	printf("\n befor calling shared library value at pointer is: %f \n", *i);

	printf("\n\n I am going to call shared library\n\n");

	i = zero_reals(5);

	printf("\n new value generated is: %f \n", *i);

	return (0);
}
