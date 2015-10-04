

#include <stdio.h>
#include <stdlib.h>

#include "led_strip_lib.h"

int main(int argc, char **argv){
	
	int i;
	
	printf("Getting started\n");
	fflush(stdout); 
	
	init_led_strip();
	
	printf("Waiting 20seconds\n");
	fflush(stdout); 
	
	sleep(20);
	
	printf("Sliding the explosion\n");
	fflush(stdout); 
	
	for(i=0;i<100;i++){
		set_led_strip_values(1, 1, 10+i);
		sleep(1);
	}
	
	printf("Done! Cleaning up\n");
	fflush(stdout); 
	cleanup_led_strip();
	
	exit(0);
}

