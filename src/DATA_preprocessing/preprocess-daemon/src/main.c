/**
 * @file main.c
 * @author Frederic Simard (fred.simard@atlantsembedded.com) | Atlants Embedded
 * @brief This application implements the preprocessing operation for EEG signals
 *        It takes the signal from the input interface and preprocess it, before
 *        broadcasting it to all output interfaces.
 * 
 *        Options are specified throught the xml config file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "main.h"
#include "data_structure.h"
#include "app_signal.h"
#include "data_input.h"
#include "feature_output.h"
#include "shm_wrt_buf.h"
#include "csv_file.h"
#include "xml.h"
#include "preprocess_core.h"

/*default config file*/
#define CONFIG_NAME "config/preprocess_config.xml"

/*contains app config (xml)*/
appconfig_t *config;

/*input/output buffer*/
static data_t data_input_buf;
static data_t feature_output_buf;
	
/*array of output interfaces*/
static output_interface_array_t output_interface_array;

/*initialisation subroutines*/
int init_data_input_interface();
int init_feat_output_array_interface();
int init_app_buffers();


/**
 * which_config(int argc, char **argv)
 * @brief return which config to use, default or app arg
 * @param argc, number of input arguments
 * @param argv, array of input arguments (string array)
 * @return string of config filename
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
 * int main(int argc, char **argv)
 * @brief return which config to use, default or app arg
 * @param argc, number of input arguments
 * @param argv, array of input arguments (string array)
 * @return string of config filename
 */
int main(int argc, char **argv)
{
	int i;
	
	/*Set up ctrl c signal handler*/
	(void)signal(SIGINT, ctrl_c_handler);

	/*read the config from the xml*/
	config = (appconfig_t *) xml_initialize(which_config(argc, argv));
	if (config == NULL) {
		printf("Error initializing XML configuration\n");
		return (-1);
	}
	
	/*init data input interface*/
	if(init_data_input_interface()==EXIT_FAILURE){
		return (-1);
	}
	
	/*init feature output interface*/
	if(init_feat_output_array_interface()==EXIT_FAILURE){
		return (-1);
	}
	
	/*init buffers*/
	if(init_app_buffers()==EXIT_FAILURE){
		return (-1);
	}
	
	/*init preprocessing*/
	init_preprocess_core(config);
	
	/*Program loop*/
	for(;;)
	{
		
		/*read data*/
		if(READ_DATA_FC(&data_input_buf)==EXIT_SUCCESS){
			
			/*preprocess into a feature vector*/
			preprocess_data(&data_input_buf, &feature_output_buf);
			
			/*loop over all feature outputs*/
			for(i=0;i<output_interface_array.nb_output;i++){
				
				/*write feature vector*/
				WRITE_FEATURE_FC(&feature_output_buf, output_interface_array.output_interface[i]);
			}
			
		}else{
			/*if we get there, it's usually because the semaphore set is*/
			/*waiting to be destroyed*/
			sleep(1);
		}
	}

	/*clean up*/
	cleanup_app();
	
	return 0;
}

/**
 * void init_app_buffers()
 * @brief initialization sub-routine for the input/output buffers
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int init_app_buffers(){
	
	/*Initialize the data input buffer*/
	/*compute number of input data from config*/
	data_input_buf.nb_data = config->nb_channels*config->window_width;
	/*allocate memory*/
	data_input_buf.ptr = malloc(data_input_buf.nb_data*sizeof(double));
	
	/*check mem validity*/
	if(data_input_buf.ptr==NULL){
		return EXIT_FAILURE;
	}
	
	/*Initialize the feature output buffer*/
	/*starts with 0*/
	feature_output_buf.nb_data = 0;
	
	/*then, for each feature set activated in config, increase the size of the buffer*/
	
	/*timeseries is the raw data*/
	if(config->timeseries){
		feature_output_buf.nb_data += config->window_width*config->nb_channels;
	}
	
	/*Fourier transform*/
	if(config->fft){
		/*one-sided fft is half window's width+1. multiplied by number of data channels*/
		feature_output_buf.nb_data += config->window_width/2*config->nb_channels; 
	}
	
	/*EEG Power bands*/
	/*alpha*/
	if(config->power_alpha){
		feature_output_buf.nb_data += config->nb_channels;
	}
	
	/*beta*/
	if(config->power_beta){
		feature_output_buf.nb_data += config->nb_channels;
	}
	
	/*gamma*/
	if(config->power_gamma){
		feature_output_buf.nb_data += config->nb_channels;
	}
	
	/*allocate the memory*/
	feature_output_buf.ptr = malloc(feature_output_buf.nb_data*sizeof(double));
	
	/*check mem validity*/
	if(feature_output_buf.ptr==NULL){
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

/**
 * int init_data_input_interface()
 * @brief initialization sub-routine for the data input interface
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int init_data_input_interface(){
	
	/*define the data input options, from the config*/
	data_input_options_t data_input_opt;
	data_input_opt.shm_key = config->rd_shm_key;
	data_input_opt.sem_key = config->sem_key;
	data_input_opt.nb_channels = config->nb_channels;
	data_input_opt.window_width = config->window_width;
	
	/*Initialize data input*/
	if(init_data_input(config->data_source, data_input_opt)){
		printf("Error initializing data input");
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

/**
 * int init_feat_output_array_interface()
 * @brief initialization sub-routine for the feature output interface array
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int init_feat_output_array_interface(){

	/*define the feature output options, from the config*/
	shm_output_options_t shm_output_opt[1];
	shm_output_opt[0].shm_key = config->wr_shm_key;
	shm_output_opt[0].sem_key = config->sem_key;
	shm_output_opt[0].power_alpha = config->power_alpha;
	shm_output_opt[0].power_beta = config->power_beta;
	shm_output_opt[0].power_gamma = config->power_gamma;
	
	/*allocate the memory for all output interfaces*/
	output_interface_array.nb_output = 1;
	output_interface_array.output_interface = malloc(sizeof(void*)*output_interface_array.nb_output);
	output_interface_array.output_interface[0] = init_feature_output(config->feature_dest,&shm_output_opt);
	
	/*Initialize feature output interfaces*/
	if(output_interface_array.output_interface[0]==NULL){
		printf("Error initializing feature output");
		return EXIT_FAILURE;
	}	
	
	return EXIT_SUCCESS;
}
	
/**
 * void cleanup_app()
 * @brief cleanup sub-routine for the app
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
void cleanup_app(){
	
	int i;
	
	/*free the input/output buffers*/
	cleanup_preprocess_core();
	free(data_input_buf.ptr);
	free(feature_output_buf.ptr);	
	
	/*terminate the input interface*/
	TERMINATE_DATA_INPUT_FC();
	
	/*loop over all feature outputs*/
	for(i=0;i<output_interface_array.nb_output;i++){
		/*terminate this output interface*/
		TERMINATE_FEATURE_OUTPUT_FC(output_interface_array.output_interface[i]);
	}
}
