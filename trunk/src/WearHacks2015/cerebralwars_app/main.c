

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

typedef struct pixel_s{
	uint16_t first_bit:1;
	uint16_t red:5;
	uint16_t green:5;
	uint16_t blue:5;
}pixel_t;


/**
 * main(int argc, char **argv)
 * @brief test the mindbx_lib
 */
int main(int argc, char **argv){
	
	int i;
	int spi_driver;
	
	/*define buffer*/
	pixel_t buffer[150];
	
	/*set the whole array to 0*/
	memset(buffer,0,150*sizeof(pixel_t));
	
	for(i=0;i<150;i+=2){
		buffer[i].red = 0x1F;
	}
	
	for(i=1;i<150;i+=2){
		buffer[i].green = 0x1F;
	}
	
	/*configure the mind box*/
	spi_driver = open("/dev/spidev0.0",O_RDWR);
	write(spi_driver, buffer, 150*sizeof(pixel_t));
	
	close(spi_driver);
	
	exit(0);
}

