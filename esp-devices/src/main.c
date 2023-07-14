#include <ubus_controller.h>
#include <additional.h>
#include <syslog.h>
#include <stdbool.h>
#include <serial_communication.h>

extern volatile bool run;

void sig_handler(int signal)
{
    run = false;
}

int main(int argc, char **argv) {
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

    int ret = 0;
    
    const char *LOGNAME = "ESP micro controllers";
    openlog(LOGNAME, LOG_PID, LOG_USER); 

    if (ret)
        syslog(LOG_USER | LOG_INFO, "Failed to daemonize the program");
    else {
        ret = ubus_loop();
        if (ret)
            syslog(LOG_USER | LOG_INFO, "Failed to run ubus loop");
    }

    syslog(LOG_USER | LOG_INFO, "UBUS module for ESP micro controllers has ended");
    closelog();

    return ret;
}