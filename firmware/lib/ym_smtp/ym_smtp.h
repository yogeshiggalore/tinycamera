#ifndef YM_SMTP_H
#define YM_SMTP_H

#include <Arduino.h>
#include "ESP_Mail_Client.h"

enum ym_smtp_status
{
	YM_SMTP_STATUS_NONE=0,
	YM_SMTP_STATUS_INIT_SUCCESS,
	YM_SMTP_STATUS_WRITE_SUCCESS,
};

enum ym_smtp_err
{
	YM_SMTP_ERR_NONE=0,
	YM_SMTP_ERR_INIT,
    YM_SMTP_ERR_CONN,
	YM_SMTP_ERR_SEND,
};

enum ym_smtp_type
{
	YM_SMTP_TYPE_GMAIL=0,
	YM_SMTP_TYPE_YAHOO,
	YM_SMTP_TYPE_OUTLOOK,
	YM_SMTP_TYPE_HOTMAIL,
};

struct ym_smtp_config
{
	uint8_t type;
	char email[32];
	uint8_t len_email;
	char password[32];
	uint8_t len_password;
	char server[32];
	uint8_t len_server;
	uint16_t port;
	uint8_t dbg;
};

struct ym_smtp_data
{
	uint8_t err;
	uint8_t init;
	uint8_t status;
	const char *tag;
};

struct ym_smtp_params 
{
	struct ym_smtp_config cfg;
	struct ym_smtp_data data;
};

void smtp_cb(SMTP_Status status);

void ym_smtp_init(struct ym_smtp_params *param);
void ym_smtp_send(struct ym_smtp_params *param, char *filename, char *subject, char *email);

#endif