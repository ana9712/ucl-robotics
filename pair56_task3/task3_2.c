/*
Created by Wayne Tsui and Matthew Bell on 10/2/2016.
This programme solves the Towers of Hanoi game using a robot to signal a move of a disk from one peg to another and also to signal the end of the algorithm.
*/

#include "simpletools.h"
#include "abdrive.h"
#include <stdio.h>
#include "librobot.h"

#define NO_OF_DISKS 3
#define START_PEG 'A'

const int ONE_UNIT = 130; // 210mm, width of 2 A4-size paper
const int TWO_UNIT = 260; // 420mm, width of 4 A4-size paper
const int THREE_UNIT = 390; // 630mm, width of 6 A4-size paper

// On LED for two seconds
void signalStart() {
  high(26);
  pause(2000);
  low(26);
}

// Turn left to face in then turn back right.
void signalEnd() {
  turn_pivot_function(-90);
  turn_pivot_function(90);
}

void towerOfHanoi(int n, char x, char y, char z) {

  // Need to move disk.
  if (n == 1) {

    // Move from Start Point to Start Peg.
    switch (x) {
      case 'A':
        drive_goto(ONE_UNIT, ONE_UNIT);
        signalStart();
        break;

      case 'B':
        drive_goto(TWO_UNIT, TWO_UNIT);
        signalStart();
        break;

      case 'C':
        drive_goto(THREE_UNIT, THREE_UNIT);
        signalStart();
        break;

      default: break;
    }

    // Move from Start Peg to End Peg and move back to Start Point.
    switch (z) {
      case 'A':
        if (x == 'C') {
          drive_goto(-TWO_UNIT,-TWO_UNIT);
          signalEnd();
          drive_goto(-ONE_UNIT, -ONE_UNIT);
        }
        else if (x == 'B') {
          drive_goto(-ONE_UNIT, -ONE_UNIT);
          signalEnd();
          drive_goto(-ONE_UNIT, -ONE_UNIT);
        }
        break;

      case 'B':
        if (x == 'A') {
          drive_goto(ONE_UNIT, ONE_UNIT);
          signalEnd();
          drive_goto(-TWO_UNIT, -TWO_UNIT);
        }
        else if (x == 'C') {
          drive_goto(-ONE_UNIT, -ONE_UNIT);
          signalEnd();
          drive_goto(-TWO_UNIT, -TWO_UNIT);
        }
        break;

      case 'C':
        if (x == 'B') {
          drive_goto(ONE_UNIT, ONE_UNIT);
          signalEnd();
          drive_goto(-THREE_UNIT, -THREE_UNIT);
        }
        else if (x == 'A') {
          drive_goto(TWO_UNIT, TWO_UNIT);
          signalEnd();
          drive_goto(-THREE_UNIT, -THREE_UNIT);
        }
        break;

      default: break;
    }
  }
  else {
    towerOfHanoi(n-1, x, z, y);
    towerOfHanoi(1, x, y, z);
    towerOfHanoi(n-1, y, x, z);
  }
}


int main() {
int n = NO_OF_DISKS;
char c = START_PEG, x, y, z;

/*  Sort pegs
    Start at A, end at C.
    Start at B, end at C.
    Start at C, end at A.
*/

switch (c) {
  case 'A':
    x = 'A';
    y = 'B';
    z = 'C';
    break;

  case 'B':
    x = 'B';
    y = 'A';
    z = 'C';
    break;

  case 'C':
    x = 'C';
    y = 'B';
    z = 'A';
    break;

  default: break;
}
towerOfHanoi(n, x, y, z);

// Turn right, signal end of algorithm.
turn_pivot_function(90);
}
