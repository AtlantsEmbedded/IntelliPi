/**
 * @file openbci.h
 * @author Ron Brash (ron.brash@gmail.com)
 * @brief Header for openbci hardware 
 */

// Placeholders
#define OPENBCI_KEEP_ALIVE "k\r\n" // keep alive
#define OPENBCI_NOTCH_FREQ "g 407c\r\n"	// 60hz filter
#define OPENBCI_SET_HOST_PLATFORM "r 5\r\n"	//linux
#define OPENBCI_PRESET "% 10\r\n"	// no accell or battery
#define OPENBCI_START_TRANSMISSION "s\r\n" // start sending data
#define OPENBCI_HALT_TRANSMISSION "h\r\n" // halt data transmission
#define OPENBCI_VERSION "v\r\n" // request device information

typedef enum { OPENBCI_HLDER } openbci_pkt_type_t;

typedef struct openbci_pkt_s {
	openbci_pkt_type_t type;
} openbci_pkt_t;

int openbci_init_hardware(void *param);
int openbci_read_pkt(void *param);
int openbci_send_keep_alive_pkt(void *param);
int openbci_send_pkt(void *param);
int openbci_translate_pkt(void *param);
void openbci_process_pkt(param_t * param);
int openbci_connect_dev(void *param);
int openbci_cleanup(void *param);
