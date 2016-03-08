/**
 * @file main.c
 * @author Frederic Simard (frederic.simard.1@outlook.com) | Atlants Embedded
 * @brief 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "data_structure.h"
#include "app_signal.h"
#include "data_input.h"
#include "feature_output.h"
#include "xml.h"
#include "preprocess_core.h"

#define CONFIG_NAME "config/preprocess_config.xml"

static data_t data_input;
static data_t feature_output;

void init_app_buffers(appconfig_t *config);
void cleanup_app_buffers(appconfig_t *config);


/**
 * which_config(int argc, char **argv)
 * @brief return which config to use
 * @param argc
 * @param argv
 * @return string of config
 */
inline char *which_config(int argc, char **argv)
{

	if (argc == 2) {
		return argv[1];
	} else {
		return CONFIG_NAME;
	}
}

/**
 * main()
 * @brief Application main running loop
 */
int main(int argc __attribute__ ((unused)), char **argv __attribute__ ((unused)))
{
	int i,j;
	
	/*Set up ctrl c signal handler*/
	(void)signal(SIGINT, ctrl_c_handler);

	/*read the config from the xml*/
	appconfig_t *config = (appconfig_t *) xml_initialize(which_config(argc, argv));
	if (config == NULL) {
		printf("Error initializing XML configuration\n");
		return (-1);
	}
	printf("xml configuration read\n");
	
	data_input_options_t data_input_opt;
	data_input_opt.shm_key = config->rd_shm_key;
	data_input_opt.sem_key = config->sem_key;
	data_input_opt.nb_channels = config->nb_channels;
	data_input_opt.window_width = config->window_width;
	
	/*Initialize data input*/
	if(init_data_input(config->data_source, data_input_opt)){
		printf("Error initializing data input");
		return (-1);
	}
	printf("Data input initialized\n");
	
	
	feature_output_options_t feature_output_opt;
	feature_output_opt.shm_key = config->wr_shm_key;
	feature_output_opt.sem_key = config->sem_key;
	feature_output_opt.power_alpha = config->power_alpha;
	feature_output_opt.power_beta = config->power_beta;
	feature_output_opt.power_gamma = config->power_gamma;
	
	/*Initialize feature output*/
	if(init_feature_output(config->feature_dest,feature_output_opt)){
		printf("Error initializing feature output");
		return (-1);
	}
	printf("Feature output initialized\n");

	init_app_buffers(config);
	init_preprocess_core(config);
	
	/*Begin data processing*/
	while(1)
	{
		
		printf("pre-process: about to read\n");
		/*read data*/
		READ_DATA_FC(&data_input);
		
		printf("pre-process: about to preprocess\n");
		/*preprocess into a feature vector*/
		preprocess_data(&data_input, &feature_output);
		
		printf("pre-process: about to write\n");
		/*write feature vector*/
		WRITE_DATA_FC(&feature_output);
	}

	TERMINATE_DATA_INPUT_FC();
	TERMINATE_FEATURE_OUTPUT_FC();
	
	cleanup_preprocess_core(config);
	cleanup_app_buffers(config);
	
	return 0;
}

void init_app_buffers(appconfig_t *config){
	
	/*Fred: not intuitive to be cleaned up*/
	data_input.type = DOUBLE;
	data_input.nb_data = config->nb_channels*config->window_width;
	data_input.ptr = malloc(config->nb_channels*config->window_width*sizeof(double)); /*to be changed to float*/
	
	/*Fred: not intuitive to be cleaned up*/
	feature_output.type = DOUBLE;
	
	feature_output.nb_data = 0;
	
	//TODO if(config->timeseries)
	
	if(config->fft){
		/*one-sided fft is half window's width. multiplied by number of data channels*/
		feature_output.nb_data += config->window_width/2*config->nb_channels; 
	}
	
	//TODO if(config->power_alpha)
	
	//TODO if(config->power_beta)
	
	//TODO if(config->power_gamma)
	
	feature_output.ptr = malloc(feature_output.nb_data*sizeof(double));
}
	
void cleanup_app_buffers(appconfig_t *config){
	
	free(data_input.ptr);
	free(feature_output.ptr);
	
}
