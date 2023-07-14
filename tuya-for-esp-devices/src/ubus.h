#ifndef UBUS_FOR_TUYA_H
#define UBUS_FOR_TUYA_H

#include <libubus.h>
#include <libubox/blobmsg_json.h>

void ubus_devices_cb(struct ubus_request *req, int type, struct blob_attr *msg);
struct blob_attr* blob_attr_for_ubus_methods();
int ubus_init();

#endif