#ifndef YM_APP_H
#define YM_APP_H

#include <Arduino.h>
#include "../lib/ym_ov2640/ym_ov2640.h"
#include "../lib/ym_wifi/ym_wifi.h"

struct ym_app_param
{
    struct ym_ov2640_param ym_cam;
    struct ym_wifi_param ym_wifi;
};

void ym_app_init(struct ym_app_param *param);
void ym_app_init_camera(void);
void ym_app_init_wifi(void);

#endif