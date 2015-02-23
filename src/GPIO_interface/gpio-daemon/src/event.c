/**
 * @file event.c
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
#include <unistd.h>

#include <include/main.h>
#include <include/xml.h>
#include <include/socket.h>
#include <include/menu_structure.h>
#include <include/menu_item.h>
#include <include/event.h>
#include <include/hardware.h>

static inline void dump_ipc_message(ipc_msg_t * msg);
static inline void print_ipc_message(ipc_msg_t * msg, unsigned int seconds);

/**
 * execute_menu_item(GNode *node)
 * @brief Execute whatever it is that a particular menu item needs
 * to do
 * @param node
 * @return 0 for success, -1 for error
 */
int execute_menu_item(GNode * node)
{

	menu_item_t *item = (menu_item_t *) node->data;
	char msg_str[32] = { 0 };

	if ((item->is_actionable == 1) && (strlen((char *)item->action) > 0)) {
		if (system(item->action) < 0) {
			debug_msg("Error executing system command\n");
			return (-1);
		}
		memcpy(msg_str, item->item_name, 16);
		memcpy(msg_str + 16, item->action, 16);
		_OUTPUT_TO_DISPLAY(&msg_str);
	}

	return (0);
}

/**
 * cancel_menu_item(GNode *node)
 * @brief Cancel whatever it is that a particular menu item needs
 * to do
 * @param node
 * @return 0 for success, -1 for error
 */
int cancel_menu_item(GNode * node)
{

	menu_item_t *item = (menu_item_t *) node->data;
	char msg_str[32] = { 0 };

	if ((item->is_cancelable == 1) && (strlen((char *)item->cancel_action) > 0)) {
		if (system(item->cancel_action) < 0) {
			debug_msg("Error executing canceling system command\n");
			return (-1);
		}
		memcpy(msg_str, item->item_name, 16);
		memcpy(msg_str + 16, item->cancel_action, 16);
		_OUTPUT_TO_DISPLAY(&msg_str);
	}

	return (0);
}

/**
 * finished_menu_item(GNode *node)
 * @brief Finished whatever it is that a particular menu item needs
 * to do
 * @param node
 * @return 0 for success, -1 for error
 */
int finished_menu_item(GNode * node)
{

	menu_item_t *item = (menu_item_t *) node->data;
	char msg_str[32] = { 0 };

	if ((item->is_finishable == 1) && (strlen((char *)item->finish_text) > 0)) {
		if (system(item->cancel_action) < 0) {
			debug_msg("Error executing canceling system command\n");
			return (-1);
		}
		memcpy(msg_str, item->item_name, 16);
		memcpy(msg_str + 16, item->finish_text, 16);
		_OUTPUT_TO_DISPLAY(&msg_str);
		sleep(5);
	}

	return (0);
}

/**
 * error_menu_item(GNode *node)
 * @brief What to do if an error is reported via IPC
 * @param node
 * @return 0 for success, -1 for error
 */
int error_menu_item(GNode * node)
{

	menu_item_t __attribute__ ((unused)) * item = (menu_item_t *) node->data;
	// Stub function

	return (0);
}

/**
 * dump_ipc_message(ipc_msg_t *msg)
 * @brief dump ipc message if needded
 * @param msg
 */
static inline void dump_ipc_message(ipc_msg_t * msg)
{
	debug_msg("version:%u\n", msg->msg_version);
	debug_msg("type:%u\n", msg->msg_type);
	debug_msg("top_msg:%s\n", msg->top_row);
	debug_msg("bot_msg:%s\n", msg->bot_row);
}

/**
 * print_ipc_message(ipc_msg_t *msg, unsigned int seconds)
 * @brief print out ipc message
 * @param msg
 * @param seconds
 */
static inline void print_ipc_message(ipc_msg_t * msg, unsigned int seconds)
{

	CLEAR_DISPLAY();
	char msg_str[32] = { 0 };
	memcpy(msg_str, msg->top_row, 16);
	memcpy(msg_str + 16, msg->bot_row, 16);
	OUTPUT_TO_DISPLAY(&msg_str);
	sleep(seconds);
}

/**
 * execute_ipc_event(ipc_msg_t * msg)
 * @brief execute IPC event
 * @param msg
 * @return 0 for success, -1 for error
 */
int execute_ipc_event(ipc_msg_t * msg)
{

	if (!msg) {
		return (-1);
	}

	/*
	 * Debug if necessary
	 * dump_ipc_msg(msg);
	 */
	switch (msg->msg_type) {
	case IPC_TYPE_MSG_ONLY:
		print_ipc_message(msg, NORMAL_MSG);
		break;
	case IPC_TYPE_MSG_EXTD:
		print_ipc_message(msg, EXTEND_MSG);
		break;
	case IPC_TYPE_MSG_OOPS:
		print_ipc_message(msg, OOPS_MSG);
		break;
	case IPC_TYPE_COLOR_RED:
		_SET_DISPLAY_COLOR((void *)RED_COLOR);
		break;
	case IPC_TYPE_COLOR_GREEN:
		_SET_DISPLAY_COLOR((void *)GREEN_COLOR);
		break;
	case IPC_TYPE_COLOR_YELLOW:
		_SET_DISPLAY_COLOR((void *)YELLOW_COLOR);
		break;
	case IPC_TYPE_COLOR_BLUE:
		_SET_DISPLAY_COLOR((void *)BLUE_COLOR);
		break;
	case IPC_TYPE_COLOR_NONE:
		_SET_DISPLAY_COLOR((void *)NO_COLOR);	// Turn off backlight for raspi
		break;
	case IPC_TYPE_ERROR:
		error_menu_item(get_current_node());
		break;
	case IPC_TYPE_CANCEL:
		cancel_menu_item(get_current_node());
		break;
	case IPC_TYPE_FINISHED:
		finished_menu_item(get_current_node());
		break;
	default:
		// Assume that the default is just a message
		break;

	}
	print_item_text(get_current_node());
	print_item_menu(get_current_node());

	return (0);

}
