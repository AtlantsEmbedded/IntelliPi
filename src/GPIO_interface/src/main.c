#include <wiringPi.h>
#include <lcd.h>
#include "main.h"

static void waitForEnter(void)
{
	printf("Press SELECT to continue: ");
	fflush(stdout);

	while (digitalRead(AF_SELECT) == HIGH)	// Wait for push
		delay(1);
		printf("pressed button\n");

	while (digitalRead(AF_SELECT) == LOW){	// Wait for release
		delay(1);
		printf("released button\n");
	}

	printf("OK\n");
}

int main(int argc, char **argv)
{

	mcp23017Setup(AF_BASE, 0x20);
	lcd = lcdInit(2, 16, 4, AF_RS, AF_E, AF_DB4, AF_DB5, AF_DB6, AF_DB7, 0, 0, 0, 0);
	
	while(1) {
		waitForEnter();
	}

	return 0;

}
