/*
  Created by Matthew Bell and Wayne Tsui.
  This is the race. Lets win this!!!
*/

#include "simpletools.h"
#include "abdrive.h"
#include "librobot.h"
#include "ping.h"

#define _MOVE_UNIT (int)(400/3.25)
#define _FRONT_DIST 30

int main() {
  int irLeft = 0, irRight = 0;
  int frontDist = 0;
  int turn = 0;
  char direction = 'n';
  int index = 0, boxNum = -3;
  int pathSeq[40];

  low(26);
  low(27);

  for(int i=0; i<40; i++) {
    pathSeq[i] = 0;
  }

  while(1) {

    // Store array boxNum (box number the robot is in)
    pathSeq[index] = boxNum;
    index++;

    //Check for obstacles (front, left, right)
    frontDist = ping_cm(8);

    freqout(11, 1, 38000);
    irLeft = input(10);

    freqout(1, 1, 38000);
    irRight = input(2);

    // Update direction
    if (irLeft == 0) {
      // Turn left.
      turn_pivot_function(-90);

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
      drive_goto(_MOVE_UNIT, _MOVE_UNIT);
    }
    else if (frontDist > _FRONT_DIST) {
      // Move one box.
      drive_goto(_MOVE_UNIT, _MOVE_UNIT);
    }
    else if (irRight == 0) {
      // Turn right.
      turn_pivot_function(90);

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
      drive_goto(_MOVE_UNIT, _MOVE_UNIT);
    }
    else {
      // Dead end. Turn around
      turn_pivot_function(180);

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
      drive_goto(_MOVE_UNIT, _MOVE_UNIT);
    }

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
      turn_pivot_function(180);
      break;
    }
  }

  // Display the path the robot took, from box to box.
  // If this is correct, Phase 1 is done.
  int ArrLen = index;
  print("Sequence robot moved: ");
  for (int i=0; i<ArrLen; i++) {
    print("%d ", pathSeq[i]);
  }
  print("\n");

}
