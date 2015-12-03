
#ifndef FAKE_FEAT_GENERATOR_H
#define FAKE_FEAT_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "feature_input.h"

int fake_feat_gen_init(void *param);
int fake_feat_gen_request(void *param);
int fake_feat_gen_wait_for_request_completed(void *param);
int fake_feat_gen_cleanup(void *param);

#endif
