/*
  Created by Matthew Bell and Wayne Tsui.
  This programme uses the robot to order the input list of numbers using the bubble sort algorithm.
  An LED blink indicates a swap of numbers.
  The robot will move to the middle of the list and turn 90 degrees to mark a finish (list is sorted).
*/

#include <stdio.h>
#include <stdlib.h>
#include "simpletools.h"
#include "librobot.h"

  int* getIntArr() {
    int i;
    printf("Enter number of elements\n");
    scanf("%i", &i);
    int *inputs = malloc(sizeof(int) * i);
    printf("Enter %i integers\n", i);

    for (int n = 0; n < i; n++) {
      scanf("%i", (inputs+n));
    }
    return inputs;
  }

  int* bubbleSort(int* g, int len) {
    int temp;
    int counter = 0;

    for (int n = 0; n < len; n++) {

      for (int j = len-1; j > 0; j--) {
        if (*(g+j) > *(g+j-1)) {
          counter++;
        }
      }

      if (counter == len-1) {
        return g;
      }
      else
        counter = 0;

      for (int i = 0; i < len-1; i++) {
        //For every comparison
        drive_goto(325,325);

        if (*(g+i) > *(g+i+1)) {

          /*
            INPUT CODE FOR LED BLINK
          */

          temp = *(g+i+1);
          *(g+i+1) = *(g+i);
          *(g+i) = temp;
        }
      }
      //End of each cycle, return to starting position.
      turn_pivot_function(180);
      drive_goto((len-1)*325, (len-1)*325);
      turn_pivot_function(180);
    }
    return g;
  }

  void results(int* g, int len) {
    printf("Sorted list in ascending order: \n");
    for (int n = 0; n < len; n++) {
      printf("%i ", *(g+n));
    }
    printf("\n");

    //Input is sorted, go to middle of list and turn 90 degrees.
    drive_goto(((len-1)*325)/2, ((len-1)*325)/2);
    turn_pivot_function(-90);
  }

  int arrLen(int* g) {
    int length = 0;
    int* h = g;
    while(1) {
      if (*h != '\0') {
        length++;
      }
      else {
        break;
      }
      h++;
    }
    return length;
  }

int main() {

    int* a = getIntArr();
    int length = arrLen(a);
    //results(bubbleSort(a, length), length);
    free(a);

}
