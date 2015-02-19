/**
 * @file main.h
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
#include <stdarg.h>

#define QUIT_EVENT 0
#define SELECT_EVENT 1
#define BACK_EVENT 2
#define CANCEL_EVENT 3
#define NEXT_EVENT 4
#define PREV_EVENT 5
#define FINISH_EVENT 6

#define DEBUG 1

#define STR_LEN 17
#define LCD_STR_LEN 16
#define CMD_STR_LEN 255

#define debug_msg(...) \
            do { if (DEBUG) printf(__VA_ARGS__); } while (0)

typedef struct app_info_s {
	char name[17];
	char text[17];
	uint8_t id;
} app_info_t;
