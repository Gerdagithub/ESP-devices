#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <syslog.h>
#include <libubus.h>

#define BAUD_RATE B9600  

void communicate_with_esp_dev(char command[50], char port[20]);
int send_command(const char *command, int fd);
int read_response(int fd);