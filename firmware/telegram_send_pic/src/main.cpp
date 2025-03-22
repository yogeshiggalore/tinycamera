#include <Arduino.h>
#include "ym_app.h"

struct ym_app_param app_param;

void setup()
{
	delay(5000);
	
	Serial.begin(921600);
	Serial.println("Project: telegram_send_pic");

	ym_app_init(&app_param);
}

void loop()
{
	delay(1000);
	Serial.println("loop test");
}