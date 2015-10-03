

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
	
	unsigned char* buffer = {0xff,0xaa,0xff,0xaa,0xff,0xaa};
	
	/*configure the mind box*/
	spi_driver = open("/dev/spidev0.0",O_RDWR);
	size_t write(spi_driver, buffer, 6);
	
	close(spi_driver);
	
	exit(0);
}

