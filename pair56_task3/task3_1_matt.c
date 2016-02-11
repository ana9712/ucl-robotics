/*
   Created by Matthew Bell and Wayne Tsui.
   Completes the Tower Of Hanoi puzzle, given a number of discs used and the peg on which they start.
   */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define _NO_OF_DISCS 3
#define _START_PEG 'A'

// which pegs we will use.
char init_peg = 0;
char mid_peg = 1;
char fin_peg = 2;

// the array of pegs. each peg is an array of length n. First in array is the 'bottom' of the peg, for simplicity. Bigger value = bigger disc.
char pegs[3][_NO_OF_DISCS];
char peg_ptr[3] = { -1,-1,-1 };

/***
 * This function determines the values of init_peg (starting peg), mid_peg (intermediate peg) and fin_peg (the goal peg).
 ***/
void determine_pegs() {
  switch (_START_PEG) {
    case 'A': break;
    case 'B': init_peg = 1;
              mid_peg = 0;
              fin_peg = 2;
              break;
    case 'C': init_peg = 2;
              mid_peg = 1;
              fin_peg = 0;
              break;
    default: break;
  }
}

/***
 * populate the pegs with starting values, as well as their 'stack' pointers.
 ***/
void populate_pegs() {

  int i = 0; // C99? Pfft, not likely.
  for (i = 0; i < 3; i++) {
    if (i == init_peg) {
      int j = 1;
      for (j = 0; j < _NO_OF_DISCS; j++) {
        pegs[i][j] = _NO_OF_DISCS - j;
      }
      peg_ptr[i] = _NO_OF_DISCS-1;
    }
    else {
      int j = 0;
      for (j = 0; j < _NO_OF_DISCS; j++) {
        pegs[i][j] = 0;
      }
      peg_ptr[i] = -1;
    }
  }
}

void display_stacks() {
  printf("%d\t%d\t%d\t\n",peg_ptr[0]+1,peg_ptr[1]+1,peg_ptr[2]+1);
}

// moves a disc from source to dest using neat stack pointers.
void move_disc(char source, char dest) {
  char tmp_disc = pegs[source][peg_ptr[source]--];
  pegs[dest][++peg_ptr[dest]] = tmp_disc;
}

void move(char n, char init, char mid, char fin) {
  if (n > 0) {
    move(n-1, init, fin, mid);
    // move disc from init to fin.
    move_disc(init,fin);
    display_stacks();
    move(n-1, mid, init, fin);
  }
}

int main() {
  determine_pegs();
  populate_pegs();
  move(_NO_OF_DISCS,init_peg,mid_peg,fin_peg);
}
