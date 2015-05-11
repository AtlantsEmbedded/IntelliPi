/**
 * @file main.c
 * @author Ron Brash (ron.brash@gmail.com) | Atom Embedded
 * @brief Handles the data interface layer which acts as an abstraction
 * interface for BLE/Bluetooth data and converts data into a usable format
 * for other applications.
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

#include "main.h"
#include "socket.h"
#include "hardware.h"
#include "xml.h"
#include "debug.h"


#define KEEP_ALIVE "k\r\n"
#define NOTCH_FREQ "g 407c\r\n" // 60hz
#define SET_HOST_PLATFORM "r 5\r\n" //linux
#define PRESET "% 10\r\n" // no accell or battery
#define START_TRANSMISSION "s\r\n"
#define HALT_TRANSMISSION "h\r\n"
#define VERSION "v\r\n"

int s;
void ctrl_c_handler(int signal);
void close_sockets();
void *read_packet();
void *send_keep_alive_pkt();
void *send_pkt();

int main(int argc, char **argv)
{
	(void)signal(SIGINT, ctrl_c_handler);

	pthread_t readT, writeT;
	int iret1, iret2;

	struct sockaddr_rc addr = { 0 };
	int status;
	char dest[18] = "00:06:66:6C:16:54";

	//allocate a socket 
	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	addr.rc_family = AF_BLUETOOTH;
	addr.rc_channel = 1;
	str2ba(dest, &addr.rc_bdaddr);

	//connect to server 
	status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

	//send a message 
	if (0 == status) {
		printf("connect success\n");
		
		iret1 = pthread_create(&readT, NULL, read_packet, NULL);
		iret2 = pthread_create(&writeT, NULL, send_keep_alive_pkt, NULL);

		pthread_join(readT, NULL);
		pthread_join(writeT, NULL);
		send_pkt();

	} else {
		printf("Unable to connect to hardware\n");
	}

	close_sockets();
	return 0;
}


/**
 * send_keep_alive_pkt(void)
 * @brief Sends a keep alive repeatedly while sleeping for the required 
 * time-out period.
 */ 
void *send_keep_alive_pkt(void)
{
	const char *msg= "k\r\n";
	int status;

	do {
		status = send(s, msg, 3, 0);
		sleep(10);
	} while (status > 0);
}

/**
 * send_pkt(char *msg)
 * @brief Sends a keep alive repeatedly while sleeping for the required 
 * time-out period.
 */ 
void *send_pkt(char *msg)
{
	int status;

	status = send(s, "s\r\n", 3, 0);
	printf("wrote & status %d\n",status);

}

/**
 * read_packet(void)
 * @brief Reads incoming packets from the socket
 */ 
void *read_packet(void)
{
	int bytes_read;
	char buf[1024] = { 0 };
	printf("reading\n");
	do {
		memset(buf, 0, 1024);
		bytes_read = recv(s, buf, sizeof(buf), 0);
		fprintf(stdout, "Bytes read = %d\n", bytes_read);
		if (bytes_read <= 0)
			break;
		hex_dump(buf,0,bytes_read);
	} while (1);
}

void close_sockets()
{
	close(s);
	fprintf(stdout, "Close sockets\n");
}

void ctrl_c_handler(int signal)
{
	fprintf(stdout, "Interrupt caught[NO: %d ]\n", signal);
	close_sockets();
	exit(0);
}
