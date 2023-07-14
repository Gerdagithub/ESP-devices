#ifndef TUYA_FOR_ESP_H
#define TUYA_FOR_ESP_H

#include <signal.h>
#include <stdio.h>
#include <cJSON.h>
#include <tuya_log.h>
#include <tuya_error_code.h>
#include <system_interface.h>
#include <mqtt_client_interface.h>
#include <tuyalink_core.h>

#include <argp_for_esp.h>
#include <syslog.h>
#include <additional.h>
#include <ubus.h>

struct Message{
    char msg_in_json[100];
    char action[4];
    char port[30];
    int pin;
};

struct TuyaReport{
    tuya_mqtt_context_t *client;
    struct ubus_context *ctx;
    uint32_t id;
    char deviceId[50];
    char message[200];
};

void sig_handler(int signal);
void tuya_init(struct Arguments tuyaArguments, tuya_mqtt_context_t *client, int *ret);
void on_messages(tuya_mqtt_context_t *context, void *user_data, const tuyalink_message_t *msg);
void tuya_loop();

#endif