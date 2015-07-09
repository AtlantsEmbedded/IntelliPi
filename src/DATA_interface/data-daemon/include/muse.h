/**
 * @file muse.h
 * @author Ron Brash (ron.brash@gmail.com)
 * @brief Header for muse hardware 
 */

#define MUSE_KEEP_ALIVE "k\r\n"	// keep alive
#define MUSE_NOTCH_FREQ "g 407c\r\n"	// 60hz filter
#define MUSE_SET_HOST_PLATFORM "r 5\r\n"	//linux
#define MUSE_PRESET "% 10\r\n"	// no accell or battery
#define MUSE_START_TRANSMISSION "s\r\n"	// start sending data
#define MUSE_HALT_TRANSMISSION "h\r\n"	// halt data transmission
#define MUSE_VERSION "v 2\r\n"	// request device information

#define MUSE_SYNC_PKT 0xF	 //First nibble of sync packet
#define MUSE_UNCOMPRESS_PKT 0xE	 //Uncompressed EEG
#define MUSE_ERR_PKT 0xD	 //Error Flags
#define MUSE_COMPRESSED_PKT 0xC	 //Compressed EEG
#define MUSE_BATT_PKT 0xB	 //Battery
#define MUSE_ACC_PKT 0xA	 //Accelerometer
#define MUSE_DRLREF_PKT  0x9	 //DRL/REF
#define MUSE_INVALID  0x0	 //Invalid

#define MUSE_NB_CHANNELS 4
#define MUSE_NB_DELTAS 16
//#define MUSE_MAX_NB_PACKETS 10

typedef enum { MUSE_RAW_EEG, MUSE_COMP_MUSE_EEG, MUSE_UNCOMP_MUSE_EEG, MUSE_SYNC, MUSE_DRL_REF, MUSE_ERROR, 
	MUSE_ACCEL, MUSE_BATT } muse_pkt_type_t;

typedef struct muse_translt_pkt_s {
	muse_pkt_type_t type;
	uint8_t nb_samples;
	int* eeg_data;
} muse_translt_pkt_t;


int muse_connect_dev(void *param);
int muse_init_hardware(void *param);
int muse_read_pkt(void *param);
int muse_send_keep_alive_pkt(void *param);
int muse_send_pkt(void *param);
int muse_translate_pkt(void *param);
int muse_process_pkt(void * param);
int muse_cleanup(void *param);
