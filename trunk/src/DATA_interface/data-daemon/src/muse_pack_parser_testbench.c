

#include "muse_pack_parser.h"


void compressed_test_report(int* medians, int* expected_medians, 
							int* quantizations, int* expected_quantizations,
							int* deltas, int* expected_deltas,
						    int bits_length, int expected_bits_length);

void test_compressed_packet_1(void);
void test_compressed_packet_2(void);
void test_uncompressed_packet(void);
void test_preparse_packet(void);

int main(void)
{
	
	//test_compressed_packet_1();
	//test_compressed_packet_2();
	//test_uncompressed_packet();
	test_preparse_packet();
	
	return 0x00;
}

void test_preparse_packet(void){
	
	int i = 0;
	unsigned char packet[61] = {0xff, 0xff, 0xaa, 0x55, 0xc0, 0x01, 0x06, 0x1a, 
							   0xe4, 0x04, 0x01, 0x56, 0xcc, 0xf3, 0xf8, 0x39, 
							   0x9c, 0xf8, 0x8c, 0xc9, 0xcf, 0xe0, 0x19, 0x93, 
							   0x82, 0x27, 0x38, 0x10, 0x66, 0x22, 0x22, 0x62, 
							   0x00, 0xcf, 0x9e, 0x0f, 0x8c, 0xf3, 0x03, 0x7d, 
							   0xf2, 0x67, 0x40, 0x75, 0xc6, 0xf2, 0xd3, 0x29, 
							   0xe1, 0x3e, 0x07, 0xe1, 0xb3, 0xb1, 0x00, 0xe0, 
							   0x17, 0x6a, 0x88, 0x63, 0x84 };
				       
	int expected_nb_of_soft_packets = 3;
	int expected_soft_packets_headers[10] = {0, 4, 55, 0, 0, 0, 0, 0, 0, 0};		
	int expected_soft_packets_types[10] = {MUSE_SYNC_PKT, MUSE_COMPRESSED_PKT, MUSE_UNCOMPRESS_PKT, 0, 0, 0, 0, 0, 0, 0};			
	
	int nb_of_soft_packets;
	int soft_packets_headers[10];		
	int soft_packets_types[10];			       
	
	nb_of_soft_packets = preparse_packet(packet, 61, soft_packets_headers, soft_packets_types);

	printf("\n\n");
	printf("*************************\n");
	printf("PreParse Packet          \n");
	printf("*************************\n");
	
	printf("Nb of Soft Packets: %i:%i\n",expected_nb_of_soft_packets,nb_of_soft_packets);
	for(i=0;i<nb_of_soft_packets;i++){
		printf("Soft Packets Start: %i:%i\n",expected_soft_packets_headers[i],soft_packets_headers[i]);
		printf("Soft Packets Type: %i:%i\n",expected_soft_packets_types[i],soft_packets_types[i]);
	}

}


void test_compressed_packet_1(void){
	
	unsigned char packet[40] = {0xc0, 0x41, 0x0a, 0x19, 0x64, 0x90, 0x00, 0xfb,
				       0x38, 0x13, 0x31, 0x38, 0x19, 0x26, 0x22, 0x66,
				       0x94, 0x31, 0x06, 0xe6, 0x0d, 0x5a, 0x71, 0xc4,
				       0xc0, 0x89, 0x89, 0xcf, 0x8c, 0xc6, 0x40, 0xf0,
				       0x44, 0x18, 0xc9, 0x88, 0x19, 0x31, 0x13, 0x00};
	
	int i=0;
	int medians[4];
	int expected_medians[4] = {1,2,1,1};
	int quantizations[4];
	int expected_quantizations[4] = {32,32,32,32};
    int deltas[16*4];
    int expected_deltas[16*4] = {0, -64, 0, 32, 32, -32, 32, -64, 0, 64, 0, 0, -32, -32, 32, 32,
								 -96, -32, -64, 32, 0, -64, 0, 96, 64, -64, 0, -96, 96, -32, 32, -64,
								 -96, 32, -32, 0, -32, 32, -32, 32, 64, -128, 64, -32, 64, 0, 0, 
								 -128, 0, 0, -32, 0, -32, 64, 0, -32, -32, 0, 64, 0, -32, -32, 32, -32};
	
	int bits_length = 0;
	int expected_bits_length = 0;
	
	/*parse out the medians and quantizations*/
	compressed_parse_medians(&(packet[1]), quantizations, medians);
	
	/*parse out the deltas*/
	bits_length = compressed_parse_deltas(&(packet[8]), medians, quantizations, deltas);
	expected_bits_length = compressed_parse_bit_length(&(packet[6]));
	
	compressed_test_report(medians, expected_medians, 
						   quantizations, expected_quantizations,
						   deltas, expected_deltas,
						   bits_length, expected_bits_length);
	
}

void test_compressed_packet_2(void){
	
	unsigned char packet[45] = {0xc0, 0x4b, 0x04, 0x29, 0xa4, 0x80, 0x01, 0x22, 
					   0x04, 0x49, 0xc7, 0x0e, 0xf0, 0x89, 0x87, 0x3f, 
					   0x58, 0x86, 0x1c, 0xb7, 0x44, 0x62, 0x33, 0x3f, 
					   0x19, 0xf3, 0x13, 0x1c, 0x7c, 0x81, 0x30, 0x15, 
					   0x84, 0x26, 0x2e, 0x93, 0x89, 0x54, 0xa6, 0x44, 
					   0x56, 0xd5, 0xa8, 0x52, 0xc0};
	
	int i=0;
	int medians[4];
	int expected_medians[4] = {11,1,2,2};
	int quantizations[4];
	int expected_quantizations[4] = {2,32,32,16};
    int deltas[16*4];
    int expected_deltas[16*4] = {0, -24, 8, 46, 22, -64, -2, -4, 44, 50, -78, -4, -6, 2, 26, 40,
								 0, -32, -64, -32, 64, 32, -160, 64, 128, -32, 32, -32, -96, 160, 0, 0,
								 64, -64, 0, -32, 96, 0, -32, 0, 64, -64, 96, -96, -32, 32, -64, 
								 64, -16, -48, -16, 32, 32, 0, -32, -48, 80, -16, 48, -16, -32, -16, 32, 16};
	
	int bits_length = 0;
	int expected_bits_length = 0;
	
	/*parse out the medians and quantizations*/
	compressed_parse_medians(&(packet[1]), quantizations, medians);
	
	/*parse out the deltas*/
	bits_length = compressed_parse_deltas(&(packet[8]), medians, quantizations, deltas);
	expected_bits_length = compressed_parse_bit_length(&(packet[6]));
	
	compressed_test_report(medians, expected_medians, 
						   quantizations, expected_quantizations,
						   deltas, expected_deltas,
						   bits_length, expected_bits_length);
	
}



void test_uncompressed_packet(void){
	
	int i=0;
	
	unsigned char packet[6] = {0xe0, 0x17, 0x6a, 0x88, 0x63, 0x84};
	
	char dropped_flag = 0;
	char expected_dropped_flag = 0;
	int eeg_values[4];
	int expected_eeg_values[4] = {535,538,568,529};
	
	dropped_flag = get_flag_value(packet[0]);
	uncompressed_parse_values(&(packet[1]), eeg_values);
	
	printf("\n\n");
	printf("*************************\n");
	printf("Uncompressed Packet      \n");
	printf("*************************\n");
	printf("\n");	
	printf("Expected:Obtained\n");
	printf("dropped flag:%i:%i\n",expected_dropped_flag,dropped_flag);
	if(expected_dropped_flag==dropped_flag){
		printf("OK\n");
	}
	else{
		printf("bug!\n");
	}
	for(i=0;i<4;i++){
		printf("value[%i]:%i:%i ",i,expected_eeg_values[i],eeg_values[i]);
		
		if(expected_eeg_values[i]==eeg_values[i]){
			printf("OK\n");
		}
		else{
			printf("bug!\n");
		}
	}
	
	
}


void compressed_test_report(int* medians, int* expected_medians, 
							int* quantizations, int* expected_quantizations,
							int* deltas, int* expected_deltas,
						    int bits_length, int expected_bits_length){
	
	int i;
	
	printf("\n\n");
	printf("*************************\n");
	printf("Medians and Quantizations\n");
	printf("*************************\n");
	printf("\n");	
	
	printf("Expected:Obtained\n");
	for(i=0;i<4;i++){
		printf("medians[%i]:%i:%i ",i,expected_medians[i],medians[i]);
		
		if(expected_medians[i]==medians[i]){
			printf("OK\n");
		}
		else{
			printf("bug!\n");
		}
		
		printf("quantizations[%i]:%i:%i ",i,expected_quantizations[i],quantizations[i]);
		
		if(expected_quantizations[i]==quantizations[i]){
			printf("OK\n");
		}
		else{
			printf("bug!\n");
		}
		
	}
	
	printf("\n");
	printf("*************************\n");
	printf("Deltas\n");
	printf("*************************\n");
	printf("\n");	
	
	
	/*show the results*/ 
	for(i=0;i<(16*4);i++){
		printf("delta[%i]: %i:%i ",i,expected_deltas[i],deltas[i]);
		if(expected_deltas[i]==deltas[i]){
			printf("OK\n");
		}
		else
		{
			printf("bug!\n");
		}
	}
	
	printf("bits_length: %i:%i ",bits_length,expected_bits_length);
	
	if(bits_length == expected_bits_length){
		printf("OK\n");
	}
	else
	{
		printf("bug!\n");
	}
	
	
	
}

