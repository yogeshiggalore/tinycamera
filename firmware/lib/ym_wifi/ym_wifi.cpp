#include <Arduino.h>
#include "WiFi.h"
#include "ym_wifi.h"

static struct ym_wifi_params *ptr_ym_wifi_param;

void ym_wifi_event(WiFiEvent_t event)
{
	Serial.printf("[WiFi-event] event: %d\n", event);

	switch (event)
	{
	case ARDUINO_EVENT_WIFI_READY:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "WiFi interface ready");
		break;
	case ARDUINO_EVENT_WIFI_SCAN_DONE:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "Completed scan for access points");
		break;
	case ARDUINO_EVENT_WIFI_STA_START:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "WiFi client started");
		break;
	case ARDUINO_EVENT_WIFI_STA_STOP:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "WiFi clients stopped");
		break;
	case ARDUINO_EVENT_WIFI_STA_CONNECTED:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "Connected to access point");
		break;
	case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "Disconnected from WiFi access point");
		WiFi.disconnect();
		WiFi.reconnect();
		//timeClient.end();
		break;
	case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "Authentication mode of access point has changed");
		break;
	case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        ptr_ym_wifi_param->data.local_ip = WiFi.localIP();
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "got IP address");
		//timeClient.begin();
		//app_api_epochtime_check(ptr_app_params);
		// mqttClient.connect();
		break;
	case ARDUINO_EVENT_WIFI_STA_LOST_IP:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "Lost IP address and IP address is reset to 0");
		break;
	case ARDUINO_EVENT_WPS_ER_SUCCESS:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "WiFi Protected Setup (WPS): succeeded in enrollee mode");
		break;
	case ARDUINO_EVENT_WPS_ER_FAILED:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "WiFi Protected Setup (WPS): failed in enrollee mode");
		break;
	case ARDUINO_EVENT_WPS_ER_TIMEOUT:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "WiFi Protected Setup (WPS): timeout in enrollee mode");
		break;
	case ARDUINO_EVENT_WPS_ER_PIN:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "WiFi Protected Setup (WPS): pin code in enrollee mode");
		break;
	case ARDUINO_EVENT_WIFI_AP_START:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "WiFi access point started");
		break;
	case ARDUINO_EVENT_WIFI_AP_STOP:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "WiFi access point  stopped");
		break;
	case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "Client connected");
		break;
	case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "Client disconnected");
		break;
	case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "Assigned IP address to client");
		break;
	case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "Received probe request");
		break;
	case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "AP IPv6 is preferred");
		break;
	case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "STA IPv6 is preferred");
		break;
	case ARDUINO_EVENT_ETH_GOT_IP6:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "Ethernet IPv6 is preferred");
		break;
	case ARDUINO_EVENT_ETH_START:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "Ethernet started");
		break;
	case ARDUINO_EVENT_ETH_STOP:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "Ethernet stopped");
		break;
	case ARDUINO_EVENT_ETH_CONNECTED:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "Ethernet connected");
		break;
	case ARDUINO_EVENT_ETH_DISCONNECTED:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "Ethernet disconnected");
		break;
	case ARDUINO_EVENT_ETH_GOT_IP:
		ESP_LOGD(ptr_ym_wifi_param->data.tag, "Obtained IP address");
		break;
	default:
		break;
	}
}

void ym_wifi_init(struct ym_wifi_params *param)
{

    ptr_ym_wifi_param = param;

	param->data.status = YM_WIFI_STATUS_NONE;
	param->data.check_cnt = 10;
	param->data.err = YM_WIFI_ERR_NONE;
	param->data.init = false;

	// // sprintf(param->cfg.ssid, "IOTDev");
	// // param->cfg.len_ssid = strlen(param->cfg.ssid);

	// // sprintf(param->cfg.password, "ammu1234");
	// // param->cfg.len_password = strlen(param->cfg.password);

	// sprintf(param->cfg.ssid, "JioFiber-ya24");
	// param->cfg.len_ssid = strlen(param->cfg.ssid);

	// sprintf(param->cfg.password, "ammu1234");
	// param->cfg.len_password = strlen(param->cfg.password);

	memset(param->data.mac, 0, sizeof(param->data.mac));
  
    ESP_LOGD(param->data.tag, "ssid: %s\n", param->cfg.ssid);
    ESP_LOGD(param->data.tag, "pass: %s\n", param->cfg.password);

	WiFi.mode(WIFI_STA);
	ym_wifi_start(param);
}

void ym_wifi_start(struct ym_wifi_params *param)
{
	esp_err_t ret;

	WiFi.onEvent(ym_wifi_event);
	WiFi.begin(param->cfg.ssid, param->cfg.password);
}