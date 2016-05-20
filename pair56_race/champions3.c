/*
  Created by Matthew Bell and Wayne Tsui.
  This is the race. Lets win this!!!
*/

#include "simpletools.h"
#include "abdrive.h"
#include "ping.h"
#include "librobot.h"

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

int main() {
  int irLeft = 0, irRight = 0;
  int frontDist = 0;
  char direction = 'n';
  int index = 0, boxNum = -3;
  int pathSeq[40];
  int midPoint = 0;
  int ticksCounter = (int)_MOVE_UNIT;
  int errorVal, prevErrorVal, totalErrorVal = 0, errorDiff = 0;
  int kp = -1, ki = -1, kd = -1;
  int baseSpd = 80, raceSpd = 128, correctionSpd;
  int ticks[2];
  ticks[0] = 0, ticks[1] = 0;
  // Store left, middle and right ping distance (for adjustment in grid before moving)
  int LMR[3];
  LMR[0] = 0, LMR[1] = 0, LMR[2] = 0;
  int rePos = 0;
  int backAtStartPoint = 0;
  int parallelCounter = 0;

  low(26);
  low(27);

  for(int i=0; i<40; i++) {
    pathSeq[i] = 0;
  }

  while(!backAtStartPoint) {

    // Store array boxNum (box number the robot is in)
    pathSeq[index] = boxNum;
    // print("In box: %d\n", boxNum);
    index++;

    // Update direction
    // Turn left.
    turn_pivot_function(-90);
    // Check for obstacles (front, left, right)
    pause(300);
    frontDist = ping_cm(8);
    // print("%d\n", frontDist);
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

      // Parallel alignment (Only applies to left wall, as once robot is parallel to left wall it will be straight for all directions.)
      parallel_align_left(LMR[0], parallelCounter);
      parallelCounter = 0;
      pause(300);
      LMR[0] = ping_cm(8);

      // Readjust position based on left ping distance.
      rePos = (LMR[0] - _MID_SPOT) * 10 / 3.25;
      drive_goto(rePos, rePos);
      ticksCounter += rePos;

      turn_pivot_function(90);
      // Check for obstacles (front, left, right)
      pause(300);
      frontDist = ping_cm(8);
      // print("%d\n", frontDist);
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
        // Check for obstacles (front, left, right)
        pause(300);
        frontDist = ping_cm(8);
        // print("%d\n", frontDist);
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
          // Check for obstacles (front, left, right)
          pause(300);
          frontDist = ping_cm(8);
          // print("%d\n", frontDist);
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
    drive_speed(0,0);
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

      // Parallel alignment (Only applies to left wall, as once robot is parallel to left wall it will be straight for all directions.)
      pause(300);
      LMR[0] = ping_cm(8);
      parallel_align_left(LMR[0], parallelCounter);
      parallelCounter = 0;

      // Check for distance (left).
      pause(300);
      frontDist = ping_cm(8);
      // print("%d\n", frontDist);
      LMR[0] = frontDist;
      rePos = (LMR[0] - _MID_SPOT) * 10 / 3.25;
      drive_goto(rePos, rePos);
      ticksCounter += rePos;

      turn_pivot_function(90);
      // Check for distance (front).
      pause(300);
      frontDist = ping_cm(8);
      // print("%d\n", frontDist);
      LMR[1] = frontDist;
      rePos = (LMR[1] - _MID_SPOT) * 10 / 3.25;
      drive_goto(rePos, rePos);
      ticksCounter += rePos;

      turn_pivot_function(90);
      // Check for distance (right).
      pause(300);
      frontDist = ping_cm(8);
      // print("%d\n", frontDist);
      LMR[2] = frontDist;
      rePos = (LMR[2] - _MID_SPOT) * 10 / 3.25;
      drive_goto(rePos, rePos);
      ticksCounter += rePos;

      turn_pivot_function(90);
      backAtStartPoint = 1;
    }
  }

  // Storing the path to and path back as two arrays.
  int arrLen = index;
  for (int i = 0; i < arrLen; i++) {
    if (pathSeq[i] == 16) {
      midPoint = i;
    }
  }

  int pathToLength = midPoint + 1;
  int pathBackLength = arrLen-midPoint;

  int* pathTo = (int*)malloc(sizeof(int) * pathToLength);
  int* pathBack = (int*)malloc(sizeof(int) * pathBackLength);

  // print("Path to: ");
  for (int i = 0; i < pathToLength; i++) {
    pathTo[i] = pathSeq[i];
  //  print("%d ", pathTo[i]);
  }
  // print("\n");

  // print("Path back: ");
  for (int i = 0; i < pathBackLength; i++) {
    pathBack[i] = pathSeq[i+midPoint];
    // print("%d ", pathBack[i]);
  }
  // print("\n");

  // Finding shortest path.
  shortest_path(&pathToLength, pathTo);
  rev_array(&pathBackLength, pathBack);
  shortest_path(&pathBackLength, pathBack);

  // Printing values of shortened pathTo and pathBack.
  // for (int i = 0; i<pathToLength; i++) {
  //   print("%d ", pathTo[i]);
  // }
  // print("\n");
  //
  // for (int i=0; i<pathBackLength; i++) {
  //   print("%d ", pathBack[i]);
  // }
  // print("\n");

  // Choosing pathTo or pathBack as pathRace.
  int pathRaceLength = 0;
  int pathTo_NumberOfCorners = 0;
  int pathBack_NumberOfCorners = 0;

  if (pathToLength < pathBackLength) {
    pathRaceLength = pathToLength;
  }
  else if (pathBackLength < pathToLength) {
    pathRaceLength = pathBackLength;
  }
  else {
    // Same length for both, counting corners is required. Movement sequence with lesser corner turns is selected as race path.
    pathTo_NumberOfCorners = count_path_corners(pathTo, pathToLength);
    pathBack_NumberOfCorners = count_path_corners(pathBack, pathBackLength);
    if (pathTo_NumberOfCorners <= pathBack_NumberOfCorners) {
      pathRaceLength = pathToLength;
    }
    else {
      pathRaceLength = pathBackLength;
    }
  }

  int* pathRace = (int*)malloc(sizeof(int) * pathRaceLength);

  if (pathToLength < pathBackLength) {
    // Storing values of pathTo as pathRace.
    for (int i = 0; i < pathRaceLength; i++) {
      pathRace[i] = pathTo[i];
    }
  }
  else if (pathBackLength < pathToLength) {
    // Storing values of pathBack as pathRace
    for (int i = 0; i < pathRaceLength; i++) {
      pathRace[i] = pathBack[i];
    }
  }
  else {
    if (pathTo_NumberOfCorners <= pathBack_NumberOfCorners) {
      // Equal length, choose pathTo, lesser or equal corners
      for (int i = 0; i < pathRaceLength; i++) {
        pathRace[i] = pathTo[i];
      }
    }
    else {
      // Equal length, choose pathBack, lesser corners
      for (int i = 0; i < pathRaceLength; i++) {
        pathRace[i] = pathBack[i];
      }
    }
  }

  // Phase 2 Route Optimisation Variables
  int isCurving = 0;
  int boxNumDiff_Ahead = 0;
  int boxNumDiff_Behind = 0;
  // Reset ticksCounter
  drive_getTicks(&ticks[0], &ticks[1]);
  ticksCounter = (ticks[0] + ticks[1])/2;
  // Adapting sensitivity to increased robot speed
  kp = -2;
  ki = -1;
  kd = -1;
  // Free pathTo and pathBack, Phase 2 is using pathRace
  free(pathTo);
  free(pathBack);

  // Indicate ready to start Phase 2.
  for (int i = 0; i < 3; i++) {
    high(26);
    pause(500);
    low(26);
    pause(500);
  }

  // Execute Phase 2
  for (int i = 1; i < pathRaceLength-1; i++) {
    // print("Cycle: %d\n", i);

    boxNumDiff_Ahead = pathRace[i+1] - pathRace[i];
    boxNumDiff_Behind = pathRace[i] - pathRace[i-1];

    if ((abs(boxNumDiff_Behind) == 4 && abs(boxNumDiff_Ahead) == 4) ||
        (abs(boxNumDiff_Behind) == 1 && abs(boxNumDiff_Ahead) == 1)) {
      if (isCurving) {
        // print("Straight-Curving\n");
        // Add required ticks to ticksCounter
        ticksCounter += (int)_MOVE_UNIT/2;
        // move HALF standard unit
        while(((ticks[0]+ticks[1])/2) < ticksCounter) {
          for (int dacVal = 0; dacVal < 160; dacVal += 40) {
            dac_ctr(26, 0, dacVal);
            freqout(11,  1, 38000);
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
            drive_speed(raceSpd, raceSpd);
          }

          // Robot is too near to right wall.
          else if (errorVal < 0) {
            // Move left, correctionSpd is positive.
            drive_speed(raceSpd-correctionSpd, raceSpd);
          }
          // Robot is too near to left wall.
          else if (errorVal > 0) {
            // Move right, correctionSpd is negative.
            drive_speed(raceSpd, raceSpd+correctionSpd);
          }
          pause(10);
          drive_getTicks(&ticks[0], &ticks[1]);
        }
        drive_speed(raceSpd,raceSpd);
        // Robot is centered in box.
        isCurving = 0;
      }
      else {
        // print("Straight-Straight\n");
        // Add required ticks to ticksCounter
        ticksCounter += (int)_MOVE_UNIT;
        // move standard unit
        while(((ticks[0]+ticks[1])/2) < ticksCounter) {
          for (int dacVal = 0; dacVal < 160; dacVal += 40) {
            dac_ctr(26, 0, dacVal);
            freqout(11,  1, 38000);
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
            drive_speed(raceSpd, raceSpd);
          }

          // Robot is too near to right wall.
          else if (errorVal < 0) {
            // Move left, correctionSpd is positive.
            drive_speed(raceSpd-correctionSpd, raceSpd);
          }
          // Robot is too near to left wall.
          else if (errorVal > 0) {
            // Move right, correctionSpd is negative.
            drive_speed(raceSpd, raceSpd+correctionSpd);
          }
          pause(10);
          drive_getTicks(&ticks[0], &ticks[1]);
        }
        drive_speed(raceSpd,raceSpd);
      }
    }
    else if ((boxNumDiff_Behind == 4 && boxNumDiff_Ahead == 1) ||
             (boxNumDiff_Behind == -4 && boxNumDiff_Ahead == -1) ||
             (boxNumDiff_Behind == 1 && boxNumDiff_Ahead == -4) ||
             (boxNumDiff_Behind == -1 && boxNumDiff_Ahead == 4)) {
      if (isCurving) {
        // print("Curving-Right-Short\n");
        // Add required ticks to ticksCounter
        ticksCounter += move_shortRightCurve();
      }
      else {
        // print("Curving-Right-Long\n");
        // Add required ticks to ticksCounter
        ticksCounter += (int)_MOVE_UNIT/2;
        // move HALF standard unit
        while(((ticks[0]+ticks[1])/2) < ticksCounter) {
          for (int dacVal = 0; dacVal < 160; dacVal += 40) {
            dac_ctr(26, 0, dacVal);
            freqout(11,  1, 38000);
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
            drive_speed(raceSpd, raceSpd);
          }

          // Robot is too near to right wall.
          else if (errorVal < 0) {
            // Move left, correctionSpd is positive.
            drive_speed(raceSpd-correctionSpd, raceSpd);
          }
          // Robot is too near to left wall.
          else if (errorVal > 0) {
            // Move right, correctionSpd is negative.
            drive_speed(raceSpd, raceSpd+correctionSpd);
          }
          pause(10);
          drive_getTicks(&ticks[0], &ticks[1]);
        }
        drive_speed(raceSpd,raceSpd);
        // Add required ticks to ticksCounter
        ticksCounter += move_shortRightCurve();
        isCurving = 1;
      }
    }
    else if ((boxNumDiff_Behind == 4 && boxNumDiff_Ahead == -1) ||
             (boxNumDiff_Behind == -4 && boxNumDiff_Ahead == 1) ||
             (boxNumDiff_Behind == 1 && boxNumDiff_Ahead == 4) ||
             (boxNumDiff_Behind == -1 && boxNumDiff_Ahead == -4)) {
      if (isCurving) {
        // print("Curving-Left-Short\n");
        // Add required ticks to ticksCounter
        ticksCounter += move_shortLeftCurve();
      }
      else {
        // print("Curving-Left-Long\n");
        // Add required ticks to ticksCounter
        ticksCounter += (int)_MOVE_UNIT/2;
        // move HALF standard unit
        while(((ticks[0]+ticks[1])/2) < ticksCounter) {
          for (int dacVal = 0; dacVal < 160; dacVal += 40) {
            dac_ctr(26, 0, dacVal);
            freqout(11,  1, 38000);
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
            drive_speed(raceSpd, raceSpd);
          }

          // Robot is too near to right wall.
          else if (errorVal < 0) {
            // Move left, correctionSpd is positive.
            drive_speed(raceSpd-correctionSpd, raceSpd);
          }
          // Robot is too near to left wall.
          else if (errorVal > 0) {
            // Move right, correctionSpd is negative.
            drive_speed(raceSpd, raceSpd+correctionSpd);
          }
          pause(10);
          drive_getTicks(&ticks[0], &ticks[1]);
        }
        drive_speed(raceSpd,raceSpd);
        // Add required ticks to ticksCounter
        ticksCounter += move_shortLeftCurve();
        isCurving = 1;
      }
    }
  }

  // Finishing move.
  if (isCurving) {
    // Move HALF standard box to finish
    drive_goto(_MOVE_UNIT/2,_MOVE_UNIT/2);
  }
  else {
    // Move standard box to finish
    drive_goto(_MOVE_UNIT,_MOVE_UNIT);
  }
  drive_speed(0,0);

  free(pathRace);
  return 0;
}
