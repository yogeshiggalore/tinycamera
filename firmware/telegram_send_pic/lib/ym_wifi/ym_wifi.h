#ifndef YM_WIFI_H
#define YM_WIFI_H

#include <Arduino.h>
#include "WiFi.h"

enum ym_wifi_err
{
	YM_WIFI_ERR_NONE=0,
	YM_WIFI_ERR_INIT,
	YM_WIFI_ERR_MAC_READ,
	YM_WIFI_ERR_CONNECT,	
};

enum ym_wifi_status
{
	YM_WIFI_STATUS_NONE=0,
	YM_WIFI_STATUS_INIT_SUCCESS,
	YM_WIFI_STATUS_CONNECTED,
	YM_WIFI_STATUS_DISCONNECTED,
};

struct ym_wifi_config
{
	char ssid[32];
	uint8_t len_ssid;
	char password[32];
	uint8_t len_password;
};

struct ym_wifi_data
{
	const char *tag;
	WiFiEvent_t event;
	uint8_t mac[6];
	IPAddress local_ip;
	uint8_t status;
	uint8_t check_cnt;
	uint8_t err;
	uint8_t init;
};

struct ym_wifi_param
{
	struct ym_wifi_config cfg;
	struct ym_wifi_data data;
};

void ym_wifi_init(struct ym_wifi_param *param);
void ym_wifi_start(struct ym_wifi_param *param);

#endif