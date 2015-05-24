/**
 * @file socket.c
 * @author Ron Brash (ron.brash@gmail.com)
 * @brief Contains all socket inforamtion for BLE connection for device 
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include "socket.h"

int sock = 0;

/**
 * get_socket()
 * @param Returns socket to be used to connect to device
 * @return sock
 */ 
int get_socket(void) {
	return sock;
}

/**
 * setup_socket(char addr_mac[])
 * @brief Sets up socket file descriptor
 * @return 0 for success, -1 for error
 */ 
int setup_socket(char addr_mac[]) {

	struct sockaddr_rc addr = { 0 };
	int status;

	sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	addr.rc_family = AF_BLUETOOTH;
	addr.rc_channel = 1;
	str2ba(addr_mac, &addr.rc_bdaddr);

	status = connect(get_socket(), (struct sockaddr *)&addr, sizeof(addr));
	
	if (status != 0) {
		return (-1);
	}
	
	return (0);
}
/**
 * close_sockets()
 * @brief Closes file descriptor for socket communication
 */
void close_sockets(void)
{
	close(get_socket());
	fprintf(stdout, "Close sockets\n");
}
