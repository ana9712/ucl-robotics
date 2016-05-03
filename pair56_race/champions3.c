/*
  Created by Matthew Bell and Wayne Tsui.
  This is the race. Lets win this!!!
*/

#include "simpletools.h"
#include "abdrive.h"
#include "librobot.h"
#include "ping.h"

/*
//Test IR Detectors for Distance.c
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

      dac_ctr(27, 1, dacVal);                       // <- add
      freqout(1, 1, 38000);
      irRight += input(2);                          // <- modify
    }                                               // <- add

    print("%c irLeft = %d, irRight = %d%c",         // <- modify
           HOME,   irLeft, irRight, CLREOL);        // <- modify
    pause(100);
  }
}*/

#define _MOVE_UNIT (int)(410/3.25)
#define _FRONT_DIST (int)40
#define _MID_SPOT (int)17

int main() {
  int irLeft = 0, irRight = 0;
  int frontDist = 0;
  char direction = 'n';
  int index = 0, boxNum = -3;
  int pathSeq[40];
  int midPoint = 0;
  int ticksCounter = (int)_MOVE_UNIT;
  int errorVal, prevErrorVal, totalErrorVal = 0, errorDiff = 0;
  int kp = -4, ki = -2, kd = -2;
  int baseSpd = 80, correctionSpd;
  int ticks[2];
  ticks[0] = 0, ticks[1] = 0;
  // Store left, middle and right ping distance (for adjustment in grid before moving)
  int LMR[3];
  LMR[0] = 0, LMR[1] = 0, LMR[2] = 0;
  int rePos = 0;
  int flag = 0;

  low(26);
  low(27);

  for(int i=0; i<40; i++) {
    pathSeq[i] = 0;
  }

  while(flag == 0) {

    // Store array boxNum (box number the robot is in)
    pathSeq[index] = boxNum;
    //print("In box: %d\n", boxNum);
    index++;

    //freqout(11, 1, 38000);
    //irLeft = input(10);

    //freqout(1, 1, 38000);
    //irRight = input(2);

    // Update direction
    // Turn left.
    turn_pivot_function(-90);
    //Check for obstacles (front, left, right)
    pause(300);
    frontDist = ping_cm(8);
    //print("%d\n", frontDist);
    LMR[0] = frontDist;
    if (frontDist > _FRONT_DIST) {
      // Update new direction.
      switch(direction) {
        case 'n':
          direction = 'w';
          break;
        case 's':
          direction = 'e';
          break;
        case 'e':
          direction = 'n';
          break;
        case 'w':
          direction = 's';
          break;
      }

      // Move one box.
      while(((ticks[0]+ticks[1])/2) < ticksCounter) {
        for (int dacVal = 0; dacVal < 160; dacVal += 8) {
          dac_ctr(26, 0, dacVal);
          freqout(11, 1, 38000);
          irLeft += input(10);                          // Left side IR-sensor

          dac_ctr(27, 1, dacVal);
          freqout(1, 1, 38000);
          irRight += input(2);                          // Right side IR-sensor
        }

      errorVal = irRight - irLeft;

      if ((prevErrorVal > 0 && errorVal <= 0) || (prevErrorVal < 0 && errorVal >= 0)) {
         totalErrorVal = 0;
      }

      errorDiff = errorVal - prevErrorVal;

      prevErrorVal = errorVal;

      irLeft = 0;
      irRight = 0;

      correctionSpd = (kp * errorVal) + (ki * totalErrorVal) + (kd * errorDiff);

      if (errorVal == 0) {
        drive_speed(baseSpd, baseSpd);
      }

      // Robot is too near to right wall.
      else if (errorVal < 0) {
        // Move left, correctionSpd is positive.
        drive_speed(baseSpd-correctionSpd, baseSpd);
      }
      // Robot is too near to left wall.
      else if (errorVal > 0) {
        // Move right, correctionSpd is negative.
        drive_speed(baseSpd, baseSpd+correctionSpd);
      }
      drive_getTicks(&ticks[0], &ticks[1]);
      }
    }
    else {
      // Readjust position based on left ping distance.
      rePos = (LMR[0] - _MID_SPOT) * 10 / 3.25;
      drive_goto(rePos, rePos);
      ticksCounter += rePos;

      turn_pivot_function(90);
      //Check for obstacles (front, left, right)
      pause(300);
      frontDist = ping_cm(8);
      //print("%d\n", frontDist);
      LMR[1] = frontDist;
      if (frontDist > _FRONT_DIST) {
        // Move one box.
        print("%d\n", ((ticks[0]+ticks[1])/2));
        while(((ticks[0]+ticks[1])/2) < ticksCounter) {
          for (int dacVal = 0; dacVal < 160; dacVal += 8) {
            dac_ctr(26, 0, dacVal);
            freqout(11, 1, 38000);
            irLeft += input(10);                          // Left side IR-sensor

            dac_ctr(27, 1, dacVal);
            freqout(1, 1, 38000);
            irRight += input(2);                          // Right side IR-sensor
          }

          errorVal = irRight - irLeft;

          if ((prevErrorVal > 0 && errorVal <= 0) || (prevErrorVal < 0 && errorVal >= 0)) {
           totalErrorVal = 0;
          }

          errorDiff = errorVal - prevErrorVal;

          prevErrorVal = errorVal;

          irLeft = 0;
          irRight = 0;

          correctionSpd = (kp * errorVal) + (ki * totalErrorVal) + (kd * errorDiff);

          if (errorVal == 0) {
            drive_speed(baseSpd, baseSpd);
          }

          // Robot is too near to right wall.
          else if (errorVal < 0) {
            // Move left, correctionSpd is positive.
            drive_speed(baseSpd-correctionSpd, baseSpd);
          }
          // Robot is too near to left wall.
          else if (errorVal > 0) {
          // Move right, correctionSpd is negative.
          drive_speed(baseSpd, baseSpd+correctionSpd);
          }
          drive_getTicks(&ticks[0], &ticks[1]);
        }
      }
      else {
        // Readjust position based on front ping distance.
        rePos = (LMR[1] - _MID_SPOT) * 10 / 3.25;
        drive_goto(rePos, rePos);
        ticksCounter += rePos;

        // Turn right.
        turn_pivot_function(90);
        //Check for obstacles (front, left, right)
        pause(300);
        frontDist = ping_cm(8);
        //print("%d\n", frontDist);
        LMR[2] = frontDist;
        if (frontDist > _FRONT_DIST) {
          // Update new direction.
          switch(direction) {
            case 'n':
              direction = 'e';
              break;
            case 's':
              direction = 'w';
              break;
            case 'e':
              direction = 's';
              break;
            case 'w':
              direction = 'n';
              break;
          }

          // Move one box.
          while(((ticks[0]+ticks[1])/2) < ticksCounter) {
            for (int dacVal = 0; dacVal < 160; dacVal += 8) {
              dac_ctr(26, 0, dacVal);
              freqout(11, 1, 38000);
              irLeft += input(10);                          // Left side IR-sensor

              dac_ctr(27, 1, dacVal);
              freqout(1, 1, 38000);
              irRight += input(2);                          // Right side IR-sensor
            }

            errorVal = irRight - irLeft;

            if ((prevErrorVal > 0 && errorVal <= 0) || (prevErrorVal < 0 && errorVal >= 0)) {
              totalErrorVal = 0;
            }

            errorDiff = errorVal - prevErrorVal;

            prevErrorVal = errorVal;

            irLeft = 0;
            irRight = 0;

            correctionSpd = (kp * errorVal) + (ki * totalErrorVal) + (kd * errorDiff);

            if (errorVal == 0) {
              drive_speed(baseSpd, baseSpd);
            }

            // Robot is too near to right wall.
            else if (errorVal < 0) {
              // Move left, correctionSpd is positive.
              drive_speed(baseSpd-correctionSpd, baseSpd);
            }
            // Robot is too near to left wall.
            else if (errorVal > 0) {
              // Move right, correctionSpd is negative.
              drive_speed(baseSpd, baseSpd+correctionSpd);
            }
            drive_getTicks(&ticks[0], &ticks[1]);
          }
        }
        else {
          // Readjust position based on right ping distance.
          rePos = (LMR[2] - _MID_SPOT) * 10 / 3.25;
          drive_goto(rePos, rePos);
          ticksCounter += rePos;

          turn_pivot_function(90);
          //Check for obstacles (front, left, right)
          pause(300);
          frontDist = ping_cm(8);
          //print("%d\n", frontDist);
          // Update new direction.
          switch(direction) {
            case 'n':
              direction = 's';
              break;
            case 's':
              direction = 'n';
              break;
            case 'e':
              direction = 'w';
              break;
            case 'w':
              direction = 'e';
              break;
          }

          // Move one box.
          while(((ticks[0]+ticks[1])/2) < ticksCounter) {
            for (int dacVal = 0; dacVal < 160; dacVal += 8) {
              dac_ctr(26, 0, dacVal);
              freqout(11, 1, 38000);
              irLeft += input(10);                          // Left side IR-sensor

              dac_ctr(27, 1, dacVal);
              freqout(1, 1, 38000);
              irRight += input(2);                          // Right side IR-sensor
            }

            errorVal = irRight - irLeft;

            if ((prevErrorVal > 0 && errorVal <= 0) || (prevErrorVal < 0 && errorVal >= 0)) {
               totalErrorVal = 0;
            }

            errorDiff = errorVal - prevErrorVal;

            prevErrorVal = errorVal;

            irLeft = 0;
            irRight = 0;

            correctionSpd = (kp * errorVal) + (ki * totalErrorVal) + (kd * errorDiff);

            if (errorVal == 0) {
              drive_speed(baseSpd, baseSpd);
            }

            // Robot is too near to right wall.
            else if (errorVal < 0) {
              // Move left, correctionSpd is positive.
              drive_speed(baseSpd-correctionSpd, baseSpd);
            }
            // Robot is too near to left wall.
            else if (errorVal > 0) {
              // Move right, correctionSpd is negative.
              drive_speed(baseSpd, baseSpd+correctionSpd);
            }
            drive_getTicks(&ticks[0], &ticks[1]);
          }
        }
      }
    }
    drive_speed(0, 0);
    ticksCounter += (int)_MOVE_UNIT;

    // Update new boxNum (box the robot is in) after moving by checking updated direction(which direction the robot the coming from).
    switch(direction) {
      case 'n':
        boxNum = pathSeq[index-1] + 4;
        break;
      case 's':
        boxNum = pathSeq[index-1] - 4;
        break;
      case 'e':
        boxNum = pathSeq[index-1] + 1;
        break;
      case 'w':
        boxNum = pathSeq[index-1] - 1;
        break;
    }

    // Back to start point. End of Phase 1.
    if (boxNum == -3) {
      pathSeq[index] = boxNum;
      index++;

      // Final box, reposition for Phase 2.
      turn_pivot_function(-90);
      //Check for distance (left).
      pause(300);
      frontDist = ping_cm(8);
      //print("%d\n", frontDist);
      LMR[0] = frontDist;
      rePos = (LMR[0] - _MID_SPOT) * 10 / 3.25;
      drive_goto(rePos, rePos);
      ticksCounter += rePos;

      turn_pivot_function(90);
      //Check for distance (front).
      pause(300);
      frontDist = ping_cm(8);
      //print("%d\n", frontDist);
      LMR[1] = frontDist;
      rePos = (LMR[1] - _MID_SPOT) * 10 / 3.25;
      drive_goto(rePos, rePos);
      ticksCounter += rePos;

      turn_pivot_function(90);
      //Check for distance (right).
      pause(300);
      frontDist = ping_cm(8);
      //print("%d\n", frontDist);
      LMR[2] = frontDist;
      rePos = (LMR[2] - _MID_SPOT) * 10 / 3.25;
      drive_goto(rePos, rePos);
      ticksCounter += rePos;

      turn_pivot_function(90);
      flag = 1;
    }
  }

  // Display the path the robot took, from box to box.
  // If this is correct, Phase 1 is done.
  int arrLen = index;
  // print("Sequence robot moved: ");
  // for (int i=0; i<arrLen; i++) {
  //   print("%d ", pathSeq[i]);
  // }
  // print("\n");
  // print("Phase 1 completed, check if path sequence is correct\n");


  // Storing the path to and path back as two arrays.
  for (int i=0; i<arrLen; i++) {
    if (pathSeq[i] == 16) {
      midPoint = i;
    }
  }

  int* pathTo = (int*)malloc(sizeof(int) * (midPoint+1));
  int* pathBack = (int*)malloc(sizeof(int) * (arrLen-midPoint));

  print("Path to: ");
  for (int i=0; i<midPoint+1; i++) {
    pathTo[i] = pathSeq[i];
    print("%d ", pathTo[i]);
  }
  print("\n");

  print("Path back: ");
  for (int i=midPoint; i<arrLen; i++) {
    pathBack[i-midPoint] = pathSeq[i];
    print("%d ", pathBack[i-midPoint]);
  }
  print("\n");

  // *****Have to remove duplicate path*****

  // // Identifying duplicate index for path to.
  // int duplicateIndexTo[10];
  // int duplicateCounterTo = 0;
  // for (int i = 0; i < 10; i++) {
  //   duplicateIndexTo[i] = 0;
  // }
  // for (int i = 0; i <= midPoint; i++) {
  //   for (int j = i+1; j <= midPoint; j++) {
  //     if (pathTo[i] == pathTo[j]) {
  //       duplicateIndexTo[duplicateCounterTo] = j;
  //       duplicateIndexTo[duplicateCounterTo+1] = i;
  //       duplicateCounterTo = duplicateCounterTo + 2;
  //     }
  //   }
  // }
  // print("Duplicate To: ");
  // for (int i = 0; i < 10; i++) {
  //   print("%d ", duplicateIndexTo[i]);
  // }
  // print("\n");
  //
  // // Identifying duplicate index for path back.
  // int duplicateIndexBack[10];
  // int duplicateCounterBack = 0;
  // for (int i = 0; i < 10; i++) {
  //   duplicateIndexBack[i] = 0;
  // }
  // for (int i = 0; i < (arrLen-midPoint-1); i++) {
  //   for (int j = i+1; j < (arrLen-midPoint); j++) {
  //     if (pathTo[i] == pathTo[j]) {
  //       duplicateIndexBack[duplicateCounterBack] = j;
  //       duplicateIndexBack[duplicateCounterBack+1] = i;
  //       duplicateCounterBack = duplicateCounterBack + 2;
  //     }
  //   }
  // }
  // print("Duplicate Back: ");
  // for (int i = 0; i < 10; i++) {
  //   print("%d ", duplicateIndexBack[i]);
  // }
  // print("\n");



  // Indicate ready to start Phase 2.
  high(26);
  pause(500);
  low(26);
  pause(500);
  high(26);
  pause(500);
  low(26);
  pause(500);
  high(26);
  pause(500);
  low(26);
  pause(500);

  for (int i = 0; i<=midPoint; i++) {
    print("%d ", pathTo[i]);
  }
  print("\n");

  for (int i=midPoint; i<arrLen; i++) {
    print("%d ", pathBack[i-midPoint]);
  }
  print("\n");

  int boxNumDiff = 0;

  // Execute Phase 2
  for (int i=0; i<midPoint; i++) {
    boxNumDiff = pathTo[i+1] - pathTo[i];
    if (boxNumDiff == 4) {
      // Move one box.
      while(((ticks[0]+ticks[1])/2) < ticksCounter) {
        for (int dacVal = 0; dacVal < 160; dacVal += 8) {
          dac_ctr(26, 0, dacVal);
          freqout(11, 1, 38000);
          irLeft += input(10);                          // Left side IR-sensor

          dac_ctr(27, 1, dacVal);
          freqout(1, 1, 38000);
          irRight += input(2);                          // Right side IR-sensor
        }

        errorVal = irRight - irLeft;

        if ((prevErrorVal > 0 && errorVal <= 0) || (prevErrorVal < 0 && errorVal >= 0)) {
           totalErrorVal = 0;
        }

        errorDiff = errorVal - prevErrorVal;

        prevErrorVal = errorVal;

        irLeft = 0;
        irRight = 0;

        correctionSpd = (kp * errorVal) + (ki * totalErrorVal) + (kd * errorDiff);

        if (errorVal == 0) {
          drive_speed(baseSpd, baseSpd);
        }

        // Robot is too near to right wall.
        else if (errorVal < 0) {
          // Move left, correctionSpd is positive.
          drive_speed(baseSpd-correctionSpd, baseSpd);
        }
        // Robot is too near to left wall.
        else if (errorVal > 0) {
          // Move right, correctionSpd is negative.
          drive_speed(baseSpd, baseSpd+correctionSpd);
        }
        drive_getTicks(&ticks[0], &ticks[1]);
      }
      drive_speed(0, 0);
      ticksCounter += (int)_MOVE_UNIT;
    }
    else if (boxNumDiff == -4) {
      turn_pivot_function(180);
      // Move one box.
      while(((ticks[0]+ticks[1])/2) < ticksCounter) {
        for (int dacVal = 0; dacVal < 160; dacVal += 8) {
          dac_ctr(26, 0, dacVal);
          freqout(11, 1, 38000);
          irLeft += input(10);                          // Left side IR-sensor

          dac_ctr(27, 1, dacVal);
          freqout(1, 1, 38000);
          irRight += input(2);                          // Right side IR-sensor
        }

        errorVal = irRight - irLeft;

        if ((prevErrorVal > 0 && errorVal <= 0) || (prevErrorVal < 0 && errorVal >= 0)) {
           totalErrorVal = 0;
        }

        errorDiff = errorVal - prevErrorVal;

        prevErrorVal = errorVal;

        irLeft = 0;
        irRight = 0;

        correctionSpd = (kp * errorVal) + (ki * totalErrorVal) + (kd * errorDiff);

        if (errorVal == 0) {
          drive_speed(baseSpd, baseSpd);
        }

        // Robot is too near to right wall.
        else if (errorVal < 0) {
          // Move left, correctionSpd is positive.
          drive_speed(baseSpd-correctionSpd, baseSpd);
        }
        // Robot is too near to left wall.
        else if (errorVal > 0) {
          // Move right, correctionSpd is negative.
          drive_speed(baseSpd, baseSpd+correctionSpd);
        }
        drive_getTicks(&ticks[0], &ticks[1]);
      }
      drive_speed(0, 0);
      ticksCounter += (int)_MOVE_UNIT;
      turn_pivot_function(180);
    }
    else if (boxNumDiff == 1) {
      turn_pivot_function(90);
      // Move one box.
      while(((ticks[0]+ticks[1])/2) < ticksCounter) {
        for (int dacVal = 0; dacVal < 160; dacVal += 8) {
          dac_ctr(26, 0, dacVal);
          freqout(11, 1, 38000);
          irLeft += input(10);                          // Left side IR-sensor

          dac_ctr(27, 1, dacVal);
          freqout(1, 1, 38000);
          irRight += input(2);                          // Right side IR-sensor
        }

        errorVal = irRight - irLeft;

        if ((prevErrorVal > 0 && errorVal <= 0) || (prevErrorVal < 0 && errorVal >= 0)) {
           totalErrorVal = 0;
        }

        errorDiff = errorVal - prevErrorVal;

        prevErrorVal = errorVal;

        irLeft = 0;
        irRight = 0;

        correctionSpd = (kp * errorVal) + (ki * totalErrorVal) + (kd * errorDiff);

        if (errorVal == 0) {
          drive_speed(baseSpd, baseSpd);
        }

        // Robot is too near to right wall.
        else if (errorVal < 0) {
          // Move left, correctionSpd is positive.
          drive_speed(baseSpd-correctionSpd, baseSpd);
        }
        // Robot is too near to left wall.
        else if (errorVal > 0) {
          // Move right, correctionSpd is negative.
          drive_speed(baseSpd, baseSpd+correctionSpd);
        }
        drive_getTicks(&ticks[0], &ticks[1]);
      }
      drive_speed(0, 0);
      ticksCounter += (int)_MOVE_UNIT;
      turn_pivot_function(-90);
    }
    else if (boxNumDiff == -1) {
      turn_pivot_function(-90);
      // Move one box.
      while(((ticks[0]+ticks[1])/2) < ticksCounter) {
        for (int dacVal = 0; dacVal < 160; dacVal += 8) {
          dac_ctr(26, 0, dacVal);
          freqout(11, 1, 38000);
          irLeft += input(10);                          // Left side IR-sensor

          dac_ctr(27, 1, dacVal);
          freqout(1, 1, 38000);
          irRight += input(2);                          // Right side IR-sensor
        }

        errorVal = irRight - irLeft;

        if ((prevErrorVal > 0 && errorVal <= 0) || (prevErrorVal < 0 && errorVal >= 0)) {
           totalErrorVal = 0;
        }

        errorDiff = errorVal - prevErrorVal;

        prevErrorVal = errorVal;

        irLeft = 0;
        irRight = 0;

        correctionSpd = (kp * errorVal) + (ki * totalErrorVal) + (kd * errorDiff);

        if (errorVal == 0) {
          drive_speed(baseSpd, baseSpd);
        }

        // Robot is too near to right wall.
        else if (errorVal < 0) {
          // Move left, correctionSpd is positive.
          drive_speed(baseSpd-correctionSpd, baseSpd);
        }
        // Robot is too near to left wall.
        else if (errorVal > 0) {
          // Move right, correctionSpd is negative.
          drive_speed(baseSpd, baseSpd+correctionSpd);
        }
        drive_getTicks(&ticks[0], &ticks[1]);
      }
      drive_speed(0, 0);
      ticksCounter += (int)_MOVE_UNIT;
      turn_pivot_function(90);
    }
  }

  free(pathTo);
  free(pathBack);
  // print("Phase 2 completed.\n");
  return 0;
}
