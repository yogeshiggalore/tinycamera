#include <Arduino.h>
#include "ym_gdrive.h"
#include <WiFiClientSecure.h>
#include "ym_base64.h"

static struct ym_gdrive_params *ptr_param;

void ym_gdrive_init(struct ym_gdrive_params *param)
{
	ptr_param = param;
	param->data.init = true;
	ESP_LOGI(param->data.tag, "google drive init success");
}

void ym_gdrive_send(struct ym_gdrive_params *param, camera_fb_t *fb)
{
	ESP_LOGI(param->data.tag, "Connect to %s", param->cfg.domain);
	WiFiClientSecure client;
	client.setInsecure(); // Comment out this line on Windows
	if (client.connect(param->cfg.domain.c_str(), param->cfg.port))
	{
		ESP_LOGI(param->data.tag, "Connection successful");
		ESP_LOGI(param->data.tag, "Sending image to Google Drive.");
		//ESP_LOGI(param->data.tag, "%d", fb->len);

		client.println("POST " + param->cfg.url + " HTTP/1.1");
		client.println("Host: " + param->cfg.domain);
		client.println("Transfer-Encoding: chunked");
		client.println();

		int fbLen = fb->len;
		char *input = (char *)fb->buf;
		int chunkSize = 3 * 1000; // must be multiple of 3
		int chunkBase64Size = ym_base64_enc_len(chunkSize);
		char output[chunkBase64Size + 1];

		int chunk = 0;
		for (int i = 0; i < fbLen; i += chunkSize)
		{
			int l = ym_base64_encode(output, input, min(fbLen - i, chunkSize));
			client.print(l, HEX);
			client.print("\r\n");
			client.print(output);
			client.print("\r\n");
			delay(100);
			input += chunkSize;
			chunk++;
		}
		client.print("0\r\n");
		client.print("\r\n");

		long int StartTime = millis();
		while (!client.available())
		{
			delay(100);
			if ((StartTime + (30 * 1000)) < millis())
			{
				ESP_LOGI(param->data.tag, "No response.");
				break;
			}
		}
		while (client.available())
		{
			Serial.print(char(client.read()));
		}
		Serial.print("image sent success\n");
	}
	else
	{
		ESP_LOGI(param->data.tag,"%s", "Connected to server failed.");
	}
	client.stop();
}
