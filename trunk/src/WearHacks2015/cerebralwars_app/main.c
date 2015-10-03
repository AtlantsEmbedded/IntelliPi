

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>


/**
 * main(int argc, char **argv)
 * @brief test the mindbx_lib
 */
int main(int argc, char **argv){
	
	int i;
	int spi_driver;
	
	unsigned char* buffer[3*150];
	
	memset(buffer,0,3*150);
	
	for(i=0;i<150;i+=2){
		buffer[i*3+0] = 0xFF;
	}
	
	for(i=1;i<150;i+=2){
		buffer[i*3+1] = 0x7F;
	}
	
	/*configure the mind box*/
	spi_driver = open("/dev/spidev0.0",O_RDWR);
	write(spi_driver, buffer, 3*150);
	
	close(spi_driver);
	
	exit(0);
}

