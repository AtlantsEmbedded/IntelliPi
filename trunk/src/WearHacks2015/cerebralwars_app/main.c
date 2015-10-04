

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
	
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} /*__attribute__((packed)) */pixel_t;


/**
 * main(int argc, char **argv)
 * @brief test the mindbx_lib
 */
int main(int argc, char **argv){
	
	int i, loop_count,start_count;
	int red, blue, green;
	int spi_driver;
	while(1){
	printf("Input the value of start count: ");
	scanf("%d", &start_count);
	printf("Input the value of end count: ");
	scanf("%d", &loop_count);
	printf("Input the Red intensity: ");
	scanf("%d", &red);
	printf("Input the Blue intensity: ");
	scanf("%d", &blue);
	printf("Input the Green intensity: ");
	scanf("%d", &green);
	
	/*define buffer*/
	pixel_t buffer[loop_count];
	
	/*set the whole array to 0*/
	memset(buffer,0,loop_count*sizeof(pixel_t));
	
	for(i=start_count;i<loop_count;i++){
		buffer[i].red = red;
		buffer[i].blue = blue;
		buffer[i].green = green;
	}
	
	
	/*configure the mind box*/
	spi_driver = open("/dev/spidev0.0",O_RDWR);
	write(spi_driver, buffer, loop_count*sizeof(pixel_t));
	
	close(spi_driver);	
	
}
	exit(0);
}

