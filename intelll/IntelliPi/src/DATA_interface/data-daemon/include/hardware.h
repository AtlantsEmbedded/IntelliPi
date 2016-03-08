/**
 * @file hardware.h
 * @author Ron Brash (ron.brash@gmail.com)
 * @brief Hardware header 
 */

#define INIT_HARDWARE_FC() \
		_INIT_HARDWARE_FC(NULL)

#define KEEP_ALIVE_FC() \
		_KEEP_ALIVE_FC(NULL)

#define SEND_PKT_FC(param) \
		_SEND_PKT_FC(param)

#define RECV_PKT_FC() \
		_RECV_PKT_FC(NULL)

#define TRANS_PKT_FC(param) \
		_TRANS_PKT_FC(param)

#define PROCESS_PKT_FC(param) \
		_PROCESS_PKT_FC(param)

#define DEVICE_CONNECTION_FC(param) \
		_DEVICE_CONNECTION_FC(param)

#define DEVICE_CLEANUP_FC() \
		_DEVICE_CLEANUP_FC(NULL)

typedef int (*functionPtr_t) (void *);
functionPtr_t _INIT_HARDWARE_FC;
functionPtr_t _KEEP_ALIVE_FC;
functionPtr_t _SEND_PKT_FC;
functionPtr_t _RECV_PKT_FC;
functionPtr_t _TRANS_PKT_FC;
functionPtr_t _PROCESS_PKT_FC;
functionPtr_t _DEVICE_CONNECTION_FC;
functionPtr_t _DEVICE_CLEANUP_FC;

typedef struct param_s {
	char *ptr;
	uint16_t len;
} param_t;

int init_hardware(char *hardware_type);
