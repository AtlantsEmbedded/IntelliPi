/**
 * @file menu_item.h
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

GNode *initialize_menu_item(ezxml_t menu_item);
void dump_menu_item(menu_item_t * menu_item_obj,uint8_t level);
