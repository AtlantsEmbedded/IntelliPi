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
#include <include/hardware.h>

/**
 * user_input_manager()
 * @brief Handles input manager - this will be a complete wrapper function
 * eventually
 * @return res
 */
int user_input_manager()
{
	// Clear console
	CLEAR_DISPLAY();

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
	return WAIT_FOR_INPUT();
}

/**
 * wait_for_arrows()
 * @brief wait for arrow inputs to be pressed
 * @return res
 */
inline int wait_for_arrows()
{
	return WAIT_FOR_INPUT_ARROWS();

}

/**
 * wait_for_select()
 * @brief wait for select inputs to be pressed
 * @return res
 */
inline int wait_for_select()
{
	return WAIT_FOR_INPUT_SELECT();
}
