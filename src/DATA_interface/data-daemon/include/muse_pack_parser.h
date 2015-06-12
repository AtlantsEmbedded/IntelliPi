#ifndef DECOMP_H
#define DECOMP_H
/**
 * @file muse_pack_parser.c
 * @author Frédéric Simard, Atlans embedded
 * @date June, 2015
 * @brief This file implements the function required to parse a packet
 *        sent by the muse. The following packets types can be parsed:
 *        - compressed EEG
 *        - uncompressed EEG
 * 
 * For most part, the algorithm used for compressed EEG is described here:
 * https://sites.google.com/a/interaxon.ca/muse-developer-site/muse-communication-protocol/compressed-eeg-packets
 * 
 * And the general description of the Muse bluetooth protocol:
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


void compressed_parse_medians(char* medians_header, int* quantizations, int* medians);

int compressed_parse_bit_length(char* bit_length_header);

int compressed_parse_deltas(char* bits_header, int* median, int* quantization, int* deltas);








#endif


