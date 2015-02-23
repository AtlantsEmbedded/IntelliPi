/**
 * @file menu_structure.h
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

typedef struct menu_attr_s {
	char menu_name[17];
	char menu_text[17];
} menu_attr_t;

typedef struct menu_item_s {
	char item_name[17];
	char item_text[17];
	uint8_t is_selectable;
	uint8_t is_actionable;
	uint8_t is_cancelable;
	uint8_t is_finishable;
	uint8_t is_backable;
	uint8_t has_children;
	uint8_t wait_for_finish;
	char action[256];
	char cancel_action[256];
	char action_text[17];
	char cancel_text[17];
	char finish_text[17];
} menu_item_t;

int initialize_menu_structure();
int cleanup_menu_structure();
void dump_menu_structure(GNode * root_of_tree);
	
void print_item_text(GNode *node);
void print_item_menu(GNode *node);

inline void set_current_menu_item(GNode *new_node);
inline GNode *get_current_node();

GNode *add_menu_node(GNode *cur_node, GNode * new_node);
GNode *get_next_sibling_node(GNode *cur_node);
GNode *get_prev_sibling_node(GNode *cur_node);
GNode *return_root_node();
