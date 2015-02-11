/**
 * @file user_input.c
 * @author Ronnie Brash (ron.brash@gmail.com)
 * @brief This source file will manage input which may come from who 
 * knows what source: cmd or SPI at this time/case
 * @copy Menu system for Atom Prototype
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ezxml.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>

#include <include/main.h>
#include <include/menu_structure.h>
#include <include/menu_item.h>
#include <include/user_input.h>
#include <include/socket.h>
#include <include/event.h>

const char accepted_input_chars[] = { 's', 'b', 'c', 'q', 'a', 'd' };

#define SELECT_CHAR accepted_input_chars[0]
#define BACK_CHAR accepted_input_chars[1]
#define CANCEL_CHAR accepted_input_chars[2]
#define QUIT_CHAR accepted_input_chars[3]
#define PREV_CHAR accepted_input_chars[4]
#define NEXT_CHAR accepted_input_chars[5]

/// Console related functions for testing from the command line
static int wait_for_select_keys();
static int wait_for_arrow_keys();
static int wait_for_console_input_keys();

/**
 * user_input_manager()
 * @brief Handles input manager - this will be a complete wrapper function
 * eventually
 * @return res
 */
int user_input_manager()
{
	// Clear console
	clear_console();

	// Print current information about node
	print_item_text(get_current_node());
	print_item_menu(get_current_node());

	int res = 0;
	res = wait_for_input();

	switch (res) {
	case NEXT_EVENT:
		set_current_menu_item(get_next_sibling_node(get_current_node()));
		break;
	case PREV_EVENT:
		set_current_menu_item(get_prev_sibling_node(get_current_node()));
		break;
	case SELECT_EVENT:
		execute_menu_item(get_current_node());
		set_current_menu_item(get_current_node());
		break;
	case CANCEL_EVENT:
		cancel_menu_item(get_current_node());
		break;
	case FINISH_EVENT:
		finished_menu_item(get_current_node());
		break;
	case QUIT_EVENT:
		break;
	default:
		break;
	}

	return res;
}

/**
 * wait_for_input()
 * @brief Waits for user input
 * @return -1 for error (should never happen), 
 */
inline int wait_for_input()
{
	return wait_for_console_input_keys();
}

/**
 * wait_for_arrows()
 * @brief wait for arrow inputs to be pressed
 * @return res
 */
inline int wait_for_arrows()
{
	set_current_menu_item(g_node_first_child(return_root_node()));

	clear_console();
	print_item_text(get_current_node());
	print_item_menu(get_current_node());

	int res = 0;
	res = wait_for_arrow_keys();
	if (res == NEXT_EVENT) {
		/// Set node to first node of the main menu
		set_current_menu_item(g_node_first_child(get_current_node()));
	} else if (res == PREV_EVENT) {
		/// Set node to first node of the main menu
		set_current_menu_item(g_node_last_child(get_current_node()));
	}
	return res;

}

/**
 * wait_for_select()
 * @brief wait for select inputs to be pressed
 * @return res
 */
inline int wait_for_select()
{
	return wait_for_select_keys();
}

/**
 * wait_for_console_input()
 * @brief Waits for user input on console
 * @return -1 for error (should never happen), 
 */
static int wait_for_console_input_keys()
{

	char buff[2] = { 0 };

	debug_msg("Enter input:\n");
	int res = 0;
	for (;;) {

		memset(buff, 0, sizeof(buff));
		buff[1] = '\0';

		if (fgets(buff, sizeof(buff), stdin) == NULL) {
			if (puts("EOF or read error\n") == EOF) {
				continue;
			}
		} else {
			errno = 0;
			if (buff[0] == '\0' || buff[0] == '\n') {
				continue;
			}
			if (isalpha(buff[0]) != 0) {
				if (buff[0] == QUIT_CHAR) {
					debug_msg("Quitting...\n");
					res = QUIT_EVENT;
					break;
				} else if (buff[0] == SELECT_CHAR) {
					debug_msg("Selecting option\n");
					res = SELECT_EVENT;
					break;
				} else if (buff[0] == BACK_CHAR) {
					debug_msg("Returning to previous menu\n");
					res = BACK_EVENT;
					break;
				} else if (buff[0] == CANCEL_CHAR) {
					debug_msg("Canceling task\n");
					res = CANCEL_EVENT;
					break;
				} else if (buff[0] == NEXT_CHAR) {
					debug_msg("Next task\n");
					res = NEXT_EVENT;
					break;
				} else if (buff[0] == PREV_CHAR) {
					debug_msg("Previous task\n");
					res = PREV_EVENT;
					break;
				} else {
					INPUT_ERROR_MSG;
					continue;
				}
			} else {
				INPUT_ERROR_MSG;
				continue;
			}

		}

	}
	return res;
}

/**
 * wait_for_select_keys()
 */
static int wait_for_select_keys()
{

	char buff[2] = { 0 };

	debug_msg("Enter input {s,q}:\n");
	int res = 0;
	for (;;) {

		memset(buff, 0, sizeof(buff));
		buff[1] = '\0';

		if (fgets(buff, sizeof(buff), stdin) == NULL) {
			if (puts("EOF or read error\n") == EOF) {
				continue;
			}
		} else {
			errno = 0;
			if (buff[0] == '\0' || buff[0] == '\n') {
				continue;
			}
			if (isalpha(buff[0]) != 0) {
				if (buff[0] == QUIT_CHAR) {
					debug_msg("Quitting...\n");
					res = QUIT_EVENT;
					break;
				} else if (buff[0] == SELECT_CHAR) {
					debug_msg("Selecting event\n");
					res = SELECT_EVENT;
					break;
				} else {
					continue;

				}

			}
		}
	}
	return res;

}

/**
 * wait_for_arrow_keys()
 * @brief wait for arrow keys to be pressed
 * @return {QUIT_EVENT, PREV_EVENT, NEXT_EVENT }
 */
static int wait_for_arrow_keys()
{

	char buff[2] = { 0 };

	debug_msg("Enter input {a,d,q}:\n");
	int res = 0;
	for (;;) {

		memset(buff, 0, sizeof(buff));
		buff[1] = '\0';

		if (fgets(buff, sizeof(buff), stdin) == NULL) {
			if (puts("EOF or read error\n") == EOF) {
				continue;
			}
		} else {
			errno = 0;
			if (buff[0] == '\0' || buff[0] == '\n') {
				continue;
			}
			if (isalpha(buff[0]) != 0) {
				if (buff[0] == QUIT_CHAR) {
					debug_msg("Quitting...\n");
					res = QUIT_EVENT;
					break;
				} else if (buff[0] == NEXT_CHAR) {
					debug_msg("Next task\n");
					res = NEXT_EVENT;
					break;
				} else if (buff[0] == PREV_CHAR) {
					debug_msg("Previous task\n");
					res = PREV_EVENT;
					break;
				} else {
					continue;

				}

			}
		}
	}
	return res;

}
