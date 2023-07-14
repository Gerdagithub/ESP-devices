#ifndef ADDITIONAL_FOR_ESP_TUYA_H
#define ADDITIONAL_FOR_ESP_TUYA_H

#include <tuya.h>
#include <syslog.h>
#include <libserialport.h>

void sig_handler(int signal);              
void convert_message_from_json(struct Message *mess);
char* convert_devices_json_array(char jsonMessage[200]);

#endif