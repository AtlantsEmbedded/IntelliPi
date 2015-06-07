/**
 * @file xml.c
 * @author Ron Brash (ron.brash@gmail.com)
 * @brief Contains all xml configuration options
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ezxml.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>

#include "main.h"
#include "xml.h"

static int get_app_attributes(ezxml_t app_attribute, appconfig_t * app_info);
static int sanity_check_app_attributes(ezxml_t app_attribute);

const char *XML_app_elements[] =
    { "debug", "interface", "device", "keep_alive", "keep_time", "compression", "remote_addr", "process_data", "buffer",
	"samples", "conn_attempts",
	"number_runs", "output_format"
};

static appconfig_t *config = NULL;

/**
 * get_appconfig()
 * @brief Returns the appconfig
 * @return appconfig_t
 */
inline appconfig_t *get_appconfig()
{
	return config;
}

/**
 * set_appconfig(appconfig_t * config_obj)
 * @brief sets appconfig
 * @param config_obj
 */
inline void set_appconfig(appconfig_t * config_obj)
{
	config = config_obj;
}

/**
 * get_app_attributes(ezxml_t app_attribute, app_info_s * app_info)
 * @brief parse menu attributes
 * @param app_attribute
 * @param app_info
 * @return < 0 for error, 0 for success
 */
static int get_app_attributes(ezxml_t app_attribute, appconfig_t * app_info)
{

	// Quick sanity check of app attributes element in XML
	if (sanity_check_app_attributes(app_attribute) < 0) {
		printf("appAttributes has an error\n");
		return (-1);
	}
	ezxml_t tmp = ezxml_child(app_attribute, "interface");

	// Get app name
	if (tmp == NULL) {
		printf("appAttributes->name is missing\n");
		return (-1);
	}
	memcpy(app_info->interface, tmp->txt, 8);

	// Get app text
	tmp = ezxml_child(app_attribute, "device");
	if (tmp == NULL) {
		printf("appAttributes->text is missing\n");
		return (-1);
	}

	memcpy(app_info->device, tmp->txt, 8);

	// Get remote_addr
	tmp = ezxml_child(app_attribute, "remote_addr");
	if (tmp == NULL) {
		printf("appAttributes->remote_addr is missing\n");
		return (-1);
	}

	memcpy(app_info->remote_addr, tmp->txt, 18);

	// Get compression
	tmp = ezxml_child(app_attribute, "compression");
	if (tmp == NULL) {
		printf("appAttributes->compression is missing\n");
		return (-1);
	}

	if (strncmp(tmp->txt, "TRUE", 4) == 0) {
		app_info->compression = 1;
	} else {
		app_info->compression = 0;
	}

	// Get keep_alive
	tmp = ezxml_child(app_attribute, "keep_alive");
	if (tmp == NULL) {
		printf("appAttributes->keep_alive is missing\n");
		return (-1);
	}

	if (strncmp(tmp->txt, "TRUE", 4) == 0) {
		app_info->keep_alive = 1;
	} else {
		app_info->keep_alive = 0;
	}

	// Get process_data
	tmp = ezxml_child(app_attribute, "process_data");
	if (tmp == NULL) {
		printf("appAttributes->process_data is missing\n");
		return (-1);
	}

	if (strncmp(tmp->txt, "TRUE", 4) == 0) {
		app_info->process_data = 1;
	} else {
		app_info->process_data = 0;
	}

	// Get buffer
	tmp = ezxml_child(app_attribute, "buffer");
	if (tmp == NULL) {
		printf("appAttributes->buffer is missing\n");
		return (-1);
	}

	if (strncmp(tmp->txt, "TRUE", 4) == 0) {
		app_info->buffer = 1;
	} else {
		app_info->buffer = 0;
	}

	// Get samples
	tmp = ezxml_child(app_attribute, "samples");
	if (tmp == NULL) {
		printf("appAttributes->samples is missing\n");
		return (-1);
	}
	app_info->samples = (uint16_t) atoi(tmp->txt);

	// number_runs
	tmp = ezxml_child(app_attribute, "number_runs");
	if (tmp == NULL) {
		printf("appAttributes->number_runs is missing\n");
		return (-1);
	}
	app_info->number_runs = (uint16_t) atoi(tmp->txt);

	// conn_attempts
	tmp = ezxml_child(app_attribute, "conn_attempts");
	if (tmp == NULL) {
		printf("appAttributes->conn_attempts is missing\n");
		return (-1);
	}
	app_info->conn_attempts = (uint16_t) atoi(tmp->txt);

	// Get keep_time
	tmp = ezxml_child(app_attribute, "keep_time");
	if (tmp == NULL) {
		printf("appAttributes->keep-time is missing\n");
		return (-1);
	}
	app_info->keep_time = (uint16_t) atoi(tmp->txt);

	// Get output_format
	tmp = ezxml_child(app_attribute, "output_format");
	if (tmp == NULL) {
		printf("appAttributes->output_format is missing\n");
		return (-1);
	}

	if (strncmp((const char *)tmp->txt, "CSV", 3) == 0) {
		app_info->output_format = CSV_OUTPUT;
	} else if (strncmp((const char *)tmp->txt, "BINARY", 6) == 0) {
		app_info->output_format = BINARY_OUTPUT;
	} else if (strncmp((const char *)tmp->txt, "MMAP", 4) == 0) {
		app_info->output_format = MMAP_OUTPUT;
	} else {
		app_info->output_format = 0;
	}

	return (0);
}

/**
 * XML_exists(char *file)
 * @brief Checks to see if a file exists
 * @param file
 * @return 0 for success, -1 for error
 */
static int XML_exists(char *file)
{
	FILE *fp = NULL;
	if ((fp = fopen(file, "r")) == NULL) {
		printf("Error opening file\n");
		return (-1);
	}

	if (fclose(fp) != 0) {
		fp = NULL;
		printf("Error closing file\n");
		return (-1);
	}

	fp = NULL;

	return (0);

}

/**
 * sanity_check_app_attributes(ezxml_t menu_item)
 * @brief Provide a quick sanity check to make sure that there are 
 * at least the minimum number of elements present in the 
 * app attributes element
 * @param menu_item
 * @return 0 for success, -1 for error
 */
static int sanity_check_app_attributes(ezxml_t app_attribute)
{

	int elements = (sizeof(XML_app_elements) / sizeof(XML_app_elements[0]));
	int i = 0;
	for (i = 0; i < elements; i++) {
		ezxml_t test = ezxml_child(app_attribute, XML_app_elements[i]);
		if (test == NULL) {
			printf("element is missing: %s\n", XML_app_elements[i]);
			return (-1);
		}
	}

	return (0);

}

/**
 * parse_menu_XML(char *file, app_info_s * app_info)
 * @brief
 * @param file
 * @param app_info
 * @return < 0 for error, 0 for success
 */
static int parse_menu_XML(char *file, appconfig_t * app_info)
{

	ezxml_t app_config = ezxml_parse_file(file);
	ezxml_t app_attributes = NULL;
	int err = 0;

	// Are app attributes defined?
	if ((app_attributes = ezxml_child(app_config, "appAttributes")) == NULL) {
		printf("appAttributes is missing\n");
		return (-1);
	}
	// Parse application attributes from XML
	if (get_app_attributes(app_attributes, app_info) < 0) {
		printf("appAttributes error\n");
		return (-1);
	}

	ezxml_free(app_config);
	return err;
}

/**
 * xml_initialize()
 * @brief wrap all configuration file and app initializations
 * @param filename
 * @return NULL if error or pointer if "okay"
 */
appconfig_t *xml_initialize(char *filename)
{
	appconfig_t *config_obj = (appconfig_t *) malloc(sizeof(appconfig_t));

	if (config_obj == NULL) {
		printf("Unable to malloc config structure\n");
		return NULL;
	}
	set_appconfig(config_obj);

	if (XML_exists(filename) < 0) {
		return NULL;
	}
	if (parse_menu_XML(filename, config_obj) < 0) {
		return NULL;
	}

	return config_obj;
}
