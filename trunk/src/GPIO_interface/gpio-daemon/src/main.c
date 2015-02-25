/**
 * @file main.c
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

#include <include/main.h>
#include <include/xml.h>
#include <include/user_input.h>
#include <include/menu_structure.h>
#include <include/menu_item.h>
#include <include/socket.h>
#include <include/event.h>
#include <include/hardware.h>

#define CONFIG "config/gpio_config.xml"

inline void print_instructions(app_info_t * app_info);
inline char *which_config(int argc, char **argv);

/**
 * print_instructions(app_info_t *app_info)
 * @brief print app instructions
 * @param app_info
 */
inline void print_instructions(app_info_t * app_info)
{
	CLEAR_DISPLAY();
	char msg_str[32] = { 0 };
	memcpy(msg_str, app_info->name, 16);
	memcpy(msg_str + 16, app_info->text, 16);
	OUTPUT_TO_DISPLAY(&msg_str);

}

/**
 * which_config(int argc, char **argv)
 * @brief return which config to use
 * @param argc
 * @param argv
 * @return string of config
 */
inline char *which_config(int argc, char **argv)
{

	if (argc == 2) {
		return argv[1];
	} else {
		return CONFIG;
	}
}

/**
 * main(int argc, char **argv)
 * @brief Main function
 * @param argc
 * @param argv
 * @return 0 for success, -1 for error
 */
int main(int argc, char **argv)
{
	app_info_t app_info;
	memset(&app_info, 0, sizeof(app_info_t));

	/// Check if menu XML exists
	if (XML_exists(which_config(argc, argv)) < 0) {
		return (-1);
	}
	/// Initialize data structures for menu
	if (initialize_menu_structure() != 0) {
		return (-1);
	}
	/// Parse menu XML file
	if (parse_menu_XML(which_config(argc, argv), &app_info) != 0) {
		return (-1);
	}
	/// Initialize IPC socket and epolling
	if (initialize_ipc_sock() < 0) {
		return (-1);
	}
	
	/// Setup hardware function pointers
	if(setup_hardware() < 0) {
		debug_msg("Unable to configure hardware\n");
		exit (-1);
	}
	
	/// Initialize hardware abstraction layer & LCD
	INIT_HARDWARE();
	
	/// Print Banner & Instructions
	print_instructions(&app_info);
	
	/// Wait for select
	wait_for_select();
	
	/// Wait for arrows
	
	/// Step 1 set menu item to child of root
	set_current_menu_item(g_node_first_child(return_root_node()));
	
	/// Step 2 print node
	print_item_text(get_current_node());
	
	/// Step 3 wait for left or right "arrow" button to be pressed
	wait_for_arrows();
	
	/// Step 4 set node to first child of this parent
	set_current_menu_item(g_node_first_child(get_current_node()));

	/*
	 * Dump complete menu data structure if needed
	 * dump_menu_structure(return_root_node());
	 */

	/// Control loop to begin normal menu operation
	for (;;) {
		int res = 0;
		res = user_input_manager();
		switch (res) {
		case QUIT_EVENT:
			return (1);
			break;
		default:
			break;
		}
	}

	return 0;
}
