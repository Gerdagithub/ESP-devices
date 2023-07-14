#include <ubus_controller.h>
#include <additional.h>
#include <serial_communication.h>

extern struct TuyaReport tuyaReportDetails;
extern volatile bool run;

int get_devices(struct ubus_context *ctx, struct ubus_object *obj,
		        struct ubus_request_data *req, const char *method,
		        struct blob_attr *msg)
{

    struct sp_port** portList;
    sp_list_ports(&portList);
    int portCount = 0;
    struct sp_port** portIterator = portList;

    char HexStr[5]; // Hexadecimal string buffer for vendor ID
    struct blob_buf buf = {};
    blob_buf_init(&buf, 0);
    void *devicesArray;
    devicesArray = blobmsg_open_array(&buf, "devices");

    while (*portIterator != NULL){
        const char* portName = sp_get_port_name(*portIterator);
        int vid, pid;
        if (sp_get_port_usb_vid_pid(*portIterator, &vid, &pid) == SP_OK &&
            vid == VENDOR_ID && pid == PRODUCT_ID) {

            void* obj1 = blobmsg_open_table(&buf, NULL);

            blobmsg_add_string(&buf, "port", portName);

            snprintf(HexStr, sizeof(HexStr), "%04X", vid);
            blobmsg_add_string(&buf, "vendor_id", HexStr);

            snprintf(HexStr, sizeof(HexStr), "%04X", pid);
            blobmsg_add_string(&buf, "product_id", HexStr);

            blobmsg_close_table(&buf, obj1);
        }

        portCount++;
        portIterator++;
    }

    blobmsg_close_array(&buf, devicesArray);

    ubus_send_reply(ctx, req, buf.head);
    blob_buf_free(&buf);
    sp_free_port_list(portList);

    return 0;
}

int on(struct ubus_context *ctx, struct ubus_object *obj,
		struct ubus_request_data *req, const char *method,
        struct blob_attr *msg)
{
	struct blob_attr *tb[__PIN_ARGS_MAX];
	
	blobmsg_parse(control_policy, __PIN_ARGS_MAX, tb, blob_data(msg), blob_len(msg));
	
	if (!tb[PORT] || !tb[PIN] || !port_exists(blobmsg_get_string(tb[PORT])) || !pin_exists(blobmsg_get_u32(tb[PIN]))){
		return UBUS_STATUS_INVALID_ARGUMENT;
    }

    char command[50];
    snprintf(command, sizeof(command), "{\"action\": \"on\", \"pin\": %d}", blobmsg_get_u32(tb[PIN]));
    communicate_with_esp_dev(command, blobmsg_get_string(tb[PORT]));

    return 0;
}

int off(struct ubus_context *ctx, struct ubus_object *obj,
		struct ubus_request_data *req, const char *method,
        struct blob_attr *msg)
{
    struct blob_attr *tb[__PIN_ARGS_MAX];
	
	blobmsg_parse(control_policy, __PIN_ARGS_MAX, tb, blob_data(msg), blob_len(msg));
	
	if (!tb[PORT] || !tb[PIN] || !port_exists(blobmsg_get_string(tb[PORT])) || !pin_exists(blobmsg_get_u32(tb[PIN])))
		return UBUS_STATUS_INVALID_ARGUMENT;

    char command[50];
    snprintf(command, sizeof(command), "{\"action\": \"off\", \"pin\": %d}", blobmsg_get_u32(tb[PIN]));
    communicate_with_esp_dev(command, blobmsg_get_string(tb[PORT]));

    return 0;
}

int ubus_loop()
{
    struct ubus_context *ctx;
	uloop_init();

	ctx = ubus_connect(NULL);
	if (!ctx) {
		syslog(LOG_ERR, "Failed to connect to ubus");
        run = false;
		return -1;
	}

	ubus_add_uloop(ctx);
	ubus_add_object(ctx, &devices_object);
	uloop_run();

	ubus_free(ctx);
	uloop_done();

    return 0;
}