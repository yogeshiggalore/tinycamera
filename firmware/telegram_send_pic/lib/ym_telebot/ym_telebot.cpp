#include <Arduino.h>
#include "ym_telebot.h"

String BOTtoken = "6032172596:AAH1Wmi-BcnIm5kn5XVcIlkveGBVfp6BRjM";  // your Bot Token (Get from Botfather)

static struct ym_telebot_param *ptr_param;

void ym_telebot_init(struct ym_telebot_param *param)
{
    ptr_param = param;
    
}
void ym_telebot_set_bot_toke(String bot_token)
{
    ptr_param->bot_token = bot_token;
}