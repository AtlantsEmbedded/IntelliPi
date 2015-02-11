/**
 * @file socket.h
 * @author Ronnie Brash (ron.brash@gmail.com)
 * @copy Menu system for Atom Prototype
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ezxml.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <glib.h>

#define VERSION 1
#define UX_SOCK_PATH "/tmp/gpio_socket"

typedef struct ipc_msg_s {
	uint8_t msg_version;
	uint8_t msg_type;
	char top_row[16];
	char bot_row[16];
} ipc_msg_t;

#define IPC_TYPE_MSG_ONLY 0
#define IPC_TYPE_MSG_EXTD 1
#define IPC_TYPE_MSG_OOPS 2
#define IPC_TYPE_COLOR_NONE 10
#define IPC_TYPE_COLOR_RED 11
#define IPC_TYPE_COLOR_GREEN 12
#define IPC_TYPE_COLOR_YELLOW 13
#define IPC_TYPE_COLOR_BLUE 14
#define IPC_TYPE_FINISHED 20
#define IPC_TYPE_ERROR 21
#define IPC_TYPE_CANCEL 22

static inline void print_usage();
static inline int check_argc(int argc, char **argv);
static inline void build_ipc_msg(ipc_msg_t * msg, uint8_t version, uint8_t type, char *top_row, char *bot_row);
static int setup_sock_and_send(ipc_msg_t * msg);
