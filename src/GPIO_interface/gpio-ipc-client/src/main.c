/**
 * @file event.h
 * @author Ronnie Brash (ron.brash@gmail.com)
 * @brief IPC client that communicates with the menu system 
 * for the Atom Prototype
 * @copy Atom Products
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ezxml.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <glib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <include/socket.h>

#define USAGE " gpio_ipc_client type{0-255} top_row_str[0-16] bot_row_str[0-16]"
#define TYPE_CODES " IPC_TYPE_MSG_ONLY 0 \n\
 IPC_TYPE_MSG_EXTD 1 \n\
 IPC_TYPE_MSG_OOPS 2 \n\
 IPC_TYPE_COLOR_NONE 10 \n\
 IPC_TYPE_COLOR_RED 11 \n\
 IPC_TYPE_COLOR_GREEN 12 \n\
 IPC_TYPE_COLOR_YELLOW 13 \n\
 IPC_TYPE_COLOR_BLUE 14 \n\
 IPC_TYPE_FINISHED 20 \n\
 IPC_TYPE_ERROR 21 \n\
 IPC_TYPE_CANCEL 22"
/**
 * print_usage()
 * @brief Print out application usage
 */
static inline void print_usage()
{
	printf("%s\n", USAGE);
	printf("%s\n", TYPE_CODES);
}

/**
 * check_argc(int argc, char **argv)
 * @brief Quick argument check
 * @param argc
 * @param argv
 * @return 0 for success (no errors), or -1 for an error
 */ 
static inline int check_argc(int argc, char **argv)
{

	// Quick parameter check
	if ((argc < 4) || (argc > 4)) {
		print_usage();
		return (-1);
	} else {
		if (strlen(argv[2]) > 17) {
			print_usage();
			return (-1);
		} else if (strlen(argv[3]) > 17) {
			print_usage();
			return (-1);
		}
	}
	return (0);
}

/**
 * build_ipc_msg(ipc_msg_t * msg, uint8_t version, uint8_t type, char *top_row, char *bot_row)
 * @brief Build GPIO IPC message
 * @param msg
 * @param version
 * @param type
 * @param top_row
 * @param bot_row
 */
static inline void build_ipc_msg(ipc_msg_t * msg, uint8_t version, uint8_t type, char top_row[17], char bot_row[17])
{
	msg->msg_version = version;
	msg->msg_type = type;
	memcpy(&msg->top_row, top_row, sizeof(char[16]));
	memcpy(&msg->bot_row, bot_row, sizeof(char[16]));
}

/**
 * setup_sock_and_send(ipc_msg_t * msg)
 * @brief Setup a unix socket
 * @param msg
 * @return 0 for success, -1 for error
 */ 
static int setup_sock_and_send(ipc_msg_t * msg)
{

	int fd = 0;
	struct sockaddr_un remote_sockaddr;
	socklen_t len;
	int msg_size = sizeof(ipc_msg_t);

	// Setting up socket
	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return (-1);
	}

	remote_sockaddr.sun_family = AF_UNIX;
	strcpy(remote_sockaddr.sun_path, UX_SOCK_PATH);
	len = strlen(remote_sockaddr.sun_path) + sizeof(remote_sockaddr.sun_family);

	// Connecting to UX socket (as defined by UX_SOCK_PATH)
	if (connect(fd, (struct sockaddr *)&remote_sockaddr, len) == -1) {
		perror("error connecting to IPC sock");
		return (-1);
	}

	int res = 0;
	while (res < msg_size) {

		if ((res = send(fd, msg, msg_size, 0)) == -1) {
			perror("error sending IPC message");
			return (-1);
		}
	}

	close(fd);
	return (0);
}

/**
 * main(int argc, char **argv)
 * @brief Main function
 * @param argc
 * @param argv
 * @return 0 for success, -1 for error
 */ 
int main(int argc, char **argv)
{
	ipc_msg_t msg = { 0 };

	if (check_argc(argc, argv) < 0) {
		return (-1);
	}

	build_ipc_msg(&msg, (uint8_t) VERSION, (uint8_t) atoi(argv[1]), argv[2], argv[3]);

	return setup_sock_and_send(&msg);

}
