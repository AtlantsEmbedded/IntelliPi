/**
 * @file mindbx_lib.c
 * @author Frederic Simard (frederic.simard.1@outlook.com) | atlants embedded
 * @brief Libraries of functions to interact with the mindbx hardware.
 *        Functions allow for the monitoring of the coin acceptor and test button, 
 *        and the control over LED strip and the solenoid that push the door open.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define NB_LEDS 150
#define RGB_BYTE_SIZE 3    
#define BUFFER_LENGTH NB_LEDS*RGB_BYTE_SIZE

#define SPI_CHANNEL 0
#define SPI_SPEED 1000000

unsigned char* ledstate_buffer;

void setup_cerebralwars(void){
	

	/*check if root*/
	if (geteuid () != 0)
	{
		fprintf (stderr, "mindbx need to be root to run\n") ;
		exit (0) ;
	}

	/*setup the wiring pi*/
	if(wiringPiSetup () == -1)
		exit (1) ;
	
	/*setup SPI*/
	if(wiringPiSPISetup (SPI_CHANNEL, SPI_SPEED) == -1)
		exit(1);
	
	
	/*allocate memory for LED state buffer*/
	ledstate_buffer = (unsigned char*)malloc(BUFFER_LENGTH*sizeof(char));
	
}

void set_training_mode(void){
	
	/*set training LED on*/
	
}

void set_pairing_mode(void){
	
	/*flash leds to indicate pairing mode*/
	
}

void set_game_mode(void){
	
	/*set game led on*/
		
}

void set_gameover_mode(void){
	/*turn off game led on*/
	
}

void reset_mode(void){
	/*set all leds to off*/
	
	
}

void compute_ledstrip_newstate(int explosion_loc, int* player1_particules_loc, int player1_nb_particules, int* player2_particules_loc, int player2_nb_particules){
	
}

void push_ledstrip_state(void){
	int wiringPiSPIDataRW (SPI_CHANNEL, ledstate_buffer, BUFFER_LENGTH);
}

void cleanup_cerebralwars(void){
	
	
	free(ledstate_buffer);
	
}
