/**
 * @file debug.c
 * @author Ron Brash (ron.brash@gmail.com)
 * @brief Debugging functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <ctype.h>

#include "debug.h" 

/**
 * hexdump(void *mem, unsigned int len)
 * @brief Dump buffer
 * @param mem
 * @param len
 */
void hexdump(void *mem, unsigned int len)
{
	unsigned int i, j;

	for (i = 0; i < len + ((len % HEXDUMP_COLS) ? (HEXDUMP_COLS - len % HEXDUMP_COLS) : 0); i++) {
		/* print offset */
		if (i % HEXDUMP_COLS == 0) {
			printf("0x%06x: ", i);
		}

		/* print hex data */
		if (i < len) {
			printf("%02x ", 0xFF & ((unsigned char *)mem)[i]);
		} else {	/* end of block, just aligning for ASCII dump */

			printf("   ");
		}

		/* print ASCII dump */
		if (i % HEXDUMP_COLS == (HEXDUMP_COLS - 1)) {
			for (j = i - (HEXDUMP_COLS - 1); j <= i; j++) {
				if (j >= len) {	/* end of block, not really printing */
					putchar(' ');
				} else if (isprint(((unsigned char *)mem)[j])) {	/* printable unsigned char */
					putchar(0xFF & ((unsigned char *)mem)[j]);
				} else {	/* other unsigned char */

					putchar('.');
				}
			}
			putchar('\n');
		}
	}
}
