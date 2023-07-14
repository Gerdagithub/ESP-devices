#include <additional.h>

volatile bool run = true;

extern volatile bool run;
extern struct TuyaReport tuyaReportDetails;
extern struct Message message;

void sig_handler(int signal)
{
    run = false;
}

char* convert_devices_json_array(char jsonMessage[200])
{
    cJSON *root = cJSON_Parse(jsonMessage);

    if (root == NULL) {
        syslog(LOG_ERR, "Failed parsing json string");
        return NULL;
    }

    cJSON *devicesObj = cJSON_GetObjectItem(root, "devices");
    int amntOfDevs = cJSON_GetArraySize(devicesObj);
    cJSON *devicesArr = cJSON_CreateArray();

    for (int i = 0; i < amntOfDevs; i++) {
        cJSON *device = cJSON_GetArrayItem(devicesObj, i);

        cJSON *pid          = cJSON_GetObjectItem(device, "product_id");
        cJSON *vid          = cJSON_GetObjectItem(device, "vendor_id");
        cJSON *port         = cJSON_GetObjectItem(device, "port");

        char device_string[512];
        sprintf(device_string, "product_id: %s, vendor_id: %s, port: %s", pid->valuestring, vid->valuestring, port->valuestring);

        cJSON *device_united = cJSON_CreateString(device_string);
        cJSON_AddItemToArray(devicesArr, device_united);
    }  

    cJSON *devices_json = cJSON_CreateObject();
    cJSON_AddItemToObject(devices_json, "devices", devicesArr);

    return cJSON_Print(devices_json);
}

void convert_message_from_json(struct Message *mess)
{
    char json_string[50];
    strcpy(json_string, mess->msg_in_json);

    cJSON *root = cJSON_Parse(json_string);
    if (!root) {
        syslog(LOG_USER | LOG_ERR, "Error parsing JSON");
        run = false;
        return;
    }

    cJSON *actionCode = cJSON_GetObjectItem(root, "actionCode");
    cJSON *inputParams = cJSON_GetObjectItem(root, "inputParams");
    cJSON *port = cJSON_GetObjectItem(inputParams, "port");
    cJSON *pin = cJSON_GetObjectItem(inputParams, "pin");

    if (!inputParams || !port || !pin || !actionCode ||
        port->type != cJSON_String || pin->type != cJSON_Number || actionCode->type != cJSON_String) {
        syslog(LOG_USER | LOG_ERR, "Invalid JSON format");
        mess->pin = NULL;

        cJSON_Delete(root);
        return;
    }

    strcpy(mess->action, actionCode->valuestring);
    strcpy(mess->port, port->valuestring);
    mess->pin = pin->valueint;

    cJSON_Delete(root);
}