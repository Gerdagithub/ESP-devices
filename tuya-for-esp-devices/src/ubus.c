#include <ubus.h>
#include <tuya.h>

extern volatile bool run;
extern struct Message message;
extern struct TuyaReport tuyaReportDetails;

void ubus_devices_cb(struct ubus_request *req, int type, struct blob_attr *msg)
{
    char devicesInJson[300];
    struct blob_buf buf = {};
    blob_buf_init(&buf, 0);
   
    strcpy(tuyaReportDetails.message, blobmsg_format_json(msg, true));
    if (tuyaReportDetails.message == NULL)
        syslog(LOG_ERR, "Failed formatting blob message");

    strcpy(tuyaReportDetails.message, convert_devices_json_array(tuyaReportDetails.message));
    if (tuyaReportDetails.message == NULL)
        syslog(LOG_ERR, "Failed converting devices json array to strings array");
}

struct blob_attr* blob_attr_for_ubus_methods()
{
    struct blob_buf buf = {};
    blob_buf_init(&buf, 0);
    blobmsg_add_string(&buf, "port", message.port);
    blobmsg_add_u32(&buf, "pin", message.pin);

    return buf.head;
}

int ubus_init()
{
    tuyaReportDetails.ctx = ubus_connect(NULL);
	if (!(tuyaReportDetails.ctx)) {
        syslog(LOG_USER | LOG_INFO, "Failed to connect to ubus");
		run = false;
        return -1;
	}

	if (run && ubus_lookup_id(tuyaReportDetails.ctx, "esp_micro_controllers", &(tuyaReportDetails.id))) {
        syslog(LOG_USER | LOG_INFO, "ubus_lookup_id failed");
        run = false;
		return -1;
	}

    return 0;
}