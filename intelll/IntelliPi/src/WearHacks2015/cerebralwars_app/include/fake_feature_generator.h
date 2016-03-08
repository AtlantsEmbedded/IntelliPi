
#ifndef FAKE_FEAT_GENERATOR_H
#define FAKE_FEAT_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "feature_input.h"

#define NB_CHANNELS 4 

int fake_feat_gen_init();

int fake_feat_gen_get_sample(void *param);

int fake_feat_gen_cleanup();


#endif
