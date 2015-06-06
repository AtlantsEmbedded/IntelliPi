/**
 * @file socket.h
 * @author Ron Brash (ron.brash@gmail.com)
 * @brief Contains socket header stuff 
 */ 

int get_socket_fd();
void set_socket_fd();
int setup_socket(unsigned char addr_mac[]);
void close_sockets();
