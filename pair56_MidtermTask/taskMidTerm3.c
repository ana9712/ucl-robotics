/*
  Created by Matthew Bell and Wayne Tsui.
  This programme sets the ActivityBot to go down a narrow passage, stopping at the end, rotating and retracing its steps
  The robot navigates to the end of the passage with sensors, but the movement back to the start point is without sensors.
*/


#include "simpletools.h"
#include "abdrive.h"
#include "librobot.h"
#include "ping.h"


int main() {
  int distance, stoppingDist = 8;
  int errorVal, prevErrorVal, totalErrorVal = 0, errorDiff = 0;
  int kp = -4, ki = -2, kd = -2;
  int baseSpd = 128, correctionSpd;
  int irLeft = 0, irRight = 0;
  
  node_correctionSpd* head = NULL;

  low(26);
  low(27);

  while(1) {

    // Check front distance.
    distance = ping_cm(8);

    // No obstacle within 5cm, can move forward.
    if (distance > stoppingDist) {

      // Get left wall distance, irLeft. 0<=irLeft<=20. Higher irLeft means object further from left side.
      // Get right wall distance, irRight. 0<=irRight<=20. Higher irRight means object further from right side.
      for (int dacVal = 0; dacVal < 160; dacVal += 8) {
        dac_ctr(26, 0, dacVal);
        freqout(11, 1, 38000);
        irLeft += input(10);                          // Left side IR-sensor

        dac_ctr(27, 1, dacVal);
        freqout(1, 1, 38000);
        irRight += input(2);                          // Right side IR-sensor
      }

      errorVal = irRight - irLeft;

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

      // Robot is equidistant from both left and right side walls.
      if (errorVal == 0) {

        // Move straight.
        drive_speed(baseSpd, baseSpd);

        // Push value to linked list. 0 means no correction speed.
        push(&head, 0);

      }
      else {

        correctionSpd = (kp * errorVal) + (ki * totalErrorVal) + (kd * errorDiff);

        if (correctionSpd > 50) {
          correctionSpd = 50;
        }
        if (correctionSpd < -50) {
          correctionSpd = -50;
        }

        totalErrorVal += errorVal;

        // Robot is too near to right wall.
        if (errorVal < 0) {
          // Move left, correctionSpd is positive.
          drive_speed(baseSpd-correctionSpd, baseSpd);

          // Push value of correctionSpd to linked list.
          push(&head, correctionSpd);
        }

        // Robot is too near to left wall.
        else if (errorVal > 0) {
          // Move right, correctionSpd is negative.
          drive_speed(baseSpd, baseSpd+correctionSpd);

          // Push value of correctionSpd to linked list.
          push(&head, correctionSpd);
        }
      }
    }

    // Obstacle within 5cm, cannot move forward, stop.
    // Execute return path movements WITHOUT sensors.
    else {
      // End of course, turn around.
      drive_setRampStep(1);
      drive_speed(0,0);
      turn_pivot_function(180);
      drive_setRampStep(4);

      node_correctionSpd* current = head;
      while (current != NULL) {
        int i = current->val;
        if (i == 0) {
          drive_speed(baseSpd, baseSpd);
        }
        else if (i > 0) {
          drive_speed(baseSpd, baseSpd-i);
        }
        else {
          drive_speed(baseSpd+i, baseSpd);
        }
        pause(64);
        current = current->next;
      }
      drive_setRampStep(1);
      drive_speed(0,0);
      turn_pivot_function(180);
      break;
    }
  }
}
