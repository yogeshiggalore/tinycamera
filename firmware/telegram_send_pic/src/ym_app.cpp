#include <Arduino.h>
#include "ym_app.h"
#include "../lib/ym_ov2640/ym_ov2640.h"
#include "../lib/ym_wifi/ym_wifi.h"

static struct ym_app_param *ptr_app_param;

const char *cam_tag = "CAM";
const char *wifi_tag = "WIFI";

void ym_app_init(struct ym_app_param *param)
{
	ptr_app_param = param;
	ym_app_init_camera();
	ym_app_init_wifi();
}

void ym_app_init_camera(void)
{
	ptr_app_param->ym_cam.data.tag = cam_tag;
	esp_log_level_set(ptr_app_param->ym_cam.data.tag, ESP_LOG_VERBOSE);
	ym_ov2640_camera_init(&(ptr_app_param->ym_cam));
}

void ym_app_init_wifi(void)
{
	ptr_app_param->ym_cam.data.tag = cam_tag;
	esp_log_level_set(ptr_app_param->ym_cam.data.tag, ESP_LOG_VERBOSE);
	
	ptr_app_param->ym_wifi.data.status = YM_WIFI_STATUS_NONE;
	ptr_app_param->ym_wifi.data.check_cnt = 10;
	ptr_app_param->ym_wifi.data.err = YM_WIFI_ERR_NONE;
	ptr_app_param->ym_wifi.data.init = false;
	ptr_app_param->ym_wifi.data.tag = wifi_tag;

	// sprintf(ptr_app_param->ym_wifi.cfg.ssid, "IOTDev");
	// ptr_app_param->ym_wifi.cfg.len_ssid = strlen(ptr_app_param->ym_wifi.cfg.ssid);

	// sprintf(ptr_app_param->ym_wifi.cfg.password, "ammu1234");
	// ptr_app_param->ym_wifi.cfg.len_password = strlen(ptr_app_param->ym_wifi.cfg.password);

	sprintf(ptr_app_param->ym_wifi.cfg.ssid, "JioFiber-ya24");
	ptr_app_param->ym_wifi.cfg.len_ssid = strlen(ptr_app_param->ym_wifi.cfg.ssid);

	sprintf(ptr_app_param->ym_wifi.cfg.password, "ammu1234");
	ptr_app_param->ym_wifi.cfg.len_password = strlen(ptr_app_param->ym_wifi.cfg.password);

	memset(ptr_app_param->ym_wifi.data.mac, 0, sizeof(ptr_app_param->ym_wifi.data.mac));

	Serial.printf("app_api_init_wifi\n");
	Serial.printf("ssid: %s\n", ptr_app_param->ym_wifi.cfg.ssid);
	Serial.printf("pass: %s\n", ptr_app_param->ym_wifi.cfg.password);

	WiFi.mode(WIFI_STA);
	ym_wifi_init(&(ptr_app_param->ym_wifi));
}