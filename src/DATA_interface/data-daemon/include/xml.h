/**
 * @file xml.h
 * @author Ron Brash (ron.brash@gmail.com)
 * @brief Header for xml functions and structures
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ezxml.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>

#define CSV_OUTPUT 1
#define SHM_OUTPUT 4  
#define BINARY_OUTPUT 2
#define MMAP_OUTPUT 3

typedef struct appconfig_s {
	unsigned char interface[8];
	unsigned char device[8];
	unsigned char remote_addr[18];
	uint32_t compression:1;
	uint32_t keep_alive:1;
	uint32_t process_data:1;
	uint32_t buffer:1;
	uint32_t output_format:2;
	uint16_t samples;
	uint16_t number_runs;
	uint16_t keep_time;
	uint16_t conn_attempts;
} appconfig_t;

appconfig_t *xml_initialize(char *filename);
inline appconfig_t *get_appconfig();
inline void set_appconfig(appconfig_t *config_obj);
