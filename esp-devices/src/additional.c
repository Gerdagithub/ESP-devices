#include <additional.h>
#include <ubus_controller.h>

int pins[] = {0, 2, 4, 5, 12, 13, 14, 15, 16};

volatile bool run = true;

bool port_exists(char portName[30])
{
    char temp[25];
    struct sp_port** portList;
    sp_list_ports(&portList);
    int portCount = 0;
    struct sp_port** portIterator = portList;

    while (*portIterator != NULL){
        const char* currPortName = sp_get_port_name(*portIterator);
        int vid, pid;
        if (sp_get_port_usb_vid_pid(*portIterator, &vid, &pid) == SP_OK &&
            vid == VENDOR_ID && pid == PRODUCT_ID && !strcmp(portName, currPortName)) 
                return true;
        
        portCount++;
        portIterator++;        
    }

    sprintf(temp, "Port %s does not exist", portName);
    syslog(LOG_USER | LOG_INFO, temp);
    return false;
}

bool pin_exists(int pin)
{
    for (int i = 0; i < sizeof(pins) / sizeof(int); i++){
        if (pins[i] == pin)
            return true;
    }

    char temp[31];
    sprintf(temp, "Pin %d does not exist", pin);
    syslog(LOG_USER | LOG_INFO, temp);
    return false;
}