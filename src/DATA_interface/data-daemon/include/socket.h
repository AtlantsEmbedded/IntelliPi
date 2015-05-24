/**
 * @file socket.h
 * @author Ron Brash (ron.brash@gmail.com)
 * @brief Contains socket header stuff 
 */ 
extern int sock;

int get_socket();
int setup_socket(char addr_mac[]);
void close_sockets();
