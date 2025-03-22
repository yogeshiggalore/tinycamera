#include <Arduino.h>
#include <ESP32FtpServer.h>
#include "ym_sd.h"
#include "ym_ov2640.h"
#include "ym_wifi.h"

struct ym_sd_params ym_sd_param;
struct ym_ov2640_param ym_cam_param;
struct ym_wifi_params ym_wifi_param;

FtpServer ftpSrv;

void app_init(void);
void sd_card_init(void);
void camera_init(void);
void wifi_init(void);
void ftp_init(void);

void take_photo_and_save_to_sd(char *filename);

void setup()
{
    delay(10000);
    Serial.begin(921600);

    Serial.println("project camera test");

    app_init();

    take_photo_and_save_to_sd("/mydir/test5.jpg");
    delay(1000);
    take_photo_and_save_to_sd("/mydir/test6.jpg");
    delay(1000);
    take_photo_and_save_to_sd("/mydir/test7.jpg");
    delay(1000);
    take_photo_and_save_to_sd("/mydir/test8.jpg");
    delay(1000);
}

void loop()
{
    ftpSrv.handleFTP();
}

void app_init(void)
{
    sd_card_init();
    camera_init();
    wifi_init();
    ftp_init();   
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

void ftp_init(void)
{
    /*  ftp init */
    ftpSrv.begin("esp32", "esp32");
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