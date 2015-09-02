/**
 * @file main.c
 * @author Fred Simard (frederic.simard.1@outlook.com)
 * @copy wiringpi testbench for atlants embedded
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

#ifdef RASPI
#include <wiringPi.h>
#include <mcp23017.h>
#include <softTone.h>
#include <lcd.h>
#endif

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
