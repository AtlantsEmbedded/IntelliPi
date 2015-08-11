
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feature_input.h"
#include "fake_feature_generator.h"
#include "shm_rd_buf.h"
#include "xml.h"

/**
 * int init_feature_input(char input_type)
 * @brief Setup function pointers for the data input
 * @param input_type, string identifying the type of input to init
 * @return EXIT_FAILURE for unknown type, EXIT_SUCCESS for known/success
 */
int init_feature_input(char input_type){

	_INIT_FEAT_INPUT_FC = NULL;
	_READ_FEAT_FC = NULL;
	_TERMINATE_FEAT_INPUT_FC = NULL;

	if(input_type == SHM_INPUT) {
		
		printf("Input source: SHM\n");
		_INIT_FEAT_INPUT_FC = &shm_rd_init;
		_READ_FEAT_FC = &shm_rd_read_from_buf;
		_TERMINATE_FEAT_INPUT_FC = &shm_rd_cleanup;
	}
	else if(input_type == FAKE_INPUT){
		printf("Input source: FAKE\n");
		_INIT_FEAT_INPUT_FC = &fake_feat_gen_init;
		_READ_FEAT_FC = &fake_feat_gen_get_sample;
		_TERMINATE_FEAT_INPUT_FC = &fake_feat_gen_cleanup;
	}
	else{
		fprintf(stderr, "Unknown input type\n");
		return EXIT_FAILURE;
	}
	return INIT_FEAT_INPUT_FC();
	
}

