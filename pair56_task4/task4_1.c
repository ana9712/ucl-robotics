/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials
*/

#include "simpletools.h"
#include "abdrive.h"
#include "ping.h"

#define _PROPELLER_IO_PIN 8

int main() {

  //int distance; setPoint = 10 , errorVal, kp = -10, speed;
  int irLeft, irRight;

  low(26);
  low(27);

  drive_setRampStep(10);

  while(ping_cm(_PROPELLER_IO_PIN) > 5) {
    freqout(11, 1, 38000);                      // Left IR LED light
    irLeft = input(10);                         // Check left IR detector

    freqout(1, 1, 38000);                       // Repeat for right detector
    irRight = input(2);

    if (irLeft == 1) {
      drive_ramp(126, 128);
    }
    else if (irLeft == 0) {
      drive_ramp(128, 126);
    }

  }
  // Stop infront of obstacle.
  drive_ramp(0, 0);
}
