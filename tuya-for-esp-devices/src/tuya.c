#include <tuya_cacert.h>
#include <tuya.h>

struct Message message;
struct TuyaReport tuyaReportDetails;
extern volatile bool run;

void tuya_init(struct Arguments tuyaArguments, tuya_mqtt_context_t *client, int *ret)
{
    *ret = tuya_mqtt_init(client, &(const tuya_mqtt_config_t) {
        .host = "m1.tuyacn.com",
        .port = 8883,
        .cacert = tuya_cacert_pem,
        .cacert_len = sizeof(tuya_cacert_pem),
        .device_id = tuyaArguments.deviceId,
        .device_secret = tuyaArguments.deviceSecret,
        .keepalive = 100,
        .timeout_ms = 2000,
        .on_messages = on_messages,
    });

    if (*ret){
        syslog(LOG_USER | LOG_ERR, "tuya_mqtt_init failed");
        run = false;
        return;
    }
    
    *ret = tuya_mqtt_connect(client);
    if (*ret){
        syslog(LOG_USER | LOG_ERR, "tuya_mqtt_connect failed");
        run = false;
        return;
    }

    while(run && !tuya_mqtt_connected(client)){
        tuya_mqtt_loop(client);
    }
}

void on_messages(tuya_mqtt_context_t *context, void *user_data, const tuyalink_message_t *msg)
{
    switch (msg->type) {
    case THING_TYPE_ACTION_EXECUTE:
        strcpy(message.msg_in_json, msg->data_string);
        convert_message_from_json(&message); // If this function fails, message.port equals to NULL
        if (message.pin == NULL)
            return;

        if (!strcmp(message.action, "on"))
            if (ubus_invoke(tuyaReportDetails.ctx, tuyaReportDetails.id, "on", blob_attr_for_ubus_methods(), NULL, NULL, 3000))
                syslog(LOG_USER | LOG_INFO, "UBUS invoke failed");
        
        if (!strcmp(message.action, "off"))
            if (ubus_invoke(tuyaReportDetails.ctx, tuyaReportDetails.id, "off", blob_attr_for_ubus_methods(), NULL, NULL, 3000))
                syslog(LOG_USER | LOG_INFO, "UBUS invoke failed");
        
        break;

    default:
        break;
    }
}

void tuya_loop()
{
    while (run) {
        tuya_mqtt_loop(tuyaReportDetails.client);

        if (ubus_invoke(tuyaReportDetails.ctx, tuyaReportDetails.id, "devices", NULL, ubus_devices_cb, NULL, 3000))
            syslog(LOG_USER | LOG_INFO, "Devices data on the Tuya cloud may be incorrect due to a failed UBUS invocation");

        tuyalink_thing_property_report(tuyaReportDetails.client, tuyaReportDetails.deviceId, tuyaReportDetails.message);
    }
}