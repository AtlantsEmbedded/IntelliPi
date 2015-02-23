/**
 * @file menu_structure.c
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
#include <include/hardware.h>

GNode *root = NULL;
GNode *current_node = NULL;

/**
 * initialize_menu_structure()
 * @brief initialize GLIB2 N-array data structure for use
 * with the menu system
 * @return 0 for success, -1 for error
 */
int initialize_menu_structure()
{
	menu_item_t *root_node = (menu_item_t *) malloc(sizeof(menu_item_t));

	if (root_node) {
		memset(root_node, 0, sizeof(menu_item_t));
		memcpy(&root_node->item_name, "root node", LCD_STR_LEN);
		root = g_node_new(root_node);
		return (0);
	}

	return (-1);
}

/**
 * set_current_menu_item(GNode *new_node)
 * @brief Set current node
 * @param new_node
 */
inline void set_current_menu_item(GNode * new_node)
{

	current_node = new_node;
}

/**
 * get_current_node() 
 * @brief Return current node
 * @return current_node
 */
inline GNode *get_current_node()
{

	return current_node;
}

/**
 * return_root_node()
 * @brief return root GNode
 * @return root ptr or NULL if not allocated/created
 */
GNode *return_root_node()
{
	return root;
}

/**
 * add_menu_node(GNode * cur_node, GNode * new_node)
 * @brief adds menu item node to the N-array tree
 * @param cur_node
 * @param new_node
 */
GNode *add_menu_node(GNode * cur_node, GNode * new_node)
{
	return g_node_append(cur_node, new_node);
}

/**
 * get_next_sibling_node(GNode * cur_node)
 * @brief Gets next sibling node, or wraps around if NULL
 * @param cur_node
 * @return GNode of sibling next, or wrapped around
 */
GNode *get_next_sibling_node(GNode * cur_node)
{
	GNode *tmp_node = NULL;
	if ((tmp_node = g_node_next_sibling(cur_node)) == NULL) {
		return g_node_first_sibling(cur_node);
	}
	return tmp_node;
}

/**
 * get_previous_sibling_node(GNode * cur_node)
 * @brief Gets previous sibling node, or wraps around if NULL
 * @param cur_node
 * @return GNode of sibling previous, or wrapped around
 */
GNode *get_prev_sibling_node(GNode * cur_node)
{
	GNode *tmp_node = NULL;
	if ((tmp_node = g_node_prev_sibling(cur_node)) == NULL) {
		return g_node_last_sibling(cur_node);
	}
	return tmp_node;
}

/**
 * cleanup_menu_structure()
 * @brief Cleanup for N-array tree
 * @return 0 for success, -1 for error
 */
int cleanup_menu_structure()
{
	if (root != NULL) {
		g_node_destroy(root);
		return (0);
	}
	return (-1);
}

/**
 * recursive_menu_dump(GNode * node, uint8_t level)
 */
static void recursive_menu_dump(GNode * node, uint8_t level)
{
	uint8_t level_tmp = level;
	if (node->children) {
		GNode *tmp = g_node_first_child(node);
		while (tmp != NULL) {
			dump_menu_item((menu_item_t *) tmp->data, level_tmp++);
			if (tmp->children) {
				recursive_menu_dump(tmp, level_tmp++);
			}
			tmp = g_node_next_sibling(tmp);

		}

	} else {
		dump_menu_item((menu_item_t *) node->data, level);
	}
}

/**
 * dump_menu_structure(GNode * node)
 * @brief Dumps menu structure with printfs
 * @param node
 * @note relies on dump_menu_item() which is provided by menu_item.c
 */
void dump_menu_structure(GNode * node)
{

	if (node == NULL) {
		debug_msg("Unable to dump tree - root NULL\n");
		return;
	}

	recursive_menu_dump(node, 0);

}

/**
 * print_item_text(GNode * node)
 * @brief print current node menu text
 * @param node
 */
void print_item_text(GNode * node)
{
	menu_item_t *item = (menu_item_t *) node->data;
	debug_msg("\nMENU ITEM-----------------------\n");
	debug_msg("  %s\n", item->item_name);
	debug_msg("  %s\n", item->item_text);

	char msg_str[32] = { 0 };
	memcpy(msg_str, item->item_name, 16);
	memcpy(msg_str + 16, item->item_text, 16);
	_OUTPUT_TO_DISPLAY(&msg_str);
}

/**
 * print_item_menu(GNode * node)
 * @brief print current node menu possibilities
 * @param node
 */
void print_item_menu(GNode * node)
{
#ifdef DEBUG
	menu_item_t *item = (menu_item_t *) node->data;

	debug_msg("\nMENU OPTIONS-------------------\n");
	if (item->is_selectable) {
		debug_msg(" * selectable (s)\n");
	}
	if (item->is_backable) {
		debug_msg(" * backable (b)\n");
	}
	if (item->is_cancelable) {
		debug_msg(" * cancelable (c)\n");
	}
	debug_msg(" * left (a)\n");
	debug_msg(" * right (d)\n");

	debug_msg(" * quit (q)\n");

	debug_msg("\n");
#endif
}
