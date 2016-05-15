#include <stdio.h>
#include <stdlib.h>

// x search value
// a array
// l length
// RETURNS index of first occurence in array,
// else returns -1.
// *implements interpolation search for fast searching.
int int_in_arr(int x, int *a, int l) {
  int i = 0;
  while (i < l) {
    if (a[i] == x)
      return i;
    i++;
  }
  return -1;
}

// Deletes this element from array.
// a     - array
// index - index of element to delete
// l     - length of array
void el_del(int *a, int index, int l) {
  for (int i = index; i < l-1; i++) {
    a[i] = a[i+1];
  }
}

// l length
// a array
void rev_array(int *l, int *a) {
  // Why is length a pointer? Well, we're going to
  // use side effects to modify it so that we can
  // return the new length as well as the array.
  //
  // The variables passed to this function *will*
  // be modified.

  // swapping elements from the outside going
  // inwards.
  int i = 0; int j = *l - 1;
  while (i < j) {
    int tmp = a[i];
    a[i++] = a[j];
    a[j--] = tmp;
  }

  // now, remove duplicate paths. Add first occurence of
  // each number to the array and then search each time.
  int search_arr[*l]; int arr_ptr = -1; i = (*l)-1;
  while (i >= 0) {
    if (int_in_arr(a[i], search_arr, arr_ptr+1) >= 0) {
      // you dun messed up. remove path.
      // remove up to other occurence of a[i].
      int j = i+1; int x;
      do {
        x = a[j];
        el_del(a, j, *l);
        (*l)--;
      } while (x != a[i]);
      i--;
    }
    else {
      search_arr[++arr_ptr] = a[i--];
    }
  }
}

// for testing purposes
int main(int argc, char **argv) {
  int arr[argc--];
  for (int i = 0; i < argc; i++) {
    // yeah I use atoi.
    arr[i] = atoi(argv[i+1]);
  }

  int length = argc;

  rev_array(&length, arr);

  printf("The new length is %d\n", length);

  printf("Array: [");
  for (int i = 0; i < length-1; i++) {
    printf("%d, ", arr[i]);
  }
  printf("%d]\n", arr[length-1]);

  return 0;
}
