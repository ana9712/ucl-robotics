/*
  Created by Matthew Bell and Wayne Tsui.
  This programme sorts a user input list of random numbers in ascending order using bubble sort algorithim.
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "simpletools.h"

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

  // swaps two integers.
  void swap(int*a, int*b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
  }

  // prints the given array to stdout.
  void print_array(int *arr, int length) {
    int i;
    for (i = 0; i < length; i++) {
      printf("%i ",arr[i]);
    }
    printf("\n");
  }

  int* bubble_sort(int* arr, int length) {
    bool swapped;
    int n = length;
    int i;
    do {
      swapped = false;
      for (i = 1; i < n; i++) {
        if ( arr[i-1] > arr[i] ) {
          swap(&arr[i-1],&arr[i]);
          swapped=true;
        }
      }
      n--;

    } while (swapped);
    return arr;
  }

  void results(int* arr, int length) {
    printf("Sorted list in ascending order: \n");
    print_array(arr, length);
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
