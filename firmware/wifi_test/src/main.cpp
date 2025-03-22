#include <Arduino.h>
#include "ym_wifi.h"

struct ym_wifi_params ym_wifi_param;

void app_init(void);
void app_wifi_init(void);

void setup()
{
	delay(10000);
	Serial.begin(921600);
	Serial.println("Project wifi test");

	app_init();
}

void loop()
{
	delay(3000);
	Serial.println("loop");
}

void app_init(void)
{
	app_wifi_init();
}

void app_wifi_init(void)
{
	ym_wifi_param.data.tag = "WIFI";

	sprintf(ym_wifi_param.cfg.ssid, "JioFiber-ya24");
	ym_wifi_param.cfg.len_ssid = strlen(ym_wifi_param.cfg.ssid);

	sprintf(ym_wifi_param.cfg.password, "ammu1234");
	ym_wifi_param.cfg.len_password = strlen(ym_wifi_param.cfg.password);
	
	ym_wifi_init(&ym_wifi_param);
}