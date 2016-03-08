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
    { "debug", "data_source", "nb_channels", 
	  "window_width", "feature_dest",
	  "rd_shm_key","wr_shm_key","sem_key", 
	  "timeseries", "fft", "power_alpha", 
	  "power_beta", "power_gamma"};

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
	
	ezxml_t tmp = ezxml_child(app_attribute, "debug");
	/*Get appAttributes/debug*/
	if (tmp == NULL) {
		printf("appAttributes->debug is missing\n");
		return (-1);
	}
	/*interpret and turn to boolean value*/
	if (strncmp(tmp->txt, "TRUE", 4) == 0) {
		app_info->debug = 1;
	} else {
		app_info->debug = 0;
	}

	/*Get appAttributes/data_source*/
	tmp = ezxml_child(app_attribute, "data_source");
	if (tmp == NULL) {
		printf("appAttributes->data_source is missing\n");
		return (-1);
	}
	/*interpret and turn to boolean value*/
	if (strncmp(tmp->txt, "SHM", 3) == 0){
		app_info->data_source = SHM_INPUT;
	}else if (strncmp(tmp->txt, "FAKE_DATA", 9) == 0){
		app_info->data_source = FAKE_INPUT;
	}

	/*Get appAttributes/nb_channels*/
	tmp = ezxml_child(app_attribute, "nb_channels");
	if (tmp == NULL) {
		printf("appAttributes->nb_channels is missing\n");
		return (-1);
	}
	app_info->nb_channels = atoi(tmp->txt);

	/*Get appAttributes/buffer_depth*/
	tmp = ezxml_child(app_attribute, "window_width");
	if (tmp == NULL) {
		printf("appAttributes->window_width is missing\n");
		return (-1);
	}
	app_info->window_width = atoi(tmp->txt);

	/*Get appAttributes/shm_key*/
	tmp = ezxml_child(app_attribute, "rd_shm_key");
	if (tmp == NULL) {
		printf("appAttributes->rd_shm_key is missing\n");
		return (-1);
	}
	app_info->rd_shm_key = atoi(tmp->txt);

	/*Get appAttributes/shm_key*/
	tmp = ezxml_child(app_attribute, "wr_shm_key");
	if (tmp == NULL) {
		printf("appAttributes->wr_shm_key is missing\n");
		return (-1);
	}
	app_info->wr_shm_key = atoi(tmp->txt);

	/*Get appAttributes/sem_key*/
	tmp = ezxml_child(app_attribute, "sem_key");
	if (tmp == NULL) {
		printf("appAttributes->sem_key is missing\n");
		return (-1);
	}
	app_info->sem_key = atoi(tmp->txt);

	/*Get appAttributes/feature_dest*/
	tmp = ezxml_child(app_attribute, "feature_dest");
	if (tmp == NULL) {
		printf("appAttributes->feature_dest is missing\n");
		return (-1);
	}	
	/*interpret and turn to boolean value*/
	if (strncmp(tmp->txt, "SHM", 3) == 0){
		app_info->feature_dest = SHM_OUTPUT;
	}else if (strncmp(tmp->txt, "CSV", 3) == 0){
		app_info->feature_dest = CSV_OUTPUT;
	}
	
	/*Get appAttributes/timeseries*/
	tmp = ezxml_child(app_attribute, "timeseries");
	if (tmp == NULL) {
		printf("appAttributes->timeseries is missing\n");
		return (-1);
	}
	/*interpret and turn to boolean value*/
	if (strncmp(tmp->txt, "TRUE", 4) == 0){
		app_info->timeseries = 1;
	}else if (strncmp(tmp->txt, "FALSE", 5) == 0){
		app_info->timeseries = 0;
	}	
	
	/*Get appAttributes/fft*/
	tmp = ezxml_child(app_attribute, "fft");
	if (tmp == NULL) {
		printf("appAttributes->fft is missing\n");
		return (-1);
	}
	/*interpret and turn to boolean value*/
	if (strncmp(tmp->txt, "TRUE", 4) == 0){
		app_info->fft = 1;
	}else if (strncmp(tmp->txt, "FALSE", 5) == 0){
		app_info->fft = 0;
	}	
	
	/*Get appAttributes/power_alpha*/
	tmp = ezxml_child(app_attribute, "power_alpha");
	if (tmp == NULL) {
		printf("appAttributes->power_alpha is missing\n");
		return (-1);
	}
	/*interpret and turn to boolean value*/
	if (strncmp(tmp->txt, "TRUE", 4) == 0){
		app_info->power_alpha = 1;
	}else if (strncmp(tmp->txt, "FALSE", 5) == 0){
		app_info->power_alpha = 0;
	}	
	
	/*Get appAttributes/power_beta*/
	tmp = ezxml_child(app_attribute, "power_beta");
	if (tmp == NULL) {
		printf("appAttributes->power_beta is missing\n");
		return (-1);
	}
	/*interpret and turn to boolean value*/
	if (strncmp(tmp->txt, "TRUE", 4) == 0){
		app_info->power_beta = 1;
	}else if (strncmp(tmp->txt, "FALSE", 5) == 0){
		app_info->power_beta = 0;
	}	
	
	/*Get appAttributes/power_gamma*/
	tmp = ezxml_child(app_attribute, "power_gamma");
	if (tmp == NULL) {
		printf("appAttributes->power_gamma is missing\n");
		return (-1);
	}
	/*interpret and turn to boolean value*/
	if (strncmp(tmp->txt, "TRUE", 4) == 0){
		app_info->power_gamma = 1;
	}else if (strncmp(tmp->txt, "FALSE", 5) == 0){
		app_info->power_gamma = 0;
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
