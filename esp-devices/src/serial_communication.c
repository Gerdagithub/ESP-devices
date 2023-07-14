#include <serial_communication.h>

extern volatile bool run;

void communicate_with_esp_dev(char command[50], char port[20])
{
    int fd = open(port, O_RDWR | O_NOCTTY);
    if (fd == -1) {
        syslog(LOG_USER | LOG_ERR, "Error opening serial port");
    }

    if (send_command(command, fd) == -1) {
        return UBUS_STATUS_UNKNOWN_ERROR;
    }else if (read_response(fd)){
        syslog(LOG_USER | LOG_ERR, "Error reading response from esp device");
    }

    close(fd);
}

// Function to send command to the microcontroller
int send_command(const char *command, int fd) 
{
    struct termios serialConfig;
    tcgetattr(fd, &serialConfig);
    cfsetispeed(&serialConfig, BAUD_RATE);
    cfsetospeed(&serialConfig, BAUD_RATE);
    serialConfig.c_cflag |= CS8;
    serialConfig.c_cflag &= ~PARENB;
    tcsetattr(fd, TCSANOW, &serialConfig);

    ssize_t bytesWritten = write(fd, command, strlen(command));
    if (bytesWritten == -1) {
        syslog(LOG_USER | LOG_ERR, "Error writing to serial port");
    }

    return 0;
}

int read_response(int fd)
{
    char response[100];
    ssize_t bytesRead = read(fd, response, sizeof(response) - 1);
    if (bytesRead == -1) {
        syslog(LOG_USER | LOG_ERR, "Error reading from serial port");
        return -1;
    }

    response[bytesRead] = '\0'; 
    syslog(LOG_USER | LOG_INFO, response);

    return 0;
}