#ifndef YM_SD_H
#define YM_SD_H

#include <Arduino.h>
#include <FS.h>
#include <SD.h>

enum ym_sd_status
{
	YM_SD_STATUS_NONE=0,
	YM_SD_STATUS_INIT_SUCCESS,
	YM_SD_STATUS_WRITE_SUCCESS,
};

enum ym_sd_err
{
	YM_SD_ERR_NONE=0,
	YM_SD_ERR_INIT,
	YM_SD_ERR_MOUNT,
	YM_SD_ERR_TYPE,
	YM_SD_ERR_NOT_DIR,
	YM_SD_ERR_MK_DIR,
	YM_SD_ERR_RM_DIR,
	YM_SD_ERR_FILE_EXISTS,
	YM_SD_ERR_DIR_OPEN,
	YM_SD_ERR_FILE_OPEN,
	YM_SD_ERR_FILE_WRITE,
	YM_SD_ERR_FILE_APPEND,
	YM_SD_ERR_FILE_DELETE,
};

struct ym_sd_config
{
	uint8_t cs_pin;
	uint8_t cardType;
};

struct ym_sd_data
{
	uint8_t err;
	uint8_t init;
	uint8_t status;
	const char *tag;
};

struct ym_sd_params 
{
	struct ym_sd_config cfg;
	struct ym_sd_data data;
};

void ym_sd_init(struct ym_sd_params *param);
int ym_sd_list_dir(const char *dirname, uint8_t levels);
int ym_sd_create_dir(const char *path);
int ym_sd_remove_dir(const char *path);
int ym_sd_read_file_and_print(const char *path);
int ym_sd_read_file(const char *path, File *file);
int ym_sd_write_to_file(const char *path, const char *message);
int ym_sd_append_data_to_file(const char *path, const char *message);
int ym_sd_delete_file(const char *path);
int ym_sd_file_exists(const char *path, bool *exists);
int ym_sd_write_image_to_file(const char *path, const uint8_t *data, size_t size);
#endif