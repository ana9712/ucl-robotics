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

int main() {
  int distance, stoppingDist = 5;
  int setPoint = 12;
  int errorVal, prevErrorVal, totalErrorVal = 0, errorDiff = 0;
  int kp = -6, ki = -2, kd = -4;
  int baseSpd = 128, correctionSpd;
  int irLeft = 0, irRight = 0;
  double* distanceWheelsTravelled = malloc(2 * sizeof(double));
  double* positionCoordinates = malloc(3 * sizeof(double));
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

      // Reset distance measurement.
      irLeft = 0;
      irRight = 0;
      //print("%d\n", errorVal);


      // Robot is 10cm away from left wall, optimal.
      if (errorVal == 0) {

        // Move straight.
        drive_speed(baseSpd, baseSpd);

        // To display distance travelled and angle, with respect to starting position.
        distanceWheelsTravelled = distance_wheels_travelled();
        positionCoordinates = position_change(distanceWheelsTravelled, 0);
        distanceTravelled = distance_travelled(positionCoordinates);
        print("Distance travelled: %.2f. Angle from start point: %.2f degrees.", distanceTravelled, *(positionCoordinates+2));

      }
      else {

        correctionSpd = (kp * errorVal) + (ki * totalErrorVal) + (kd * errorDiff);

        if (correctionSpd > baseSpd/3) {
          correctionSpd = baseSpd/3;
        }
        if (correctionSpd < -baseSpd/3) {
          correctionSpd = -baseSpd/3;
        }

        totalErrorVal += errorVal;
        //print("%d\n",totalErrorVal);

        // Robot is too far from left wall.
        if (errorVal < 0) {
          // Move left, correctionSpd is positive.
          drive_speed(baseSpd-correctionSpd, baseSpd);

          // To display distance travelled and angle, with respect to starting position.
          distanceWheelsTravelled = distance_wheels_travelled();
          positionCoordinates = position_change(distanceWheelsTravelled, 0);
          distanceTravelled = distance_travelled(positionCoordinates);
          print("Distance travelled: %.2f. Angle from start point: %.2f degrees.", distanceTravelled, *(positionCoordinates+2));
        }

        // Robot is too near to left wall.
        else if (errorVal > 0) {
          // Move right, correctionSpd is negative.
          drive_speed(baseSpd, baseSpd+correctionSpd);

          // To display distance travelled and angle, with respect to starting position.
          distanceWheelsTravelled = distance_wheels_travelled();
          positionCoordinates = position_change(distanceWheelsTravelled, 0);
          distanceTravelled = distance_travelled(positionCoordinates);
          print("Distance travelled: %.2f. Angle from start point: %.2f degrees.", distanceTravelled, *(positionCoordinates+2));
        }
      }
    }

    // Obstacle within 5cm, cannot move forward, stop.
    else {
      drive_speed(0, 0);
    }
  }
}
