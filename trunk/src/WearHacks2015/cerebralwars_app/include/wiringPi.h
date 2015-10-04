#ifndef WIRING_PI_H
#define WIRING_PI_H

/*Stub for wiringPi*/

#define HIGH 1
#define LOW 0


#define INPUT 0
#define OUTPUT 0
#define PWM_OUTPUT 0

int wiringPiSetup(void);
void pwmWrite(int pin, int value);
void digitalWrite(int pin, int value);
void pinMode(int pin, int mode);
int digitalRead(int pin);
int delay(int time_delay);


#endif
