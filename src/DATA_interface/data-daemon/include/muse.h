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
#define MUSE_VERSION "v\r\n"	// request device information

typedef enum { MUSE_RAW_EEG, MUSE_COMP_MUSE_EEG, MUSE_SYNC, MUSE_DRL_REF, MUSE_ERROR, MUSE_ACCEL,
	    MUSE_BATT } muse_pkt_type_t;

typedef struct muse_pkt_s {
	muse_pkt_type_t type;
} muse_pkt_t;

int muse_init_hardware(void *param);
int muse_read_pkt(void *param);
int muse_send_keep_alive_pkt(void *param);
int muse_send_pkt(void *param);
int muse_translate_pkt(void *param);
