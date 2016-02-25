/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials
*/

#include "simpletools.h"
#include "abdrive.h"
#include "librobot.h"
#include "ping.h"

#define _PROPELLER_IO_PIN 8

int main() {

  //int distance; setPoint = 10 , errorVal, kp = -10, speed;
  int irLeft, irRight;
  int distance;
  int stoppingDist = 5;

  low(26);
  low(27);

  drive_setRampStep(10);
  while(1) {
    distance = ping_cm(8);
  if (distance > 5) {
    /*
    print("%c distance = %d%c cm",            // Display distance
           HOME, distance, CLREOL);

    pause(200);                               // Wait 1/5 second
    */

    freqout(11, 1, 38000);                      // Left IR LED light
    irLeft = input(10);

    //freqout(1, 1, 38000);                       // Repeat for right detector
    //irRight = input(2);

    if (irLeft == 0) {
      turn_function(7);
    }
    else if (irLeft == 1) {
      drive_goto(20,20);
      turn_function(-9);
    }

  }
    else {
    break;
    }
  }
  // Stop infront of obstacle.
  drive_ramp(0, 0);
}
