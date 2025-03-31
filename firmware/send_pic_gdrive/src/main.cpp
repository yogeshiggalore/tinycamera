#include <Arduino.h>
#include "ym_sd.h"
#include "ym_ov2640.h"
#include "ym_wifi.h"
#include "ym_gdrive.h"

struct ym_sd_params ym_sd_param;
struct ym_ov2640_param ym_cam_param;
struct ym_wifi_params ym_wifi_param;
struct ym_gdrive_params ym_gdrive_param;

void app_init(void);
void sd_card_init(void);
void camera_init(void);
void wifi_init(void);
void smtp_init(void);
void gdrive_init(void);

void take_photo_and_save_to_sd(char *filename);
void ym_gdrive_send(struct ym_smtp_params *param, char *filename);

void setup()
{
    delay(10000);
    Serial.begin(921600);

    Serial.println("project camera test");

    app_init();

    Serial.println("waiting for 10 seconds");
    delay(1000);
    ym_ov2640_get_cam_buffer(&ym_cam_param);
	ym_gdrive_send(&ym_gdrive_param, ym_cam_param.data.fb);
    ym_ov2640_release_cam_buffer(&ym_cam_param);
}

void loop()
{
	delay(1000);
}

void app_init(void)
{
    sd_card_init();
    camera_init();
    wifi_init();
	gdrive_init();
}

void sd_card_init(void)
{
    /* ym_sd init */
    ym_sd_param.cfg.cs_pin = 21;
	ym_sd_param.data.tag = "SD";
	esp_log_level_set(ym_sd_param.data.tag, ESP_LOG_VERBOSE);
	ym_sd_init(&ym_sd_param);
}

void camera_init(void)
{
    /* ym_ov2640 init */
    ym_cam_param.data.tag = "CAM";
	esp_log_level_set(ym_cam_param.data.tag, ESP_LOG_VERBOSE);
	ym_ov2640_camera_init(&ym_cam_param);
}

void wifi_init(void)
{
    /* wifi init */
    ym_wifi_param.data.tag = "WIFI";
	sprintf(ym_wifi_param.cfg.ssid, "JioFiber-ya24");
	ym_wifi_param.cfg.len_ssid = strlen(ym_wifi_param.cfg.ssid);
	sprintf(ym_wifi_param.cfg.password, "ammu1234");
	ym_wifi_param.cfg.len_password = strlen(ym_wifi_param.cfg.password);
	ym_wifi_init(&ym_wifi_param);

    delay(1000);
}

void gdrive_init(void)
{
	ym_gdrive_param.data.tag = "GDRIVE";
	ym_gdrive_param.cfg.domain =  "script.google.com";
	ym_gdrive_param.cfg.port = 443;
	ym_gdrive_param.cfg.url = "https://script.google.com/macros/s/AKfycbzPfp6xxoOinFK-q45QGd3OgjdSW7SDxu6CsNX7AArCvroTu0qUqtV1v-oLnTrQU4Ch/exec?folder=TINYCAM";
	ym_gdrive_init(&ym_gdrive_param);

    ESP_LOGI(ym_gdrive_param.data.tag, "server:%s", ym_gdrive_param.cfg.domain);
    ESP_LOGI(ym_gdrive_param.data.tag, "port:%d", ym_gdrive_param.cfg.port);
    Serial.println(ym_gdrive_param.cfg.port);
}


void take_photo_and_save_to_sd(char *filename)
{
    ym_ov2640_get_cam_buffer(&ym_cam_param);
    if( ym_cam_param.data.fb != NULL )
    {
        ym_sd_write_image_to_file(filename, (const uint8_t *)ym_cam_param.data.fb->buf, ym_cam_param.data.fb->len);
        ym_ov2640_release_cam_buffer(&ym_cam_param);
        ESP_LOGE(ym_cam_param.data.tag, "camera image saved to %s", filename);
    }
    else
    {
        ESP_LOGE(ym_cam_param.data.tag, "camera buffer is NULL");
    }
}