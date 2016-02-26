/*
  Created by Matthew Bell and Wayne Tsui.
  This programme sets a robot to curve along a left wall and stops when an obstacle is 5cm ahead of it.
*/

#include "simpletools.h"
#include "abdrive.h"
#include "librobot.h"
#include "ping.h"
#include "stdio.h"

int stoppingDist = 5;
int distance = 20;

int check_dist() {
  distance = ping_cm(8);
  if (distance < stoppingDist)
    return 1;
  else return 0;
}

int main() {

  //int distance; setPoint = 10 , errorVal, kp = -10, speed;
  int irLeft, irRight;
    
  int startSpd = 60;
  int uBound = 70;
  int lBound = 30;
  int leftSpd = startSpd;
  int rightSpd = startSpd;

  low(26);
  low(27);

  while (check_dist() == 0) {
    drive_speed(leftSpd,rightSpd);

    freqout(11, 1, 38000);
    irLeft = input(10);

    printf("%d\n",irLeft);

    if (irLeft == 0) {
      if (leftSpd < uBound)
        leftSpd++;
      if (rightSpd > lBound)
        rightSpd--;
    }
    else {
      if (leftSpd > lBound)
        leftSpd--;
      if (rightSpd < uBound)
        rightSpd++;
    }

    pause(10);
  }

  drive_speed(0,0);
}
