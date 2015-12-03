/**
 * @file xml.c
 * @author Ron Brash (ron.brash@gmail.com)
 *         Frederic Simard (frederic.simard.1@outlook.com)
 * 		   For Atlants Embedded, 2015
 * @brief Contains all xml configuration options for preprocessing
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ezxml.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>

#include "xml.h"

static int get_app_attributes(ezxml_t app_attribute, appconfig_t * app_info);
static int sanity_check_app_attributes(ezxml_t app_attribute);

const char *XML_app_elements[] =
    { "debug", "feature_source", "feature_vect_size", "training_set_size", "eeg_harware_present","test_duration","avg_kernel"};

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
 * @brief parse menu attributes and defines appconfig struct
 * @param app_attribute, reference to xml file
 * @param (out)app_info, now contains information of xmlfile
 * @return < 0 for error, 0 for success
 */
static int get_app_attributes(ezxml_t app_attribute, appconfig_t * app_info)
{

	/*Quick sanity check of app attributes element in XML*/
	if (sanity_check_app_attributes(app_attribute) < 0) {
		printf("appAttributes has an error\n");
		return (-1);
	}
	
	/*Get appAttributes/debug*/
	ezxml_t tmp = ezxml_child(app_attribute, "debug");
	if (tmp == NULL) {
		printf("appAttributes->debug is missing\n");
		return (-1);
	}
	if (strncmp(tmp->txt, "TRUE", 4) == 0) {
		app_info->debug = 1;
	} else {
		app_info->debug = 0;
	}

	/*Get appAttributes/feature_source*/
	tmp = ezxml_child(app_attribute, "feature_source");
	if (tmp == NULL) {
		printf("appAttributes->feature_source is missing\n");
		return (-1);
	}
	if(strcmp(tmp->txt,"FAKE")==0){
		app_info->feature_source = FAKE_INPUT;
	}else if(strcmp(tmp->txt,"SHM")==0){
		app_info->feature_source = SHM_INPUT;
	}else{
		app_info->feature_source = 0;
	}

	/*Get appAttributes/feature_vect_size*/
	tmp = ezxml_child(app_attribute, "feature_vect_size");
	if (tmp == NULL) {
		printf("appAttributes->feature_vect_size is missing\n");
		return (-1);
	}
	app_info->feature_vect_size = atoi(tmp->txt);

	/*Get appAttributes/training_set_size*/
	tmp = ezxml_child(app_attribute, "training_set_size");
	if (tmp == NULL) {
		printf("appAttributes->training_set_size is missing\n");
		return (-1);
	}
	app_info->training_set_size = atoi(tmp->txt);
	
	tmp = ezxml_child(app_attribute, "eeg_harware_present");
	if (tmp == NULL) {
		printf("appAttributes->eeg_harware_present is missing\n");
		return (-1);
	}
	if (strncmp(tmp->txt, "TRUE", 4) == 0) {
		app_info->eeg_hardware_required = 1;
	} else {
		app_info->eeg_hardware_required = 0;
	}
	
	tmp = ezxml_child(app_attribute, "test_duration");
	if (tmp == NULL) {
		printf("appAttributes->test_duration is missing\n");
		return (-1);
	}
	app_info->test_duration = atof(tmp->txt);
		
	tmp = ezxml_child(app_attribute, "avg_kernel");
	if (tmp == NULL) {
		printf("appAttributes->avg_kernel is missing\n");
		return (-1);
	}
	app_info->avg_kernel = atof(tmp->txt);
	
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
