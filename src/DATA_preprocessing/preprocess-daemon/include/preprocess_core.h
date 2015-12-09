#ifndef PREPROCESS_CORE_H
#define PREPROCESS_CORE_H
/**
 * @file preprocess_core.c
 * @author Frederic Simard (fred.simard@atlantsembedded.com) | Atlants Embedded
 * @brief Signal preprocessing core. It can let the raw data flow through,
 *        compute the fft or the EEG freq power bands.
 */


#include <stdio.h>
#include <stdlib.h>

#include "data_structure.h"

/*init function*/
int init_preprocess_core(appconfig_t *config);

/*this function take the data and form the feature output*/
int preprocess_data(data_t* data_input, data_t* feature_output);

/*clean up function*/
void cleanup_preprocess_core();

#endif
