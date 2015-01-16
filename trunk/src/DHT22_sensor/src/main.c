/**
 * @file main.c
 * @author src from https://chrisbaume.wordpress.com/2013/02/10/beer-monitoring/
 * @author with some cleanup by Ronnie Brash (ron.brash@gmail.com)
 * @brief Return sensor data from DHT_22 or AM2302
 */ 
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_TIME 85
#define DHT11PIN 4		//This is a wiringPI pin, not physical or BCRM numbers
#define ATTEMPTS 5

int dht22_val[5] = { 0, 0, 0, 0, 0 };

/**
 * dht22_read_val()
 * @return 0 if error, 1 if success 
 */ 
int dht22_read_val()
{
	uint8_t lststate = HIGH;
	uint8_t counter = 0;
	uint8_t j = 0, i;
	float farenheit = 0;
	float t = 0;
	float h = 0;
	
	for (i = 0; i < 5; i++) {
		dht22_val[i] = 0;
	}
	
	pinMode(DHT11PIN, OUTPUT);
	digitalWrite(DHT11PIN, LOW);
	
	delay(18);
	
	digitalWrite(DHT11PIN, HIGH);
	
	delayMicroseconds(40);
	pinMode(DHT11PIN, INPUT);
	
	for (i = 0; i < MAX_TIME; i++) {
		counter = 0;
		while (digitalRead(DHT11PIN) == lststate) {
			counter++;
			delayMicroseconds(1);
			if (counter == 255) {
				break;
			}
		}
		lststate = digitalRead(DHT11PIN);
		if (counter == 255) {
			break;
		}
		// top 3 transistions are ignored
		if ((i >= 4) && (i % 2 == 0)) {
			dht22_val[j / 8] <<= 1;
			if (counter > 16){
				dht22_val[j / 8] |= 1;
			}
			j++;
		}
	}
	
	// Verify sensor output data
	if ((j >= 40) && (dht22_val[4] == ((dht22_val[0] + dht22_val[1] + dht22_val[2] + dht22_val[3]) & 0xFF))) {
		
		h = (float)dht22_val[0] * 256 + (float)dht22_val[1];
		h /= 10;
		t = (float)(dht22_val[2] & 0x7F) * 256 + (float)dht22_val[3];
		t /= 10.0;
		
		if ((dht22_val[2] & 0x80) != 0) {
			t *= -1;
		}
		
		farenheit = (1.8 * t) + 32;
		
		printf("Hum:%3.0f AmT:%3.0f\n", h, farenheit);
		
		return 1;
	} else {
		return 0;
	}
}

/**
 * main(int argc, char **argv)
 * @param argc
 * @param argv
 * @return 1 if error, 0 if success
 */ 
int main(int argc, char **argv)
{
	int attempts = ATTEMPTS;
	if (wiringPiSetup() == -1) {
		exit(1);
	}
	while (attempts) {
		int success = dht22_read_val();
		if (success) {
			break;
		}
		attempts--;
		delay(500);
	}
	return 0;
}
