#include <ubus.h>
#include <daemon.h>
#include <argp_for_esp.h>
#include <additional.h>
#include <syslog.h>
#include <libserialport.h>

extern struct DevicesInfo arguments;
extern struct TuyaReport tuyaReportDetails;
extern volatile bool run;

int main(int argc, char **argv) 
{
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);
    int ret = 0;
    struct Arguments tuyaArguments;
    tuyaArguments.becomeDaemon = false;

    const char *LOGNAME = "Tuya IoT for ESP";
    openlog(LOGNAME, LOG_PID, LOG_USER); 

    argp_parse(&argp, argc, argv, 0, 0, &tuyaArguments);
    strcpy(tuyaReportDetails.deviceId, tuyaArguments.deviceId);

    if (tuyaArguments.becomeDaemon)
        ret = become_daemon(0);

    if (ret) {
        syslog(LOG_USER | LOG_INFO, "Failed to daemonise the program");
		run = false;
    }

    if (run)
        ret = ubus_init();

    if (!ret) {
        tuya_mqtt_context_t client_instance;
        tuyaReportDetails.client = &client_instance;
        tuya_init(tuyaArguments, tuyaReportDetails.client, &ret);
        tuya_loop();
    }

    tuya_mqtt_disconnect(tuyaReportDetails.client);
    tuya_mqtt_deinit(tuyaReportDetails.client);

    syslog(LOG_USER | LOG_INFO, "Tuya IoT for ESP devices has ended");
    closelog();

    return ret;
}