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
 * https://sites.google.com/a/interaxon.ca/muse-developer-site/muse-communication-protocol/data-streaming-protocol
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MUSE_SYNC_PKT 0xF	 //First nibble of sync packet
#define MUSE_UNCOMPRESS_PKT 0xE	 //Uncompressed EEG
#define MUSE_ERR_PKT 0xD	 //Error Flags
#define MUSE_COMPRESSED_PKT 0xC	 //Compressed EEG
#define MUSE_BATT_PKT 0xB	 //Battery
#define MUSE_ACC_PKT 0xA	 //Accelerometer
#define MUSE_DRLREF_PKT  0x9	 //DRL/REF
#define MUSE_INVALID  0x0	 //Invalid

#define MAX_NB_SOFT_PACKETS 10

int get_packet_type(unsigned char packet_header);

int preparse_packet(unsigned char* raw_packet_header, int packet_length, int *soft_packet_headers, int *soft_packet_types);

int parse_compressed_packet(unsigned char* packet_header, int* deltas);

void parse_uncompressed_packet(unsigned char* values_header, int* values);

inline int get_flag_value(unsigned char first_byte);










#endif


