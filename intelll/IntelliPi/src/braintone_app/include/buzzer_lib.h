#ifndef BUZZER_LIB_H
#define BUZZER_LIB_H

int setup_buzzer_lib(void);
int set_buzzer_state(int state);
int set_beep_mode(unsigned char state_a, unsigned char state_b, int half_period);
void turn_off_beeper();

#endif
