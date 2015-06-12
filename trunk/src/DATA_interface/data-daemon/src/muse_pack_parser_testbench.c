

#include "muse_pack_parser.h"

void test_compressed_packet_1(void);
void test_uncompressed_packet(void);

int main(void)
{
	
	test_compressed_packet_1();
	
	return 0x00;
}

void test_compressed_packet_1(void){
	
	char packet[40] = {0xc0, 0x41, 0x0a, 0x19, 0x64, 0x90, 0x00, 0xfb,
				       0x38, 0x13, 0x31, 0x38, 0x19, 0x26, 0x22, 0x66,
				       0x94, 0x31, 0x06, 0xe6, 0x0d, 0x5a, 0x71, 0xc4,
				       0xc0, 0x89, 0x89, 0xcf, 0x8c, 0xc6, 0x40, 0xf0,
				       0x44, 0x18, 0xc9, 0x88, 0x19, 0x31, 0x13, 0x00};
	
	int i=0;
	int medians[4];
	int quantizations[4];
    int deltas[16*4];
    int expected_deltas[16*4] = {0, -64, 0, 32, 32, -32, 32, -64, 0, 64, 0, 0, -32, -32, 32, 32,
								 -96, -32, -64, 32, 0, -64, 0, 96, 64, -64, 0, -96, 96, -32, 32, -64,
								 -96, 32, -32, 0, -32, 32, -32, 32, 64, -128, 64, -32, 64, 0, 0, 
								 -128, 0, 0, -32, 0, -32, 64, 0, -32, -32, 0, 64, 0, -32, -32, 32, -32};
	
	int nb_bits = 0;
	
	printf("\n\n");
	printf("*************************\n");
	printf("Medians and Quantizations\n");
	printf("*************************\n");
	printf("\n");	
	
	/*parse out the medians and quantizations*/
	compressed_parse_medians(&(packet[1]), quantizations, medians);
	
	printf("Expected\n");
	printf("medians[0]:1\n");
	printf("quantizations[0]:32\n");
	printf("medians[1]:2\n");
	printf("quantizations[1]:32\n");
	printf("medians[2]:1\n");
	printf("quantizations[2]:32\n");
	printf("medians[3]:1\n");
	printf("quantizations[3]:32\n");
	printf("\n");
	printf("Obtained\n");
	printf("medians[0]:%i\n",medians[0]);
	printf("quantizations[0]:%i\n",quantizations[0]);
	printf("medians[1]:%i\n",medians[1]);
	printf("quantizations[1]:%i\n",quantizations[1]);
	printf("medians[2]:%i\n",medians[2]);
	printf("quantizations[2]:%i\n",quantizations[2]);
	printf("medians[3]:%i\n",medians[3]);
	printf("quantizations[3]:%i\n",quantizations[3]);
	printf("\n");
	
	printf("\n\n");
	printf("*************************\n");
	printf("Deltas\n");
	printf("*************************\n");
	printf("\n");	
	
	/*parse out the deltas*/
	nb_bits = compressed_parse_deltas(&(packet[8]), medians, quantizations, deltas);
	
	/*show the results*/ 
	for(i=0;i<(16*4);i++){
		printf("delta[%i]: %i:%i\n",i,expected_deltas[i],deltas[i]);
	}
	
	printf("nb_bits: %i:%i\n",nb_bits,compressed_parse_bit_length(&(packet[6])));
	
}



void test_uncompressed_packet(void){
	
	
}
