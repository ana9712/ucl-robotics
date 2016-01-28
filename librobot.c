/*
  Created by Matthew Bell and Wayne Tsui.
*/

#include "robot.h"

int calculate_arc_length(int angle) {
    double arc_length = _WHEEL_BASE * (angle * PI / 180);
    return (arc_length / _TICK_LENGTH);
}

// In pivot turn, wheel base = 52.9
int calculate_pivot_length(int angle) {
  return (calculate_arc_length(angle)/2);
}

void turn_function(int angle) {
    int left_wheel = 0, right_wheel = 0;
    if (angle > 0) {
        right_wheel = 0;
        left_wheel = calculate_arc_length(angle);
    }
    else if (angle < 0) {
        left_wheel = 0;
        right_wheel = calculate_arc_length(angle);
    }
    drive_goto(left_wheel,right_wheel);
    return;
}

void turn_pivot_function(int angle) {
    int left_wheel = 0, right_wheel = 0;
    if (angle > 0) {
      left_wheel = calculate_pivot_length(angle);
      right_wheel = -left_wheel;
    }
    else if (angle < 0) {
      right_wheel = calculate_pivot_length(angle);
      left_wheel = -right_wheel;
    }
    drive_goto(left_wheel, right_wheel);
    return;
}
