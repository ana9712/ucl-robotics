/*
  Created by Matthew Bell and Wayne Tsui.
  This programme uses the robot to order the input list of numbers using the bubble sort algorithm.
  An LED blink indicates a swap of numbers.
  The robot will move to the middle of the list and turn 90 degrees to mark a finish (list is sorted).
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "librobot.h"

  int* getIntArr() {
    int i;
    print("Enter number of elements\n");
    scanf("%i", &i);
    int *inputs = malloc(sizeof(int) * i);
    print("Enter %i integers\n", i);

    for (int n = 0; n < i; n++) {
      scanf("%i", (inputs+n));
    }
    return inputs;
  }

  // swaps two integers.
  void swap(int*a, int*b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
  }

  int* bubble_sort(int* arr, int length) {
    bool swapped;
    int n = length;
    int i;

    do {
      swapped = false;
      for (i = 1; i < n; i++) {
        // Move inbetween each number, 325mm
        drive_goto(100,100);
        if ( arr[i-1] > arr[i] ) {
          swap(&arr[i-1],&arr[i]);
          swapped=true;
          // Blink LED for 1 second to indicate manual swap (PIN 26)
          high(26);
          pause(1000);
          low(26);
        }
      }
      // Finished one cycle, return to starting position
      turn_pivot_function(180);
      drive_goto((n-1)*100, (n-1)*100);
      turn_pivot_function(180);
      n--;
    } while (swapped);
    return arr;
  }

  void results(int* arr, int length) {
    // List sorted, robot moves to middle of list and turn left.
    drive_goto((((length-1)*100)/2 + 50), (((length-1)*100)/2 + 50));
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
    results(bubble_sort(a, length), length);
    free(a);

}
