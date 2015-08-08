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

#define COMMAND_LINE_OUTPUT 1  
#define WIRING_OUTPUT 2  

#define MAX_CHAR_FIELD_LENGTH 18

typedef struct appconfig_s {
	
	char debug;
	
	/*data source config*/
	char feature_source;
    int nb_features;
	
	/**/
	int training_set_size;
	
} appconfig_t;

appconfig_t *xml_initialize(char *filename);
inline appconfig_t *get_appconfig();
inline void set_appconfig(appconfig_t *config_obj);


#endif
