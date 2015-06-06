/**
 * @file serial.c
 * @author Ron Brash (ron.brash@gmail.com)
 * @brief Handles serial port connection to devices
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

static int sport;

/**
 * get_serial_fd()
 * @param Returns socket to be used to connect to device
 * @return sock
 */
inline int get_serial_fd(void)
{
	return sport;
}

/**
 * set_serial_fd()
 * @param Sets socket to be used to connect to device
 * @param fd
 */
inline void set_serial_fd(int fd)
{
	sport = fd;
}

/**
 * setup_serial(unsigned char dev_name[])
 * @brief Setup serial terminal
 * @param dev_name
 * @return 0 for success, else -1 for error
 */ 
int setup_serial(unsigned char dev_name[])
{
	int fd;

	fd = open((const char *)dev_name, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1) {
		printf("open_port: Unable to open %s - ", dev_name);
		return (-1);
	} else {
		fcntl(fd, F_SETFL, 0);
	}
	set_serial_fd(fd);

	// Setup serial port and options
	struct termios options;
	tcgetattr(fd, &options);

	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);

	options.c_cflag |= (CLOCAL | CREAD);

	tcsetattr(fd, TCSANOW, &options);
	
	// 8N1 settings
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	options.c_oflag &= ~OPOST;
	options.c_lflag = 0;
	options.c_cc[VTIME] = 0;  // timeout after .1s that isn't working
	options.c_cc[VMIN] = 33;  // want to read a chunk of 64 bytes at a given time

	return (0);
}

/**
 * close_serial()
 * @brief closes serial port file descriptor
 * @return 0
 */
int close_serial()
{
	close(get_serial_fd());
	return (0);
}
