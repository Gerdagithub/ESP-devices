#ifndef ADDITIONAL_FOR_ESP_H
#define ADDITIONAL_FOR_ESP_H

#include <cJSON.h>
#include <syslog.h>
#include <stdbool.h>
                        
bool port_exists(char portName[20]);
bool pin_exists(int pin);

#endif