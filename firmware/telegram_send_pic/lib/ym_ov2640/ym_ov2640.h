#ifndef YM_OV2640_H
#define YM_OV2640_H

#include <Arduino.h>
#include "esp_camera.h"

#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     10
#define SIOD_GPIO_NUM     40
#define SIOC_GPIO_NUM     39

#define Y9_GPIO_NUM       48
#define Y8_GPIO_NUM       11
#define Y7_GPIO_NUM       12
#define Y6_GPIO_NUM       14
#define Y5_GPIO_NUM       16
#define Y4_GPIO_NUM       18
#define Y3_GPIO_NUM       17
#define Y2_GPIO_NUM       15
#define VSYNC_GPIO_NUM    38
#define HREF_GPIO_NUM     47
#define PCLK_GPIO_NUM     13

#define LED_GPIO_NUM      21

#define IMG_TAKE_TIMEOUT	30

enum ym_ov2640_err
{
	YM_OV2640_ERR_NONE=0,
	YM_OV2640_ERR_INIT,
	YM_OV2640_ERR_FRAME_BUF,
	YM_OV2640_ERR_CAPTURE,
};

enum ym_ov2640_status
{
	YM_OV2640_STATUS_NONE=0,
	YM_OV2640_STATUS_INIT_SUCCESS,
	YM_OV2640_STATUS_PHOTO_SUCCESS,
};

struct ym_ov2640_config
{
	camera_config_t cam;
};

struct ym_ov2640_data
{
	uint8_t err;
	uint8_t status;
	uint8_t init;
	uint32_t img_cnt;
	uint16_t img_timeout;
	camera_fb_t *fb;
	const char *tag;
};

struct ym_ov2640_param
{
	struct ym_ov2640_config cfg;
	struct ym_ov2640_data data;
};

void ym_ov2640_camera_init(struct ym_ov2640_param *param);
void ym_ov2640_get_cam_buffer(struct ym_ov2640_param *param);
void ym_ov2640_release_cam_buffer(struct ym_ov2640_param *param);

#endif //YM_OV2640_H