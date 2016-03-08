#ifndef LED_STRIP_LIB_H
#define LED_STRIP_LIB_H

#include <stdio.h>
#include <stdlib.h>

#define NB_LEDS 149   

void init_led_strip();
void set_led_strip_values(int red_player_period, int blue_player_period, int contact_location);
void cleanup_led_strip();


#endif
