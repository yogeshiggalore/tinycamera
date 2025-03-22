#ifndef YM_TELEBOT_H
#define YM_TELEBOT_H
#include <Arduino.h>

struct ym_telebot_param
{
	uint8_t err;
	uint8_t status;
	uint8_t init;
    String bot_token;
};

void ym_telebot_init(struct ym_telebot_param *param);
void ym_telebot_set_bot_toke(String bot_token);

#endif