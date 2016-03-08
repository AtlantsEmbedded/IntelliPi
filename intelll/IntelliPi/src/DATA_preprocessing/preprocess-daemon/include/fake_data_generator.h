
#ifndef FAKE_DATA_GENERATOR_H
#define FAKE_DATA_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "data_structure.h"
#include "data_input.h"

#define NB_CHANNELS 4 
#define NB_SAMPLES 110
#define SAMPLING_FREQ 2200

int fake_data_gen_init();

int fake_data_gen_get_sample(void *param);

int fake_data_gen_cleanup();


#endif
