/**
 * @file socket.c
 * @author Ronnie Brash (ron.brash@gmail.com)
 * @brief This source file contains all of the documentation for 
 * an epoll IPC mechanism that will handle external input which
 * may come from an external application called gpio_ipc_client
 * @note For IPC codes look in include/event.h
 * @copy Menu system for Atom Prototype
 * @todo Synchronization between user_input.c and
 * event.c functions; there could be a battle between
 * who accesses the LCD
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ezxml.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <glib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <sys/epoll.h>

#include <include/main.h>
#include <include/socket.h>
#include <include/event.h>

static void *epoll_sock_for_ipc(void *ptr);
static int set_fd_to_nonblocking(int fd);
static void list_on_epoll_fd(int epoll_event_fd);

/**
 * initialize_ipc_sock()
 * @brief initialize the IPC socket and thread
 * @return 0 for success (if thread fails it exits...)
 */ 
int initialize_ipc_sock()
{

	pthread_t ipc_thread;
	int rc = 0;
	rc = pthread_create(&ipc_thread, NULL, epoll_sock_for_ipc, NULL);

	if (rc) {
		printf("ERROR; return code from pthread_create() is %d\n", rc);
		return (-1);
	}
	rc = pthread_detach(ipc_thread);
	if (rc) {
		printf("ERROR; return code from pthread_detach() is %d\n", rc);
		return (-1);
	}

	return (0);
}

/**
 * list_on_epoll_fd(int epoll_event_fd)
 * @brief Retrieve data on epoll event and execute IPC event
 * @param fd
 */ 
static void list_on_epoll_fd(int epoll_event_fd)
{
	ipc_msg_t msg;
	int recv_bytes = 0;
	
    if ((recv_bytes = recv(epoll_event_fd,  &msg,  sizeof(ipc_msg_t),  0)) == -1) {
        perror("Error recv incoming message");
	}
	
	if(recv_bytes == sizeof(ipc_msg_t)) {
		execute_ipc_event(&msg);
	} else {
		perror("Size of msg does not meet min");
	}
	
    close(epoll_event_fd);
}

/**
 * set_fd_to_nonblocking(int fd)
 * @brief set socket to nonblocking
 * @param fd
 * @return results from fcntl or ioctl
 */ 
static int set_fd_to_nonblocking(int fd)
{
	int flags;

	/* If they have O_NONBLOCK, use the Posix way to do it */
#if defined(O_NONBLOCK)
	/* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
	if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
		flags = 0;
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
	/* Otherwise, use the old way of doing it */
	flags = 1;
	return ioctl(fd, FIOBIO, &flags);
#endif
}

/**
 * epoll_sock_for_ipc(void *ptr)
 * @brief function that sets up epolling and any file descriptors
 * @param ptr (if needed)
 */ 
static void *epoll_sock_for_ipc(void *ptr)
{
	int *data __attribute__ ((unused)) = (int *)ptr;
	
	struct epoll_event ev, events[MAX_EVENTS];
	int listen_sock, conn_sock, epollfd;
	struct sockaddr_un local;
	socklen_t len;

	/// Setup socket
	if ((listen_sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	/// Some fs/socket initialization & cleanup
	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, UX_SOCK_PATH);
	unlink(local.sun_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);
	
	/// Socket usuals, bind, listen
	if (bind(listen_sock, (struct sockaddr *)&local, len) == -1) {
		perror("bind");
		exit(1);
	}

	if (listen(listen_sock, 5) == -1) {
		perror("listen");
		exit(1);
	}

	/// Initialize epoll and related socket
	epollfd = epoll_create(MAX_EVENTS);
	if (epollfd == -1) {
		perror("epoll_create");
		exit(1);
	}

	ev.events = EPOLLIN;
	ev.data.fd = listen_sock;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
		perror("epoll_ctl: listen_sock");
		exit(1);
	}

	/// EPOLL running loop
	for (;;) {
		int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
		if (nfds == -1) {
			// Don't care if there is an interupt :)
			continue;
		}

		int n = 0;
		for (n = 0; n < nfds; ++n) {
			if (events[n].data.fd == listen_sock) {
				conn_sock = accept(listen_sock, (struct sockaddr *)&local, &len);
				if (conn_sock == -1) {
					perror("accept");
					continue;
				}
				set_fd_to_nonblocking(conn_sock);
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = conn_sock;
				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
					perror("epoll_ctl: conn_sock");
					continue;
				}
			} else {
				list_on_epoll_fd(events[n].data.fd);
			}
		}
	}

}
