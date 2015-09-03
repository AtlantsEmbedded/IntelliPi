#ifndef MINDBX_LIB_H
#define MINDBX_LIB_H
/**
 * @file mindbx_lib.h
 * @author Frederic Simard (frederic.simard.1@outlook.com) | atlants embedded
 * @brief Libraries of functions to interact with the mindbx hardware.
 *        Functions allow for the monitoring of the coin acceptor and test button, 
 *        and the control over LED strip and the solenoid that push the door open.
 */

/*call to setup the system*/
void setup_mindbx(void);

/*blocking call until a coin is inserted*/
void wait_for_coin_insertion(void);

/*blocking call until test button pressed*/
void wait_for_test_button(void);

/*sets led strip intensity*/
void set_led_strip_intensity(int intensity);

/*opens mindbx door*/
void open_door(void);

#endif
