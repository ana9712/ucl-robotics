/*
  Created by Matthew Bell and Wayne Tsui.
  This programme sets a robot to curve along a left wall and stops when an obstacle is 5cm ahead of it.
*/

#include "simpletools.h"
#include "abdrive.h"
#include "librobot.h"
#include "ping.h"

int stoppingDist = 5;
int distance;

int check_dist() {
  distance = ping_cm(8);
  if (distance < stoppingDist)
    return 1;
  else return 0;
}  

int main() {

  //int distance; setPoint = 10 , errorVal, kp = -10, speed;
  int irLeft, irRight;

  int leftSpd = 10;
  int rightSpd = 10;

  low(26);
  low(27);
  
  while(1) {
    
    if (check_dist() == 0) {

      freqout(11, 1, 38000);                      // Left IR LED light
      irLeft = input(10);                         // Get Left IR LED light input

      //freqout(1, 1, 38000);                     // Repeat for right detector
      //irRight = input(2);

      while (irLeft == 0) {
        leftSpd = leftSpd + 1;
        drive_speed(leftSpd,rightSpd);
        freqout(11, 1, 38000);                     
        irLeft = input(10);
        if (check_dist() == 1) {
          break;
        }                   
      }
      leftSpd = 10;
      while (irLeft == 1) {
        rightSpd = rightSpd + 1;
        drive_speed(leftSpd,rightSpd);
        freqout(11, 1, 38000);                     
        irLeft = input(10);
        if (check_dist() == 1) {
          break;
        }            
      }
      rightSpd = 10;

    }
    else {
      break;
    }
  }
}
