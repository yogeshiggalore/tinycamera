#include <Arduino.h>
#include "esp_log.h"

#include "esp_camera.h"
#include "ym_ov2640.h"

static struct ym_ov2640_param *ptr_param;

void ym_ov2640_camera_init(struct ym_ov2640_param *param)
{

	ptr_param = param;
	
	param->data.status = YM_OV2640_STATUS_NONE;
	param->data.err = YM_OV2640_ERR_NONE;
	param->data.init = false;

	memset(&(param->cfg.cam), 0, sizeof(camera_config_t));

	param->cfg.cam.ledc_channel = LEDC_CHANNEL_0;
	param->cfg.cam.ledc_timer = LEDC_TIMER_0;
	param->cfg.cam.pin_d0 = Y2_GPIO_NUM;
	param->cfg.cam.pin_d1 = Y3_GPIO_NUM;
	param->cfg.cam.pin_d2 = Y4_GPIO_NUM;
	param->cfg.cam.pin_d3 = Y5_GPIO_NUM;
	param->cfg.cam.pin_d4 = Y6_GPIO_NUM;
	param->cfg.cam.pin_d5 = Y7_GPIO_NUM;
	param->cfg.cam.pin_d6 = Y8_GPIO_NUM;
	param->cfg.cam.pin_d7 = Y9_GPIO_NUM;
	param->cfg.cam.pin_xclk = XCLK_GPIO_NUM;
	param->cfg.cam.pin_pclk = PCLK_GPIO_NUM;
	param->cfg.cam.pin_vsync = VSYNC_GPIO_NUM;
	param->cfg.cam.pin_href = HREF_GPIO_NUM;
	param->cfg.cam.pin_sscb_sda = SIOD_GPIO_NUM;
	param->cfg.cam.pin_sscb_scl = SIOC_GPIO_NUM;
	param->cfg.cam.pin_pwdn = PWDN_GPIO_NUM;
	param->cfg.cam.pin_reset = RESET_GPIO_NUM;
	param->cfg.cam.xclk_freq_hz = 20000000;
	param->cfg.cam.frame_size = FRAMESIZE_XGA;
	//param->cfg.cam.frame_size = FRAMESIZE_240X240;
	param->cfg.cam.pixel_format = PIXFORMAT_JPEG; // for streaming
	param->cfg.cam.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
	param->cfg.cam.fb_location = CAMERA_FB_IN_PSRAM;
	param->cfg.cam.jpeg_quality = 12;
	param->cfg.cam.fb_count = 1;

	// Check if PSRAM is available
    ESP_LOGI(param->data.tag, "PSRAM Found: %s", psramFound() ? "Yes" : "No");

	// if PSRAM IC present, init with UXGA resolution and higher JPEG quality for larger pre-allocated frame buffer.
	if (param->cfg.cam.pixel_format == PIXFORMAT_JPEG)
	{
		if (psramFound())
		{
			param->cfg.cam.jpeg_quality = 10;
			param->cfg.cam.fb_count = 2;
			param->cfg.cam.grab_mode = CAMERA_GRAB_LATEST;
		}
		else
		{
			// Limit the frame size when PSRAM is not available
			param->cfg.cam.frame_size = FRAMESIZE_SVGA;
			param->cfg.cam.fb_location = CAMERA_FB_IN_DRAM;
		}
	}
	else
	{
		// Best option for face detection/recognition
		param->cfg.cam.frame_size = FRAMESIZE_240X240;
		#if CONFIG_IDF_TARGET_ESP32S3
			param->cfg.cam.fb_count = 2;
		#endif
	}

	// camera init
	esp_err_t err = esp_camera_init(&(param->cfg.cam));
	if (err != ESP_OK)
	{
		param->data.err = YM_OV2640_ERR_INIT;
		ESP_LOGE(param->data.tag, "Camera Init Failed");
	}
	else
	{
		param->data.status = YM_OV2640_STATUS_INIT_SUCCESS;
		param->data.init = true;
		param->data.img_timeout = IMG_TAKE_TIMEOUT;
		ESP_LOGE(param->data.tag, "Camera Init success");
	}
}

void ym_ov2640_get_cam_buffer(struct ym_ov2640_param *param)
{
	if (param->data.init == true)
	{
		// Take a photo
		param->data.fb = esp_camera_fb_get();
		if (!param->data.fb)
		{
			param->data.err = YM_OV2640_ERR_FRAME_BUF;
			param->data.fb = NULL;
			ESP_LOGE(param->data.tag, "Camera Frame Buffer Get Failed");
		}
	}
	else
	{
		param->data.status = YM_OV2640_ERR_INIT;
		param->data.fb = NULL;
		ESP_LOGE(param->data.tag, "Camera Init Failed");
	}
}

void ym_ov2640_release_cam_buffer(struct ym_ov2640_param *param)
{
	if( param->data.fb != NULL )
	{
		esp_camera_fb_return(param->data.fb);
	}
	param->data.fb = NULL;
}