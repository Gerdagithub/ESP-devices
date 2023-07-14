#ifndef UBUS_CONTROLLER_H
#define UBUS_CONTROLLER_H

#include <libubus.h>
#include <libserialport.h>
#include <libubox/blobmsg_json.h>
#include <stdbool.h>
#include <syslog.h>

#define MAX_AMOUNT_OF_DEVICES 200
#define VENDOR_ID 0x10c4
#define PRODUCT_ID 0xea60

enum {
    PORT,
    PIN,
    __PIN_ARGS_MAX,
};

int get_devices(struct ubus_context *ctx, struct ubus_object *obj,
		        struct ubus_request_data *req, const char *method,
		        struct blob_attr *msg);

int on(struct ubus_context *ctx, struct ubus_object *obj,
	   struct ubus_request_data *req, const char *method, struct blob_attr *msg);

int off(struct ubus_context *ctx, struct ubus_object *obj,
	   struct ubus_request_data *req, const char *method, struct blob_attr *msg);

int ubus_loop();

static const struct blobmsg_policy control_policy[__PIN_ARGS_MAX] = {
    [PORT] = { .name = "port", .type = BLOBMSG_TYPE_STRING },
    [PIN]  = { .name = "pin", .type = BLOBMSG_TYPE_INT32 },
};

static const struct ubus_method device_methods[] = {
    UBUS_METHOD_NOARG("devices", get_devices),
    UBUS_METHOD("on", on, control_policy),
    UBUS_METHOD("off", off, control_policy)
};

static struct ubus_object_type device_object_type =
	UBUS_OBJECT_TYPE("esp-devices", device_methods);


static struct ubus_object devices_object = {
	.name = "esp_micro_controllers",
	.type = &device_object_type,
	.methods = device_methods,
	.n_methods = ARRAY_SIZE(device_methods),
};

#endif