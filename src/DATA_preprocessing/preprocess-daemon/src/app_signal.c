/**
 * @file app_signal.c
 * @author Ron Brash (ron.brash@gmail.com)
 * @brief Contains Signal related functions 
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "app_signal.h"
#include "main.h"

/**
 * ctrl_c_handler(int signal)
 * @brief Ctrl C signal handler
 * @param signal
 */ 
void ctrl_c_handler(int signal)
{
	fprintf(stdout, "Interrupt caught[NO: %d ]\n", signal);
	cleanup_app();
	exit(0);
}
