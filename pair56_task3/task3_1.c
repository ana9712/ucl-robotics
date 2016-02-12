/*
  Created by Wayne Tsui and Matthew Bell on 10/2/2016.
  This programme solves the Towers of Hanoi game.
*/

#include "simpletools.h"
#include <stdio.h>

#define _NO_OF_DISKS 3
#define _START_PEG 'A'

  int aNum = 0, bNum = 0, cNum = 0;

  void towerOfHanoi(int n, char x, char y, char z) {
    if (n == 1) {

      // Show movement of disk from one peg to another.
      printf("Move %c to %c\n", x, z);

      // Adjusting the number of disks on each of the peg after the move.
      switch (x) {
        case 'A':
          aNum--;
          if (z == 'B') {
            bNum++;
          }
          else {
            cNum++;
          }
          break;

        case 'B':
          bNum--;
          if (z == 'A') {
            aNum++;
          }
          else {
            cNum++;
          }
          break;

        case 'C':
          cNum--;
          if (z == 'B') {
            bNum++;
          }
          else {
            aNum++;
          }
          break;

        default: break;
      }
      // Display the number of disks on each peg after each move.
      printf("%d    %d    %d\n", aNum, bNum, cNum);
    }
    else {
      towerOfHanoi(n-1, x, z, y);
      towerOfHanoi(1, x, y, z);
      towerOfHanoi(n-1, y, x, z);
    }
  }


int main() {
  int n = _NO_OF_DISKS;
  char c = _START_PEG, x, y, z;

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
      aNum = n;
      break;

    case 'B':
      x = 'B';
      y = 'A';
      z = 'C';
      bNum = n;
      break;

    case 'C':
      x = 'C';
      y = 'B';
      z = 'A';
      cNum = n;
      break;
  }
  // Initial arrangement of disks on pegs.
  printf("%d    %d    %d\n", aNum, bNum, cNum);

  towerOfHanoi(n, x, y, z);
  printf("Tower of Hanoi solved.\n");
}
