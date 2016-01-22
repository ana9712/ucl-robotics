#include<math.h>
#include <stdint.h>

#define _WHEEL_BASE 105.8
#define _TICK_LENGTH 3.25

int calculate_arc_length(int angle) {
    double arc_length = _WHEEL_BASE * (angle * M_PI / 180);
    return arc_length / 3.25;
}

int calculate_pivot_length(int angle) {
    return calculate_arc_length(angle) / 2;
}

void turn_function(int angle) {
    int left_wheel, right_wheel;
    if (angle > 0) {
        right_wheel = 0;
    }
    else if (angle < 0) {
        left_wheel = 0;
    }
    return;
}
