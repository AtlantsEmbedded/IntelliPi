/**
 * @file xml.h
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

int XML_exists(char *file);
int parse_menu_XML(char *file, app_info_t * app_info);
