#include <Arduino.h>
#include "ym_sd.h"

static struct ym_sd_params *ptr_param;

void ym_sd_init(struct ym_sd_params *param)
{
	ptr_param = param;

	param->data.init = false;

	// Initialize SD card
	if (!SD.begin(param->cfg.cs_pin))
	{
		param->data.err = YM_SD_ERR_TYPE;
		ESP_LOGE(param->data.tag, "sd card type error");
	}
	else
	{
		// Get card type
		param->cfg.cardType = SD.cardType();

		// Determine if the type of SD card is available
		if (param->cfg.cardType == CARD_NONE)
		{
			param->data.err = YM_SD_ERR_TYPE;
			ESP_LOGE(param->data.tag, "sd card type error");
		}
		else
		{
			param->data.status = YM_SD_STATUS_INIT_SUCCESS;
			param->data.init = true;
			ESP_LOGI(param->data.tag, "sd card init success");
		}
	}
}

int ym_sd_list_dir(const char *dirname, uint8_t levels)
{
	int err = YM_SD_ERR_NONE;

	if(ptr_param->data.init == true)
	{
		ESP_LOGI(ptr_param->data.tag, "Listing directory: %s", dirname);
		File root = SD.open(dirname);
		if (!root)
		{
			ESP_LOGI(ptr_param->data.tag, "Failed to open directory %s", dirname);
			err = YM_SD_ERR_DIR_OPEN;
		}

		if( err == YM_SD_ERR_NONE )
		{
			if (!root.isDirectory())
			{
				ESP_LOGI(ptr_param->data.tag, "Not a directory %s", dirname);
				err = YM_SD_ERR_NOT_DIR;
			}
		}
		
		if( err == YM_SD_ERR_NONE )
		{
			File file = root.openNextFile();
			while (file)
			{
				if (file.isDirectory())
				{
					ESP_LOGI(ptr_param->data.tag, "DIR %s", file.name());
					if (levels) ym_sd_list_dir(file.name(), levels - 1);
				}
				else
				{
					ESP_LOGI(ptr_param->data.tag, "FILE: %s\tSIZE: %d", file.name(), file.size());
				}
				file = root.openNextFile();
			}
		}
	}
	else
	{
		ESP_LOGE(ptr_param->data.tag, "sd card not initialized");	
		err = YM_SD_ERR_INIT;
	}
	

	return err;
}

int ym_sd_create_dir(const char *path)
{
	int err = YM_SD_ERR_NONE;

	if(ptr_param->data.init == true)
	{
		ESP_LOGI(ptr_param->data.tag, "Creating Dir: %s", path);
		if(SD.exists(path) == false)
		{
			if (SD.mkdir(path)) 
			{
				ESP_LOGI(ptr_param->data.tag, "Directory created successfully");
			}
			else
			{
				ESP_LOGI(ptr_param->data.tag, "mkdir failed");
				err = YM_SD_ERR_MK_DIR;
			}
		}
		else
		{
			ESP_LOGI(ptr_param->data.tag, "Directory already exists %s", path);
			err = YM_SD_ERR_MK_DIR;
		}
	}
	else
	{
		ESP_LOGE(ptr_param->data.tag, "sd card not initialized");	
		err = YM_SD_ERR_INIT;
	}

	return err;
}

int ym_sd_remove_dir(const char *path)
{
	int err = YM_SD_ERR_NONE;
	if(ptr_param->data.init == true)
	{
		ESP_LOGE(ptr_param->data.tag, "Removing Dir: %s", path);
		if(SD.exists(path) == true)
		{
			if (SD.rmdir(path))
			{
				ESP_LOGE(ptr_param->data.tag, "Directory removed successfully %s", path);
			}
			else
			{
				ESP_LOGE(ptr_param->data.tag, "rmdir failed %s", path);
				err = YM_SD_ERR_RM_DIR;
			}
		}
	}
	else
	{
		ESP_LOGE(ptr_param->data.tag, "sd card not initialized");	
		err = YM_SD_ERR_INIT;
	}
	
	return err;
}

int ym_sd_read_file_and_print(const char *path)
{
	int err = YM_SD_ERR_NONE;
	if(ptr_param->data.init == true)
	{
		if(SD.exists(path) == true)
		{
			ESP_LOGI(ptr_param->data.tag, "Reading file: %s", path);
			File file = SD.open(path, FILE_READ);
			if (file)
			{
				ESP_LOGI(ptr_param->data.tag, "File content: ");
				while (file.available())
				{
					Serial.write(file.read());
				}
				Serial.println();
				file.close();
			}
			else
			{
				ESP_LOGE(ptr_param->data.tag, "Failed to open file %s for reading", path);
				err = YM_SD_ERR_FILE_OPEN;
			}
		}
		else
		{
			err = YM_SD_ERR_FILE_EXISTS;
			ESP_LOGE(ptr_param->data.tag, "File %s does not exist", path);	
		}
	}
	else
	{
		ESP_LOGE(ptr_param->data.tag, "sd card not initialized");	
		err = YM_SD_ERR_INIT;
	}
	
	return err;
}

int ym_sd_read_file(const char *path, File *file)
{
	int err = YM_SD_ERR_NONE;
	if(ptr_param->data.init == true)
	{
		if(SD.exists(path) == true)
		{
			ESP_LOGI(ptr_param->data.tag, "Reading file: %s", path);
			*file = SD.open(path, FILE_READ);
			if (!(*file))
			{
				ESP_LOGE(ptr_param->data.tag, "Failed to open file %s for reading", path);
				err = YM_SD_ERR_FILE_OPEN;
			}
		}
		else
		{
			err = YM_SD_ERR_FILE_EXISTS;
			ESP_LOGE(ptr_param->data.tag, "File %s does not exist", path);	
		}
	}
	else
	{
		ESP_LOGE(ptr_param->data.tag, "sd card not initialized");	
		err = YM_SD_ERR_INIT;
	}
	
	return err;
}

int ym_sd_write_to_file(const char *path, const char *message)
{
	int err = YM_SD_ERR_NONE;
	if(ptr_param->data.init == true)
	{
		ESP_LOGE(ptr_param->data.tag, "Writing file: %s", path);
		File file = SD.open(path, FILE_WRITE);
		if (file)
		{
			if (!file.print(message))
			{
				ESP_LOGE(ptr_param->data.tag, "Write failed %s", path);
				err = YM_SD_ERR_FILE_WRITE;
			}
			file.close();
		}
		else
		{
			ESP_LOGE(ptr_param->data.tag, "Failed to open file %s for writing", path);
			err = YM_SD_ERR_FILE_WRITE;
		}
	}
	else
	{
		ESP_LOGE(ptr_param->data.tag, "sd card not initialized");	
		err = YM_SD_ERR_INIT;
	}
	
	return err;
}

int ym_sd_append_data_to_file(const char *path, const char *message)
{
	int err = YM_SD_ERR_NONE;
	if(ptr_param->data.init == true)
	{
		if(SD.exists(path) == true)
		{
			ESP_LOGE(ptr_param->data.tag, "appending to file: %s", path);
			File file = SD.open(path, FILE_APPEND);
			if (file)
			{
				if (!file.print(message))
				{
					ESP_LOGE(ptr_param->data.tag, "append failed %s", path);
					err = YM_SD_ERR_FILE_APPEND;
				}
				file.close();
			}
			else
			{
				ESP_LOGE(ptr_param->data.tag, "Failed to open file  %s for appending", path);
				err = YM_SD_ERR_FILE_APPEND;
			}
		}
		else
		{
			err = YM_SD_ERR_FILE_EXISTS;
			ESP_LOGE(ptr_param->data.tag, "File does not exist %s", path);	
		}
	}
	else
	{
		ESP_LOGE(ptr_param->data.tag, "sd card not initialized");	
		err = YM_SD_ERR_INIT;
	}
	
	return err;
}

int ym_sd_delete_file(const char *path)
{
	int err = YM_SD_ERR_NONE;
	if(ptr_param->data.init == true)
	{
		if(SD.exists(path) == true)
		{
			ESP_LOGE(ptr_param->data.tag, "Deleting file: %s", path);
			if(!SD.remove(path))
			{
				ESP_LOGE(ptr_param->data.tag, "delete file %s failed", path);
				err = YM_SD_ERR_FILE_DELETE;
			}
			else
			{
				ESP_LOGE(ptr_param->data.tag, "file: deleted %s", path);
			}
		}
		else
		{
			err = YM_SD_ERR_FILE_EXISTS;
			ESP_LOGE(ptr_param->data.tag, "File does not exist %s", path);	
		}
	}
	else
	{
		ESP_LOGE(ptr_param->data.tag, "sd card not initialized");	
		err = YM_SD_ERR_INIT;
	}
	
	return err;
}

int ym_sd_file_exists(const char *path, bool *exists)
{
	int err = YM_SD_ERR_NONE;
	if(ptr_param->data.init == true)
	{
		*exists = SD.exists(path);
	}
	else
	{
		ESP_LOGE(ptr_param->data.tag, "sd card not initialized");	
		err = YM_SD_ERR_INIT;
	}
	
	return err;
}

int ym_sd_write_image_to_file(const char *path, const uint8_t *data, size_t size)
{
	int err = YM_SD_ERR_NONE;
	if(ptr_param->data.init == true)
	{
		ESP_LOGE(ptr_param->data.tag, "Writing image to sd file: %s", path);
		File file = SD.open(path, FILE_WRITE);
		if (file)
		{
			if (!file.write(data, size))
			{
				ESP_LOGE(ptr_param->data.tag, "Write failed %s", path);
				err = YM_SD_ERR_FILE_WRITE;
			}
			file.close();
		}
		else
		{
			ESP_LOGE(ptr_param->data.tag, "Failed to open file %s for writing", path);
			err = YM_SD_ERR_FILE_WRITE;
		}
	}
	else
	{
		ESP_LOGE(ptr_param->data.tag, "sd card not initialized");	
		err = YM_SD_ERR_INIT;
	}
	
	return err;
}