#ifndef YM_GDRIVE_H
#define YM_GDRIVE_H

#include <Arduino.h>
#include <esp_camera.h>

enum YM_GDRIVE_STATUS
{
	YM_GDRIVE_STATUS_NONE=0,
	YM_GDRIVE_STATUS_INIT_SUCCESS,
	YM_GDRIVE_STATUS_WRITE_SUCCESS,
};

enum YM_GDRIVE_ERR
{
	YM_GDRIVE_ERR_NONE=0,
	YM_GDRIVE_ERR_INIT,
    YM_GDRIVE_ERR_CONN,
	YM_GDRIVE_ERR_SEND,
};

enum YM_GDRIVE_TYPE
{
	YM_GDRIVE_TYPE_GMAIL=0,
	YM_GDRIVE_TYPE_YAHOO,
	YM_GDRIVE_TYPE_OUTLOOK,
	YM_GDRIVE_TYPE_HOTMAIL,
};

struct ym_gdrive_config
{
	String domain;
	String token;
	String url;
	uint16_t port;
	uint8_t dbg;
};

struct ym_gdrive_data
{
	uint8_t err;
	uint8_t init;
	uint8_t status;
	const char *tag;
};

struct ym_gdrive_params 
{
	struct ym_gdrive_config cfg;
	struct ym_gdrive_data data;
};

void ym_gdrive_init(struct ym_gdrive_params *param);
void ym_gdrive_send(struct ym_gdrive_params *param, camera_fb_t *fb);

#endif