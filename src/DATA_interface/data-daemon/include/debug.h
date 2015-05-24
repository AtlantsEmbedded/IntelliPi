/**
 * @file debug.h
 * @author Ron Brash (ron.brash@gmail.com)
 * @brief Hexdump/debugging header 
 */

#ifndef HEXDUMP_COLS
#define HEXDUMP_COLS 8
#endif

void hexdump(void *mem, unsigned int len);
