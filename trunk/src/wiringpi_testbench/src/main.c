/**
 * @file main.c
 * @author Fred Simard (frederic.simard.1@outlook.com)
 * @copy wiringpi testbench for atlants embedded
 */
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

/**
 * main(int argc, char **argv)
 * @brief Main function
 * @param argc
 * @param argv
 * @return 0 for success, -1 for error
 */
int main(int argc, char **argv)
{


	wiringPiSetup();

	pinMode (0, OUTPUT) ;
	for (;;)
	{
		digitalWrite (0, HIGH) ; delay (500) ;
		digitalWrite (0,  LOW) ; delay (500) ;
	}
	
	return 0 ;

}
