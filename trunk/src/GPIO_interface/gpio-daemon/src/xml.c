/**
 * @file xml.c
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
#include <include/menu_structure.h>
#include <include/menu_item.h>

const char *XML_app_elements[] = { "name", "text" };

static int parse_menu_item(ezxml_t menu_item_input, GNode * cur_node);
static int parse_menu_items(ezxml_t menu_item_input, GNode * cur_node);
static int get_app_attributes(ezxml_t app_attribute, app_info_t * app_info);
static int sanity_check_app_attributes(ezxml_t app_attribute);

/**
 * XML_exists(char *file)
 * @brief Checks to see if a file exists
 * @param file
 * @return 0 for success, -1 for error
 */
int XML_exists(char *file)
{
	FILE *fp = NULL;
	if ((fp = fopen(file, "r")) == NULL) {
		printf("Error opening file\n");
		return (-1);
	}

	if (fclose(fp) != 0) {
		fp = NULL;
		printf("Error closing file\n");
		return (-1);
	}

	fp = NULL;

	return (0);

}

/**
 * sanity_check_app_attributes(ezxml_t menu_item)
 * @brief Provide a quick sanity check to make sure that there are 
 * at least the minimum number of elements present in the 
 * app attributes element
 * @param menu_item
 * @return 0 for success, -1 for error
 */
static int sanity_check_app_attributes(ezxml_t app_attribute) {
	
	int elements = (sizeof(XML_app_elements) / sizeof(XML_app_elements[0]));
	int i = 0;
	for (i = 0; i < elements; i++) {
		ezxml_t test = ezxml_child(app_attribute, XML_app_elements[i]);
		if (test == NULL) {
			debug_msg("element is missing: %s\n", XML_app_elements[i]);
			return (-1);
		}
	}

	return (0);
	
}

/**
 * get_app_attributes(ezxml_t app_attribute, app_info_s * app_info)
 * @brief parse menu attributes
 * @param app_attribute
 * @param app_info
 * @return < 0 for error, 0 for success
 */
static int get_app_attributes(ezxml_t app_attribute, app_info_t * app_info)
{

	// Quick sanity check of app attributes element in XML
	if(sanity_check_app_attributes(app_attribute) < 0) {
		printf("appAttributes has an error\n");
		return (-1);
	}
	ezxml_t tmp = ezxml_child(app_attribute, "name");

	// Get app name
	if (tmp == NULL) {
		printf("appAttributes->name is missing\n");
		return (-1);
	}
	memcpy(app_info->name, tmp->txt, LCD_STR_LEN);

	// Get app text
	tmp = ezxml_child(app_attribute, "text");
	if (tmp == NULL) {
		printf("appAttributes->text is missing\n");
		return (-1);
	}

	memcpy(app_info->text, tmp->txt, LCD_STR_LEN);

	return (0);
}

/**
 * parse_menu_items(ezxml_t menu_item_input)
 * @brief parse menuItems element for children menuItem(s)
 * @param menu_item_input
 * @param app_info
 * @return < 0 for error, 0 for success
 */
static int parse_menu_items(ezxml_t menu_item_input, GNode * cur_node)
{

	int err = 0;
	ezxml_t menu_item = NULL;
	GNode *stored_cur_node = cur_node;

	for (menu_item = ezxml_child(menu_item_input, "menuItem"); menu_item; menu_item = menu_item->next) {

		if ((err = parse_menu_item(menu_item,stored_cur_node)) < 0) {
			break;
		}

	}

	return err;
}

/**
 * parse_menu_item(ezxml_t menu_item)
 * @brief parse menu item
 * @param menu_item
 * @return < 0 for error, 0 for success
 */
static int parse_menu_item(ezxml_t menu_item, GNode * cur_node)
{

	if (menu_item == NULL) {
		return (0);
	}

	ezxml_t hasChildren = ezxml_child(menu_item, "hasChildren");
	ezxml_t children = ezxml_child(menu_item, "children");
	GNode *new_menu_node = NULL;

	if ((new_menu_node = initialize_menu_item(menu_item)) == NULL) {
		debug_msg("Error creating menu_item { XML or malloc }\n");
		return (-1);
	}
	
	if(add_menu_node(cur_node,new_menu_node) == NULL) {
		debug_msg("Unable to add new menu item to data-structure\n");
		return (-1);		
	}

	if (hasChildren == NULL) {
		debug_msg("Has no children element - could be an error\n");
		return (0);
	}
	if (memcmp(hasChildren->txt, "true", 4) == 0) {

		if (children != NULL) {
			parse_menu_items(children, new_menu_node);
		}

	}

	return (0);
}

/**
 * parse_menu_XML(char *file, app_info_s * app_info)
 * @brief
 * @param file
 * @param app_info
 * @return < 0 for error, 0 for success
 */
int parse_menu_XML(char *file, app_info_t * app_info)
{

	ezxml_t app_config = ezxml_parse_file(file);
	ezxml_t app_menu = NULL, app_attributes = NULL;
	int err = 0;

	// Are app attributes defined?
	if ((app_attributes = ezxml_child(app_config, "appAttributes")) == NULL) {
		printf("appAttributes is missing\n");
		return (-1);
	}
	// Parse application attributes from XML
	if (get_app_attributes(app_attributes, app_info) < 0) {
		printf("appAttributes error\n");
		return (-1);
	}

	// Parse appMenu for menuItems
	for (app_menu = ezxml_child(app_config, "appMenu"); app_menu; app_menu = app_menu->next) {

		err = parse_menu_items(app_menu,return_root_node());
		if (err < 0) {
			break;
		}
	}

	ezxml_free(app_config);
	return err;
}
