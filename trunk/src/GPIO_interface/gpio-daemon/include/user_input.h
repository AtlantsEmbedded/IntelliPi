/**
 * @file user_input.h
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

#define INPUT_ERROR_MSG debug_msg("You have entered invalid input.\n\
Please choose one of {q, s, b, c, a, d}\n\
Enter input:\n");

int user_input_manager();
inline int wait_for_input();
inline int wait_for_arrows();
inline int wait_for_select();
