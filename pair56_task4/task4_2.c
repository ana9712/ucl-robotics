/*
  Created by Matthew Bell and Wayne Tsui.
  This programme sets a robot to curve along a left wall and stops when an obstacle is 5cm ahead of it.
*/

#include "simpletools.h"
#include "abdrive.h"
#include "librobot.h"
#include "ping.h"

/*
  Test IR Detectors for Distance.c
*/
/*
#include "simpletools.h"

int irLeft, irRight;

int main()
{
  low(26);
  low(27);

  while(1)
  {
    irLeft = 0;                                     // <- add
    irRight = 0;                                    // <- add

    for(int dacVal = 0; dacVal < 160; dacVal += 8)  // <- add
    {                                               // <- add
      dac_ctr(26, 0, dacVal);                       // <- add
      freqout(11, 1, 38000);                        // <- add
      irLeft += input(10);                          // <- modify


    }                                               // <- add

    print("%c irLeft = %d%c",         // <- modify
           HOME,   irLeft, CLREOL);        // <- modify
    pause(100);
  }
}
*/

/*
int distLeft[4], distRight[4];

int main()
{
  drive_getTicks(&distLeft[0], &distRight[0]);

  print("distLeft[0] = %d, distRight[0] = %d\n", distLeft[0], distRight[0]);

  drive_speed(80, 60);
  pause(2000);
  drive_speed(0, 0);

  drive_getTicks(&distLeft[1], &distRight[1]);

  print("distLeft[1] = %d, distRight[1] = %d\n", distLeft[1], distRight[1]);
}*/

int main() {
  int distance, stoppingDist = 5;
  int setPoint = 12;
  int errorVal, prevErrorVal, totalErrorVal = 0, errorDiff = 0;
  int kp = -8, ki = -3, kd = -3;
  int baseSpd = 86, correctionSpd;
  int irLeft = 0, irRight = 0;
  int flag = 1;

  double* distanceWheelsTravelled = (double*)malloc(2 * sizeof(double));
  double* positionCoordinates = (double*)malloc(3 * sizeof(double));
  double distanceTravelled = 0;
  
  low(26);
  low(27);

  while(1) {

    // Check front distance.
    distance = ping_cm(8);

    // No obstacle within 5cm, can move forward.
    if (distance > stoppingDist) {

      // Get left wall distance, irLeft. 0<=irLeft<=20. Higher irLeft means object further.
      for (int dacVal = 0; dacVal < 160; dacVal += 8) {
        dac_ctr(26, 0, dacVal);
        freqout(11, 1, 38000);
        irLeft += input(10);                          // Left side IR-sensor

        //dac_ctr(27, 1, dacVal);
        //freqout(1, 1, 38000);
        //irRight += input(2);                          // Right side IR-sensor
      }

      errorVal = setPoint - irLeft;

      // If robot moves from too far left/right to too far right/left, without hitting optimal setPoint, reset totalErrorVal.
      if ((prevErrorVal > 0 && errorVal <= 0) || (prevErrorVal < 0 && errorVal >= 0)) {
         totalErrorVal = 0;
      }

      // Measure difference in error values to turn more responsively at sharp turns.
      errorDiff = errorVal - prevErrorVal;

      prevErrorVal = errorVal;
      //print("%d\n", errorDiff);

      // Reset distance measurement.
      irLeft = 0;
      irRight = 0;
      //print("%d\n", errorVal);


      // Robot is 10cm away from left wall, optimal.
      if (errorVal == 0) {

        // Move straight.
        drive_speed(baseSpd, baseSpd);

      }
      else {

        correctionSpd = (kp * errorVal) + (ki * totalErrorVal) + (kd * errorDiff);

        if (correctionSpd > 30) {
          correctionSpd = 30;
        }
        if (correctionSpd < -30) {
          correctionSpd = -30;
        }

        totalErrorVal += errorVal;
        //print("%d\n",totalErrorVal);

        // Robot is too far from left wall.
        if (errorVal < 0) {
          // Move left, correctionSpd is positive.
          drive_speed(baseSpd-correctionSpd, baseSpd);
        }

        // Robot is too near to left wall.
        else if (errorVal > 0) {
          // Move right, correctionSpd is negative.
          drive_speed(baseSpd, baseSpd+correctionSpd);
        }
      }
    }

    // Obstacle within 5cm, cannot move forward, stop.
    else {
      drive_speed(0, 0);
      if (flag == 1) {
        // To display distance travelled and angle, with respect to starting position.
        distanceWheelsTravelled = distance_wheels_travelled();

        // test if function returns the distances of each wheel ie. if getTicks work.
        print("Left wheel moved: %.2f mm. Right wheel moved: %.2f mm.\n", *distanceWheelsTravelled, *(distanceWheelsTravelled+1));

        positionCoordinates = position_change(distanceWheelsTravelled, 0);
        print("Position X: %.2f Position Y: %.2f\n", *positionCoordinates, *(positionCoordinates+1));
        
        distanceTravelled = distance_travelled(positionCoordinates);
        print("Distance travelled: %.2f mm. Angle from start point: %.2f degrees.\n", distanceTravelled, *(positionCoordinates+2) * PI/180);
        
        // Execute print values only once when robot stops.
        flag = 0;
        
        // Test code for SD Card
        /*
        sd_mount(DO, CLK, DI, CS);
        FILE* fp = fopen("task4_2.txt", "w");
        log_write(fp, positionCoordinates);
        fclose(fp);
        */
      }
    }
  }
}
