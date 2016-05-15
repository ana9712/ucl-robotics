/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"                      // Include simple tools
#include "abdrive.h"

int main()                                    // Main function
{
  
  pause(500);
  drive_speed(97,128);
  pause(1800);
  drive_speed(0,0);

  pause(500);
  drive_speed(128,97);
  pause(1800);
  drive_speed(0,0);
   
}
