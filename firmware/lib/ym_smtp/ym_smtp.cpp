#include <Arduino.h>
#include "ym_smtp.h"
#include "ESP_Mail_Client.h"

static struct ym_smtp_params *ptr_param;
SMTPSession smtp;

void ym_smtp_init(struct ym_smtp_params *param)
{
	ptr_param = param;
	param->data.init = true;
	MailClient.networkReconnect(true);
	ESP_LOGI(param->data.tag, "smtp init success");
}

void ym_smtp_send(struct ym_smtp_params *param, char *filename, char *subject, char *email)
{

	smtp.debug(param->cfg.dbg);

	smtp.callback(smtp_cb);

	Session_Config config;

	config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
	config.time.gmt_offset = 0;
	config.time.day_light_offset = 1;

	/* Set the session config */
	config.server.host_name = param->cfg.server;
	config.server.port = param->cfg.port;
	config.login.email = param->cfg.email;
	config.login.password = param->cfg.password;
	config.login.user_domain = "";

	/* Declare the message class */
	SMTP_Message message;

	/* Enable the chunked data transfer with pipelining for large message if server supported */
	message.enable.chunking = true;

	/* Set the message headers */
	message.sender.name = "IOTDev";
	message.sender.email = param->cfg.email;

	message.subject = subject;
	message.addRecipient(email, email);

	String htmlMsg = "<h4>Photo captured with tinycam and attached in this email.</h4>";
	message.html.content = htmlMsg.c_str();
	message.html.charSet = "utf-8";
	message.html.transfer_encoding = Content_Transfer_Encoding::enc_qp;

	message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_normal;
	message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

	/* The attachment data item */
	SMTP_Attachment att;

	/** Set the attachment info e.g.
	 * file name, MIME type, file path, file storage type,
	 * transfer encoding and content encoding
	 */
	att.descr.filename = filename;
	att.descr.mime = "image/png";
	att.file.path = filename;
	att.file.storage_type = esp_mail_file_storage_type_sd;
	att.descr.transfer_encoding = Content_Transfer_Encoding::enc_base64;

	/* Add attachment to the message */
	message.addAttachment(att);

	/* Connect to server with the session config */
	if (!smtp.connect(&config))
		return;

	/* Start sending the Email and close the session */
	if (!MailClient.sendMail(&smtp, &message, true))
		Serial.println("Error sending Email, " + smtp.errorReason());
}

void smtp_cb(SMTP_Status status)
{
	/* Print the current status */
	Serial.println(status.info());

	/* Print the sending result */
	if (status.success())
	{
		Serial.println("----------------");
		Serial.printf("Message sent success: %d\n", status.completedCount());
		Serial.printf("Message sent failled: %d\n", status.failedCount());
		Serial.println("----------------\n");
		struct tm dt;

		for (size_t i = 0; i < smtp.sendingResult.size(); i++)
		{
			/* Get the result item */
			SMTP_Result result = smtp.sendingResult.getItem(i);
			time_t ts = (time_t)result.timestamp;
			localtime_r(&ts, &dt);

			ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
			ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
			ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
			ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients.c_str());
			ESP_MAIL_PRINTF("Subject: %s\n", result.subject.c_str());
		}
		Serial.println("----------------\n");

		// You need to clear sending result as the memory usage will grow up.
		smtp.sendingResult.clear();
	}
}