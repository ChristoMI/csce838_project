#include <sys/ioctl.h>
#include <fstream>
#include <unistd.h>
#include <sys/fcntl.h>
#include <linux/watchdog.h>
#include <iostream>
#include <string>

#define OPEN_WDT 0
#define GET_WDT_TIMEOUT 1
#define SET_WDT_TIMEOUT 2
#define KICK_WDT_TIMER 3
#define DISABLE_WDT_TIMER 4

int ret = 0;
int timeout = 0;
std::string executable_path = "/home/car/ryan_brown_ws/src/bluetooth_com/src/ErrorLog";

int main (int argc, char* argv[]){

    int wdt_operation_code = atoi(argv[1]);
    int fd = 0;

    // open WDT0 device (WDT0 enables itself automatically)
    fprintf(stdout, "Trying to open the watchdog!\n");
    
    fd =  open("/dev/watchdog", O_RDWR);
    if(fd < 0) {
        fprintf(stdout, "Open watchdog device failed!\n");
        return -1; 
    }
    fprintf(stdout, "Watchdog opened!\n");

    if(wdt_operation_code == GET_WDT_TIMEOUT){ // WDT0 is counting now,check the default timeout value
        ret = ioctl(fd, WDIOC_GETTIMEOUT, &timeout);

        if(ret) {
            fprintf(stderr, "Get watchdog timeout value failed!\n");
            return -1; 
        }

        fprintf(stdout, "Watchdog timeout value: %d\n", timeout);
    }
    else if(wdt_operation_code == SET_WDT_TIMEOUT){ // set new timeout value; note the value should be within [5, 1000]
        timeout = atoi(argv[2]);

        ret = ioctl(fd, WDIOC_SETTIMEOUT, &timeout);

        if(ret) {
            fprintf(stderr, "Set watchdog timeout value failed!\n");
            return -1;
        }

        fprintf(stdout, "New watchdog timeout value: %d\n", timeout);
    }
    else if(wdt_operation_code == KICK_WDT_TIMER){ // kick WDT0, this should be running periodically
        ret = ioctl(fd, WDIOC_KEEPALIVE, NULL);
        if(ret) {
            fprintf(stderr, "Kick watchdog failed!\n");
            return -1;
        }

        fprintf(stdout, "Watchdog kicked!\n");
    }
    else if(wdt_operation_code == DISABLE_WDT_TIMER){
        int flags = WDIOS_DISABLECARD;

        ret = ioctl(fd, WDIOC_SETOPTIONS, &flags);
        
        if(ret) {
            fprintf(stderr, "Disable watchdog failed!\n");
            return -1;
        }

        fprintf(stdout, "Watchdog Disabled!\n");
    }
    else{
        std::cout << "Unsupported WDT operation...\n";

        return -2;
    } 

    close(fd);
    
    return 0;
}