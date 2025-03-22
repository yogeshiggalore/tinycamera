#include <Arduino.h>

#include <WiFi.h>
#include "ESP32FtpServer.h"
#include "ym_sd.h"
#include "ym_wifi.h"

void app_init(void);
void app_wifi_init(void);
void app_sd_init(void);
void app_ftp_init(void);
void app_sd_card_test_run(void);

FtpServer ftpSrv;

struct ym_sd_params ym_sd_param;
struct ym_wifi_params ym_wifi_param;

void setup()
{
	delay(10000);
	Serial.begin(92600);

	app_init();

	app_sd_card_test_run();
}

void loop()
{
	ftpSrv.handleFTP();
}

void app_init(void)
{
	app_wifi_init();
	app_sd_init();
	app_ftp_init();
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

void app_sd_init(void)
{
	ym_sd_param.cfg.cs_pin = 21;
	ym_sd_param.data.tag = "SD";
	esp_log_level_set(ym_sd_param.data.tag, ESP_LOG_VERBOSE);

	ym_sd_init(&ym_sd_param);
}

void app_ftp_init(void)
{
	if(  ym_sd_param.data.init == true)
	{
		ftpSrv.begin("esp32", "esp32");
	}
}

void app_sd_card_test_run(void)
{
	/* lsit directory */
	ym_sd_list_dir("/", 2);

	/* create directory */
	ym_sd_create_dir("/mydir");

	/* list directory */
	ym_sd_write_to_file("/mydir/test.txt", "Hello ESP32!");

	/* list directory */
	ym_sd_list_dir("/mydir", 2);

	/* read file */
	ym_sd_read_file_and_print("/mydir/test.txt");

	/* append data to file */
	ym_sd_append_data_to_file("/mydir/test.txt", "\nAppending new line...");

	/* read file */
	ym_sd_read_file_and_print("/mydir/test.txt");
	
	bool isfile_present = false;
	ym_sd_file_exists("/mydir/test.txt",&isfile_present);
	if(isfile_present)
	{
		Serial.println("File exists");
	}
	else
	{
		Serial.println("File does not exist");
	}
}