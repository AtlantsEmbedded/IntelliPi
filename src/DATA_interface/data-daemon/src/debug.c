#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include "main.h"
#include "socket.h"
#include "hardware.h"
#include "xml.h"
#include "debug.h"

void hex_dump(unsigned char *buffer, unsigned long index, unsigned long width) {
	unsigned long i;
	for (i=0; i<index; i++) {
		printf("%02x ", buffer[i]);
	}
	unsigned long spacer;
	for (spacer = index; spacer < width; spacer++) {
		printf("        ");
		
	}
	printf(": ");
	for(i=0;i<index;i++){
		if(buffer[i] < 32) {
			printf(".");
		} else {
			printf("%c", buffer[i]);
		}
		printf("\n");
	}
}
