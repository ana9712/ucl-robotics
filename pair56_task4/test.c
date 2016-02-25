#include "simpletools.h"
#include "abdrive.h"
#include "ping.h"
#include "librobot.h"

#define _PROPELLER_IO_PIN 8

int distance = 20;

int main() {
    while(distance > 10)
    {
        drive_ramp(50,50);
        distance = ping_cm(8);
        print("%c distance = %d%c cm",
                HOME, distance, CLREOL);
        pause(200);
    }
    drive_ramp(0,0);
}
