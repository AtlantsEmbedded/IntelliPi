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
#include <errno.h>

#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include "xml.h"

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
	int fd, rc;

	fd = open((const char *)dev_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (fd == -1) {
		printf("open_port: Unable to open %s - ", dev_name);
		return (-1);
	} 
	set_serial_fd(fd);

	struct termios toptions;
    cfsetispeed(&toptions, B115200);
    cfsetospeed(&toptions, B115200);

    // 8N1 & no flow control
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    toptions.c_cflag &= ~CRTSCTS;

    toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    toptions.c_oflag &= ~OPOST; // make raw

    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 0;
    
    tcsetattr(fd, TCSANOW, &toptions);

    if((rc = tcsetattr(fd, TCSANOW, &toptions)) < 0){
        fprintf(stderr, "failed to set attr: %d, %s\n", fd, strerror(errno));
        exit(EXIT_FAILURE);
    }

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
