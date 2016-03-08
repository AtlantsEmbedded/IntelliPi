#ifndef XML_H
#define XML_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ezxml.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>

#define SHM_INPUT 1    
#define FAKE_INPUT 2

#define SHM_OUTPUT 1  
#define CSV_OUTPUT 2  

#define MAX_CHAR_FIELD_LENGTH 18

typedef struct appconfig_s {
	
	char debug;
	
	/*data source config*/
	char data_source;
    int nb_channels;
    int window_width;
    
    int rd_shm_key;
    int wr_shm_key;
	int sem_key;
	
	/*feature vector config*/
	char feature_dest;
	
	uint16_t timeseries:1;
	uint16_t fft:1;
	uint16_t power_alpha:1;
	uint16_t power_beta:1;
	uint16_t power_gamma:1;
	
} appconfig_t;

appconfig_t *xml_initialize(char *filename);
inline appconfig_t *get_appconfig();
inline void set_appconfig(appconfig_t *config_obj);


#endif
