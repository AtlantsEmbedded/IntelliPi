/**
 * @file main.c
 * @author Ronnie Brash (ron.brash@gmail.com)
 * @copy Menu system for Atom Prototype
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ezxml.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>

#ifdef RASPI
#include <wiringPi.h>
#include <mcp23017.h>
#include <softTone.h>
#include <lcd.h>
#endif


int main(int argc, char **argv)
{

	/// Setup wiringPi Library
	wiringPiSetup();

	pinMode(0, OUTPUT);
	pinMode(1, OUTPUT);
	pinMode(2, INPUT);
	pinMode(3, INPUT);

	for (;;)
	{
		digitalWrite(0, HIGH); 
		delay(500);
		digitalWrite(0, LOW); 
		delay (500) ;
	}
	
	return 0;
}
