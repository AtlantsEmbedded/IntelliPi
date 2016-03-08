#ifndef PREPROCESS_CORE_H
#define PREPROCESS_CORE_H

#include <stdio.h>
#include <stdlib.h>

#include "data_structure.h"

int init_preprocess_core(appconfig_t *config);

/*this function take the data and form the feature output*/
int preprocess_data(data_t* data_input, data_t* feature_output);

int cleanup_preprocess_core();

#endif
