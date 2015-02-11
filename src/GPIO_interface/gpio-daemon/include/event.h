/**
 * @file event.h
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

#define NORMAL_MSG 5
#define EXTEND_MSG 10
#define OOPS_MSG 65535

int execute_menu_item(GNode * node);
int cancel_menu_item(GNode * node);
int finished_menu_item(GNode * node);

int execute_ipc_event(ipc_msg_t * msg);
