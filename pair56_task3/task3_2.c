/*
Created by Wayne Tsui and Matthew Bell on 10/2/2016.
This programme solves the Towers of Hanoi game using a robot to signal a move of a disk from one peg to another and also to signal the end of the algorithm.
*/

#include <stdio.h>
#include <stdbool.h>
//#include "librobot.h"

#define oneUnit 65; // 210mm, width of A4-size paper
#define twoUnit 130; // 420mm, width of 2 A4-size paper

bool firstTime = true;

int getNumberOfDisks() {
  printf("How many disk?\n");
  while(1) {
    int n;
    scanf("%i", &n);
    if (n>0) {
      return n;
    }
    else {
      printf("You did not input a valid positive integer. Try again\n");
      printf("How many disk?");
    }
  }
}

char getStartPeg() {
  printf("Now, on which peg do you want the disks to start from? A? B? or C?");
  while(1) {
    char c;
    scanf("%c", &c);
    if (c == 'A' || c == 'B' || c =='C') {
      return c;
    }
    else {
      printf("You did not input a valid character. Input A, B or C.\n");
      printf("Which peg do you want the disk to start from?\n");
    }
  }
}
/*
  // On LED for two seconds
  void signalStart() {
    high(26);
    pause(2000);
    low(26);
  }

  // Blink LED for two seconds
  void signalEnd() {
    int n = 40;
    while(n > 0) {
      high(26);
      pause(50);
      low(26);
      n--;
    }
  }
*/

void towerOfHanoi(int n, char x, char y, char z, char t) {

  // Need to move disk
  if (n == 1) {
    // Robot start at peg that has stacked disks.
    if (!firstTime) {
      // Move from previous Z (previous End peg) to current X (Start peg).
      switch(t) {

        case 'A':
          if (x == 'B') {
            //drive_goto(oneUnit, oneUnit);
            printf("Returned to %c\n", x);
          }
          if (x == 'C') {
            //drive_goto(twoUnit, twoUnit);
            printf("Returned to %c\n", x);
          }
          break;

        case 'B':
          if (x == 'C') {
            //drive_goto(oneUnit, oneUnit);
            printf("Returned to %c\n", x);
          }
          if (x == 'A') {
            //drive_goto(-oneUnit, -oneUnit);
            printf("Returned to %c\n", x);
          }
          break;

        case 'C':
          if (x == 'B') {
            //drive_goto(-oneUnit, -oneUnit);
            printf("Returned to %c\n", x);
          }
          if (x == 'A'){
            //drive_goto(-twoUnit, -twoUnit);
            printf("Returned to %c\n", x);
          }

        default: break;
      }
    }

    firstTime = false;

    // Move from current X (Start peg) to current Z (End peg).
    switch (x) {

      case 'A':
        //signalStart();
        if (z == 'B') {
          //drive_goto(oneUnit, oneUnit);
          //signalEnd();
          printf("Move %c to %c\n", x, z);
        }
        else {
          //drive_goto(twoUnit, twoUnit);
          //signalEnd();
          printf("Move %c to %c\n", x, z);
        }
        break;

      case 'B':
        if (z == 'C') {
          //drive_goto(oneUnit, oneUnit);
          //signalEnd();
          printf("Move %c to %c\n", x, z);
        }
        else {
          //drive_goto(-oneUnit, -oneUnit);
          //signalEnd();
          printf("Move %c to %c\n", x, z);
        }
        break;

      case 'C':
        if (z == 'B') {
          //drive_goto(-oneUnit, -oneUnit);
          //signalEnd();
          printf("Move %c to %c\n", x, z);
        }
        else {
          //drive_goto(-twoUnit, -twoUnit);
          //signalEnd();
          printf("Move %c to %c\n", x, z);
        }

      default: break;
    }
  }
  else {
    towerOfHanoi(n-1, x, z, y, y);
    towerOfHanoi(1, x, y, z, z);
    towerOfHanoi(n-1, y, x, z, z);
  }
}


int main() {
int n = getNumberOfDisks();
char c = getStartPeg(), x, y, z, t;

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
towerOfHanoi(n, x, y, z, t);
// Turn left to signal end of algorithm.
//turn_pivot_function(-90);
printf("Tower of Hanoi solved.\n");
}
