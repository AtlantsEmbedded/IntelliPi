

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
#include <pthread.h>

#include "led_strip_lib.h"

#define NB_LEDS 149   
#define PARTICLE_LENGTH 4
#define RED 0
#define GREEN 1
#define BLUE 2

#define BEGIN 0
#define END 1

#define RED_UPDATE_PERIOD 8
#define BLUE_UPDATE_PERIOD 8

const unsigned char particle_kernel[PARTICLE_LENGTH] = {0, 25, 50, 150};
const unsigned char explosion_kernel[8] = {0, 10, 25, 100, 100, 25, 10, 0};
const float explosion_animation_kernel[8] = {0.1, 0.3, 0.5, 0.7, 0.7, 0.5, 0.3, 0.1};

typedef struct pixel_s{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}pixel_t;

static pixel_t buffer[NB_LEDS];
static int spi_driver;
static int red_update_period = 8;
static int blue_update_period = 1;
static int explosion_location = NB_LEDS/2;
static int red_update_counter = RED_UPDATE_PERIOD;
static int blue_update_counter = BLUE_UPDATE_PERIOD;
static unsigned char particle_counter[2] = {0x00,0x00};
static unsigned char particle_color[2] = {0x00,0x00};


unsigned char led_strip_alive = 0x00;
pthread_t led_strip_thread;

void* led_strip_task(void* param);



void init_led_strip(){
	
	pthread_attr_t attr;
	
	static uint32_t speed = 1000000;
	
	spi_driver = open("/dev/spidev0.0",O_RDWR);
	ioctl(spi_driver, SPI_IOC_WR_MAX_SPEED_HZ, &speed);	
	
	memset(buffer,0,sizeof(pixel_t)*NB_LEDS);
	
	led_strip_alive = 0x01;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_create(&led_strip_thread, &attr, led_strip_task, NULL);
	pthread_attr_destroy(&attr);
	
}

void set_led_strip_values(int red_player_period, int blue_player_period, int contact_location){
		
	red_update_period = red_player_period;
	blue_update_period = blue_player_period;
	explosion_location = contact_location;
}

void* led_strip_task(void* param){
	
	/*define buffer*/
	int i;
	int address;
	int explosion_delay = (NB_LEDS/2)*RED_UPDATE_PERIOD;
	
	while(led_strip_alive){
		
		if(red_update_counter<=0){
			red_update_counter = red_update_period;
			
			/*from the start to explosion*/
			for(i=explosion_location;i>=0;i--){
				buffer[i+1].red = buffer[i].red;
				buffer[i+1].green = buffer[i].green;
				buffer[i+1].blue = buffer[i].blue;
			}
			
			/*check if a particle is being placed at the beginning*/
			if(particle_counter[BEGIN]>0){
				
				switch(particle_color[BEGIN]){
					
					case RED:
						buffer[0].red = particle_kernel[particle_counter[BEGIN]];
						buffer[0].green = 0;
						buffer[0].blue = 0;
						break;
					case GREEN:
						buffer[0].red = 0;
						buffer[0].green = particle_kernel[particle_counter[BEGIN]];
						buffer[0].blue = 0;
						break;
					case BLUE:
						buffer[0].red = 0;
						buffer[0].green = 0;
						buffer[0].blue = particle_kernel[particle_counter[BEGIN]];
						break;
				
				}
				particle_counter[BEGIN]--;
			}else{
		
				buffer[0].red = 0;
				buffer[0].green = 0;
				buffer[0].blue = 0;
				
				/*else roll a dice to determine if a new particule needs to be spawned*/
				if(((float)rand()/(float)RAND_MAX)>0.66){
					particle_counter[BEGIN] = (PARTICLE_LENGTH-1);
					particle_color[BEGIN] = BLUE;
					
				}
			}	
		}else{
			red_update_counter--;
		}
		
		
		if(blue_update_counter<=0){
			blue_update_counter = blue_update_period;
			
			/*from the end to explosion*/
			/*roll back by bringing encountered values forward*/
			for(i=explosion_location;i<NB_LEDS;i++){
				buffer[i-1].red = buffer[i].red;
				buffer[i-1].green = buffer[i].green;
				buffer[i-1].blue = buffer[i].blue;
			}
			
			
			/*check if a particle is being placed at the end*/
			if(particle_counter[END]>0){
				
				switch(particle_color[END]){
					
					case RED:
						buffer[NB_LEDS-1].red = particle_kernel[particle_counter[END]];
						buffer[NB_LEDS-1].green = 0;
						buffer[NB_LEDS-1].blue = 0;
						break;
					case GREEN:
						buffer[NB_LEDS-1].red = 0;
						buffer[NB_LEDS-1].green = particle_kernel[particle_counter[END]];
						buffer[NB_LEDS-1].blue = 0;
						break;
					case BLUE:
						buffer[NB_LEDS-1].red = 0;
						buffer[NB_LEDS-1].green = 0;
						buffer[NB_LEDS-1].blue = particle_kernel[particle_counter[END]];
						break;
				
				}
				particle_counter[END]--;
			}else{
		
				buffer[NB_LEDS-1].red = 0;
				buffer[NB_LEDS-1].green = 0;
				buffer[NB_LEDS-1].blue = 0;
				
				/*else roll a dice to determine if a new particule needs to be spawned*/
				if(((float)rand()/(float)RAND_MAX)>0.66){
					particle_counter[END] = (PARTICLE_LENGTH-1);
					particle_color[END] = RED;
					
					//printf("New particle up!\n");
				}
			}	
		}else{
			blue_update_counter--;
		}
		
		
		/*paint explosion on top*/
		if(explosion_delay<=0){
			for(i=0;i<8;i++){
				
				address = explosion_location-4 + i;
				
				if(address>=0 && address<NB_LEDS){
				
					if(((float)rand()/(float)RAND_MAX)>explosion_animation_kernel[i]){
						
						buffer[address].red = explosion_kernel[i];
						buffer[address].green = explosion_kernel[i];
						buffer[address].blue = explosion_kernel[i];
					}else{
						buffer[address].red = 0x00;
						buffer[address].green = 0x00;
						buffer[address].blue = 0x00;
					}
				}
				
			}
		}
		
		
		write(spi_driver, buffer, NB_LEDS*sizeof(pixel_t));
		usleep(5000);	
	}
	
	return NULL;
}


void cleanup_led_strip(){
	close(spi_driver);
	led_strip_alive = 0x00;
	pthread_join(led_strip_thread,NULL); 
}
