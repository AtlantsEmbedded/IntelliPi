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
#define MAX_EVENTS 10

typedef struct ipc_msg_s {
	uint8_t msg_version;
	uint8_t msg_type;
	char top_row[16];
	char bot_row[16];
} ipc_msg_t;

int initialize_ipc_sock();
