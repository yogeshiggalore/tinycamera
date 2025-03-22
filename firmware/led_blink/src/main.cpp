#include <Arduino.h>

#define USER_LED 21

void setup()
{
	pinMode(USER_LED, OUTPUT);
}

void loop()
{
	digitalWrite(USER_LED, HIGH);
	delay(100);
	digitalWrite(USER_LED, LOW);
	delay(100);
}
