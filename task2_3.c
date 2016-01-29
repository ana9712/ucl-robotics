/*
   Created by Matthew Bell and Wayne Tsui.
   This programme sorts a user input list of random numbers in ascending order using bubble sort algorithim. 
   */
#include <stdbool.h>
#include "simpletools.h"                     

#define _ARR_SIZE 5

int* getIntArr() {
  int i;
  printf("Enter number of elements\n");
  scanf("%i", &i);
  int *inputs = malloc(sizeof(int) * i);
  printf("Enter %i integers\n", i);

  for (int n = 0; n < i; n++) {
    scanf("%i", (inputs+n));
    printf("%i", *(inputs+n));
  }      
  return *inputs;
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


void bubble_sort(int* arr, int length) {
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
      print_array(arr,length);
    }
    n--;

  } while (swapped);
  print_array(arr,length);
}

int main() {
  int* a = getIntArr();
  bubble_sort(getIntArr(),_ARR_SIZE);
  free(a);
}

