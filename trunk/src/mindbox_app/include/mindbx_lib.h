#ifndef MINDBX_LIB_H
#define MINDBX_LIB_H
/**
 * @file mindbx_lib.h
 * @author Frederic Simard (frederic.simard.1@outlook.com) | atlants embedded
 * @brief Libraries of functions to interact with the mindbx hardware.
 *        Functions allow for the monitoring of the coin acceptor and test button, 
 *        and the control over LED strip and the solenoid that push the door open.
 */
 
/*quick LED strip color definition*/
#define OFF 0x00
#define RED 0x01
#define GREEN 0x02
#define BLUE 0x04
#define YELLOW RED|GREEN
#define CYAN GREEN|BLUE
#define PINK RED|BLUE
#define WHITE RED|GREEN|BLUE

/*call to setup the system*/
void setup_mindbx(void);

/*blocking call until a coin is inserted*/
void wait_for_coin_insertion(void);

/*blocking call until test button pressed*/
void wait_for_test_button(void);

/*sets led strip intensity*/
void set_led_strip_color(char rgb);

/*opens mindbx door*/
void open_door(void);

/*sets the LED strip to flash between rgb_on and rgb_off, changing every half period (ms)*/
void set_led_strip_flash_state(char rgb_on, char rgb_off, int half_period);

/*stops the flashing LED strip*/
void reset_led_strip_flash_state(void);

#endif
