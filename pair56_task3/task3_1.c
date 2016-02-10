/*
  Created by Wayne Tsui and Matthew Bell on 10/2/2016.
  This programme solves the Towers of Hanoi game.
*/

//#include "simpletools.h"
#include <stdio.h>

  int getNumberOfDisks() {
    printf("How many disk?\n");
    while(1) {
      int n;
      scanf("%d", &n);
      if (n>0) {
        return n;
      }
      else {
        printf("You did not input a valid positive integer. Try again!\n");
        printf("How many disk?\n");
      }
    }
  }

  char getStartPeg() {
    char c;
    printf("Now, on which peg do you want the disks to start from? A? B? or C?\n");
    while(1) {
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

  void towerOfHanoi(int n, char x, char y, char z) {
    if (n == 1) {
      printf("Move %c to %c\n", x, z);
    }
    else {
      towerOfHanoi(n-1, x, z, y);
      towerOfHanoi(1, x, y, z);
      towerOfHanoi(n-1, y, x, z);
    }
  }


int main() {
  int n = getNumberOfDisks();
  char c = getStartPeg(), x, y, z;
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
  }
  towerOfHanoi(n, x, y, z);
  printf("Tower of Hanoi solved.\n");
}
