/**
 * @file serial.h
 * @author Ron Brash (ron.brash@gmail.com)
 * @brief Header for serial functions etc...
 */
inline int get_serial_fd(void);
inline void set_serial_fd(int fd);
int setup_serial(unsigned char dev_name[]);
int close_serial();
