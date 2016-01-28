/*
  Created by Matthew Bell and Wayne Tsui.
  This programme gets the robot to go in a straight line and then stop.
*/

#include "simpletools.h"                     
#include "abdrive.h"

void straightLine(int n) {
  drive_goto(n/3.25,n/3.25);
}

int main()
{
  straightLine(1000);
}
