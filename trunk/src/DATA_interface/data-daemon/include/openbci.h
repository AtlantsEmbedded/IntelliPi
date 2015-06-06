/**
 * @file openbci.h
 * @author Ron Brash (ron.brash@gmail.com)
 * @brief Header for openbci hardware 
 */

// Placeholders
#define OPENBCI_START_TRANSMISSION "b" // start sending data
#define OPENBCI_HALT_TRANSMISSION "s" // halt data transmission
#define OPENBCI_RESET "v" // request device information

#define STATUS_PACKET_LENGTH 84
#define DATA_PACKET_LENGTH 33

typedef enum { OPENBCI_HLDER } openbci_pkt_type_t;

typedef struct openbci_pkt_s {
	openbci_pkt_type_t type;
} openbci_pkt_t;

int openbci_init_hardware(void *param);
int openbci_read_pkt(void *param);
int openbci_send_keep_alive_pkt(void *param);
int openbci_send_pkt(void *param);
int openbci_translate_pkt(void *param);
int openbci_process_pkt(void * param);
int openbci_connect_dev(void *param);
int openbci_cleanup(void *param);
