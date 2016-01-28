/*
  Created by Matthew Bell and Wayne Tsui.
  This programme sorts a user input list of random numbers in ascending order using bubble sort algorithim. 
*/

#include "simpletools.h"                     

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
  
  void bubbleSort(int* g) {
    int temp;
    
    for (int i = 0; i < 5; i++) {
      if (*(g+i) > *(g+i+1)) {
        temp = *(g+i+1);
        *(g+i+1) = *(g+i);
        *(g+i) = temp;
      }        
    }
    
    for (int n = 0; n < 5; n++) {
      printf("%i\n", *(g+n));
    }            
  }       

int main() {
    //int* a = getIntArr();
    //bubbleSort(getIntArr());
    //free(a);
                    
}
  
