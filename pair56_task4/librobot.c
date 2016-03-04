/*
  Created by Matthew Bell and Wayne Tsui.
*/

#include "librobot.h"

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
        angle = -angle;
        left_wheel = 0;
        right_wheel = calculate_arc_length(angle);
    }
    drive_goto(left_wheel,right_wheel);
    return;
}

void turn_pivot_function(int angle) {
    int left_wheel = 0, right_wheel = 0;

      left_wheel = calculate_pivot_length(angle);
      right_wheel = -left_wheel;

    drive_goto(left_wheel, right_wheel);
    return;
}

double* distance_wheels_travelled() {
  int* ticksArr = malloc(2 * sizeof(int));
  drive_getTicks(*ticksArr, *(ticksArr+1));
  double* distanceWheelsTravelled = malloc(2 * sizeof(double));
  *distanceWheelsTravelled = (*ticksArr/_ENCODER_CLICKS_PER_FULL_TURN) * 2 * PI * _WHEEL_RADIUS;
  *(distanceWheelsTravelled+1) = (*(ticksArr+1)/_ENCODER_CLICKS_PER_FULL_TURN) * 2 * PI * _WHEEL_RADIUS;
  free(ticksArr);
  return distanceWheelsTravelled;
}

double angle_change(double* distanceWheelsTravelled) {
  return (*distanceWheelsTravelled - *(distanceWheelsTravelled+1)) / _WHEEL_BASE;
}

double radius_middle(double* distanceWheelsTravelled) {
  double angleChange = angle_change(distanceWheelsTravelled);
  double radiusLeft = *distanceWheelsTravelled/angleChange;
  double radiusRight = *(distanceWheelsTravelled+1)/angleChange;
  return (radiusLeft + radiusRight) / 2;
}

double* position_change(double* distanceWheelsTravelled, double currentAngle) {
  double* positionCoordinates = malloc(3 * sizeof(double));
  double radiusMiddle = radius_middle(distanceWheelsTravelled);
  double angleChange = angle_change(distanceWheelsTravelled);
  *positionCoordinates = radiusMiddle*cos(currentAngle) - radiusMiddle*cos(currentAngle+angleChange);
  *(positionCoordinates + 1) = radiusMiddle*sin(currentAngle+angleChange) - radiusMiddle*sin(currentAngle);
  *(positionCoordinates + 2) = angleChange;
  return positionCoordinates;
}

double distance_travelled(double* positionCoordinates) {
  // Pythagora's theorem
  return sqrt(pow(*positionCoordinates, 2) + pow(*(positionCoordinates + 1), 2));
}

void log_write(FILE* fp, double* position_coords) {
    fprintf(fp, "Position: (%f, %f). Angle: %f", position_coords[0], position_coords[1], position_coords[2]);
}
