#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>


/**
 * main(int argc, char **argv)
 * @brief test the mindbx_lib
 */
int main(int argc, char **argv){
	
	int i;
	
	/*configure the mind box*/
	
	
	wiringPiSPISetup(0,1000000);
	
	
	wiringPiSPIDataRW(0,0xF1,1);
	
	
	
	exit(0);
}

