/*
  Created by Matthew Bell and Wayne Tsui.
  This programme gets the robot to move in a 1-metre square.
*/

#include "simpletools.h"                      
#include "abdrive.h"
#include "librobot.h"

void drawSquare(int n) {
  for(int i = 0; i < 4; i++) {
  drive_goto(n/3.25,n/3.25);
  turn_pivot_function(90);
  }  
}

int main()                                    
{
  
  drawSquare(1000);
 
}
