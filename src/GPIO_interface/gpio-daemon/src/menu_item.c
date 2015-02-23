/**
 * @file menu_item.c
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

#include <include/main.h>
#include <include/menu_structure.h>
#include <include/menu_item.h>

inline char *null_or_not(char *input);
inline char *true_or_false(uint8_t value);
static int sanity_check_menu_item(ezxml_t menu_item);

const char *XML_menu_item_elements[] = { "name", "text", "selectable", "actionable",
	"cancelable", "backable", "finishable", "waitForFinish", "hasChildren",
	"action", "actionText", "cancelAction", "finishText", "children"
};


/**
 * sanity_check_menu_item(ezxml_t menu_item)
 * @brief Provide a quick sanity check to make sure that there are 
 * at least the minimum number of elements present in a menu item
 * @param menu_item
 * @return 0 for success, -1 for error
 */

static int sanity_check_menu_item(ezxml_t menu_item)
{
	int elements = (sizeof(XML_menu_item_elements) / sizeof(XML_menu_item_elements[0]));
	int i = 0;
	for (i = 0; i < elements; i++) {
		ezxml_t test = ezxml_child(menu_item, XML_menu_item_elements[i]);
		if (test == NULL) {
			debug_msg("element is missing: %s\n", XML_menu_item_elements[i]);
			return (-1);
		}
	}

	return (0);
}

/**
 * initialize_menu_item(ezxml_t menu_item)
 * @brief initialize single menu item
 * @param menu_item
 * @return 0 for success, else -1
 */
GNode *initialize_menu_item(ezxml_t menu_item)
{
	// Quickly check if all of the elements are there
	if (sanity_check_menu_item(menu_item) < 0) {
		return NULL;
	}

	uint8_t bool_value = 0;
	menu_item_t *menu_item_obj = (menu_item_t *) malloc(sizeof(menu_item_t));
	
	if (!menu_item_obj) {
		return NULL;
	}

	memcpy(menu_item_obj->item_name, ezxml_child(menu_item, "name")->txt, LCD_STR_LEN);
	memcpy(menu_item_obj->item_text, ezxml_child(menu_item, "text")->txt, LCD_STR_LEN);

	if (strncmp(ezxml_child(menu_item, "selectable")->txt, "true", 4) == 0) {
		bool_value = 1;
	} else {
		bool_value = 0;
	}
	memcpy(&menu_item_obj->is_selectable, &bool_value, sizeof(uint8_t));

	if (strncmp(ezxml_child(menu_item, "actionable")->txt, "true", 4) == 0) {
		bool_value = 1;
	} else {
		bool_value = 0;
	}
	memcpy(&menu_item_obj->is_actionable, &bool_value, sizeof(uint8_t));

	if (strncmp(ezxml_child(menu_item, "cancelable")->txt, "true", 4) == 0) {
		bool_value = 1;
	} else {
		bool_value = 0;
	}
	memcpy(&menu_item_obj->is_cancelable, &bool_value, sizeof(uint8_t));

	if (strncmp(ezxml_child(menu_item, "finishable")->txt, "true", 4) == 0) {
		bool_value = 1;
	} else {
		bool_value = 0;
	}
	memcpy(&menu_item_obj->is_finishable, &bool_value, sizeof(uint8_t));

	if (strncmp(ezxml_child(menu_item, "hasChildren")->txt, "true", 4) == 0) {
		bool_value = 1;
	} else {
		bool_value = 0;
	}
	memcpy(&menu_item_obj->has_children, &bool_value, sizeof(uint8_t));

	if (strncmp(ezxml_child(menu_item, "waitForFinish")->txt, "true", 4) == 0) {
		bool_value = 1;
	} else {
		bool_value = 0;
	}
	memcpy(&menu_item_obj->wait_for_finish, &bool_value, sizeof(uint8_t));

	if (strncmp(ezxml_child(menu_item, "backable")->txt, "true", 4) == 0) {
		bool_value = 1;
	} else {
		bool_value = 0;
	}
	memcpy(&menu_item_obj->is_backable, &bool_value, sizeof(uint8_t));

	memcpy(&menu_item_obj->action_text, ezxml_child(menu_item, "actionText")->txt, LCD_STR_LEN);
	memcpy(&menu_item_obj->cancel_text, ezxml_child(menu_item, "cancelText")->txt, LCD_STR_LEN);
	memcpy(&menu_item_obj->finish_text, ezxml_child(menu_item, "finishText")->txt, LCD_STR_LEN);

	memcpy(&menu_item_obj->action, ezxml_child(menu_item, "action")->txt, CMD_STR_LEN);
	memcpy(&menu_item_obj->cancel_action, ezxml_child(menu_item, "cancelAction")->txt, CMD_STR_LEN);

	return g_node_new(menu_item_obj);
}

/**
 * true_or_false(uint8_t value)
 * @brief return static string if true or false
 * @param value
 * @return if value == 1 then TRUE, else FALSE
 */
inline char *true_or_false(uint8_t value)
{

	if (value == 1) {
		return "TRUE";
	} else {
		return "FALSE";
	}

}

/**
 * null_or_not(char *input) 
 * @brief return static string if len < 1 (null because everything
 * should be zero'd)
 * @param input
 * @return if input len > 1 then input, else (NULL)
 */
inline char *null_or_not(char *input)
{
	if (strlen(input) < 1) {
		return "(NULL)";
	} else {
		return input;
	}
}

/**
 * dump_menu_item(menu_item_t * menu_item_obj)
 * @brief Dump menu item structure
 * @param menu_item_obj
 */
void dump_menu_item(menu_item_t * menu_item_obj, uint8_t level)
{

	if (menu_item_obj == NULL) {
		return;
	}

	int i = 0;
	char tabs[10] = "\t";
	for (i = 0; i < level; i++) {
		strncat(tabs, "\t", 1);
	}

	debug_msg("----------------------------------\n");
	debug_msg("%sname: %s\n", tabs, null_or_not(menu_item_obj->item_name));
	debug_msg("%stext: %s\n", tabs, null_or_not(menu_item_obj->item_text));

	debug_msg("%sselectable: %s\n", tabs, true_or_false(menu_item_obj->is_selectable));
	debug_msg("%sactionable: %s\n", tabs, true_or_false(menu_item_obj->is_actionable));
	debug_msg("%scancelable: %s\n", tabs, true_or_false(menu_item_obj->is_cancelable));
	debug_msg("%sbackable: %s\n", tabs, true_or_false(menu_item_obj->is_backable));
	debug_msg("%sfinishable: %s\n", tabs, true_or_false(menu_item_obj->is_finishable));
	debug_msg("%swait_for_finish: %s\n", tabs, true_or_false(menu_item_obj->wait_for_finish));
	debug_msg("%shas_children: %s\n", tabs, true_or_false(menu_item_obj->has_children));

	debug_msg("%saction: %s\n", tabs, null_or_not(menu_item_obj->action));
	debug_msg("%saction_text: %s\n", tabs, null_or_not(menu_item_obj->action_text));

	debug_msg("%scancel_action: %s\n", tabs, null_or_not(menu_item_obj->cancel_action));
	debug_msg("%scancel_text: %s\n", tabs, null_or_not(menu_item_obj->cancel_text));

	debug_msg("%sfinish_text: %s\n", tabs, null_or_not(menu_item_obj->finish_text));
	debug_msg("----------------------------------\n\n");

}
