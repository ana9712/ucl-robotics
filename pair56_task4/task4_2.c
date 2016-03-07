/*
  Created by Matthew Bell and Wayne Tsui.
  This programme sets a robot to curve along a left wall and stops when an obstacle is 5cm ahead of it.
  The x-coordinates, y-coordinates, total distance travelled and angle change (relative to starting position) are displayed on Terminal.
*/

#include "simpletools.h"
#include "abdrive.h"
#include "librobot.h"
#include "ping.h"


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
        print("Distance travelled: %.2f mm. Angle from start point: %.2f degrees.\n", distanceTravelled, *(positionCoordinates+2) * 180/PI);
        
        // Execute print values only once when robot stops.
        flag = 0;
        
      }
    }
  }
}
