/*
  Created by Wayne Tsui and Matthew Bell on 10/2/2016.
  This programme solves the Towers of Hanoi game.
*/

#include "simpletools.h"
#include <stdio.h>

  int aNum = 0, bNum = 0, cNum = 0;

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
  int n = getNumberOfDisks();
  char c = getStartPeg(), x, y, z;


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
