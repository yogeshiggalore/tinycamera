#include <Arduino.h>
#include "ym_sd.h"

struct ym_sd_params ym_sd_param;

void setup()
{
	delay(10000);

	Serial.begin(921600);
	Serial.println("Project: sdcard_test");

	ym_sd_param.cfg.cs_pin = 21;
	ym_sd_param.data.tag = "SD";
	esp_log_level_set(ym_sd_param.data.tag, ESP_LOG_VERBOSE);

	ym_sd_init(&ym_sd_param);
	
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

	/* list directory */
	ym_sd_list_dir("/", 2);

	/* delete file */
	ym_sd_delete_file("/mydir/test.txt");

	/* list directory */
	ym_sd_remove_dir("/mydir");

}

void loop()
{
	Serial.println("loop");
	delay(1000);
}
