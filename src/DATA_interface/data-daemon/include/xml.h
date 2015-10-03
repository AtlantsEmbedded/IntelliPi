/**
 * @file xml.h
 * @author Ron Brash (ron.brash@gmail.com), Frederic Simard (frederic.simard.1@outlook.com) | Atlants Embedded 2015
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
#define BINARY_OUTPUT 2
#define MMAP_OUTPUT 3
#define SHM_OUTPUT 4  

#define MAX_CHAR_FIELD_LENGTH 18

typedef struct appconfig_s {
	unsigned char interface[MAX_CHAR_FIELD_LENGTH];
	unsigned char device[MAX_CHAR_FIELD_LENGTH];
	unsigned char remote_addr[MAX_CHAR_FIELD_LENGTH];
	int shm_key;
	int sem_key;
	int nb_data_channels;
	int window_size;
	int nb_pages;
	uint32_t compression:1;
	uint32_t keep_alive:1;
	uint32_t process_data:1;
	uint32_t buffer:1;
	uint32_t output_format:3;
	uint16_t samples;
	uint16_t number_runs;
	uint16_t keep_time;
	uint16_t conn_attempts;
} appconfig_t;

appconfig_t *xml_initialize(char *filename);
inline appconfig_t *get_appconfig();
inline void set_appconfig(appconfig_t *config_obj);
