/*
   Created by Matthew Bell and Wayne Tsui.
   */

#include "librobot.h"

// ROBOT MOVEMENT FUNCTIONS

int calculate_arc_length(int angle) {
    double arc_length = _WHEEL_BASE * angle * PI / 180;
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

// double* distance_wheels_travelled() {
//   int* ticksArr = (int*)malloc(2 * sizeof(int));
//   drive_getTicks(ticksArr, (ticksArr+1));
//   double* distanceWheelsTravelled = malloc(2 * sizeof(double));
//   *distanceWheelsTravelled = ((double)*ticksArr/_ENCODER_CLICKS_PER_FULL_TURN) * 2 * PI * _WHEEL_RADIUS;
//   *(distanceWheelsTravelled+1) = ((double)*(ticksArr+1)/_ENCODER_CLICKS_PER_FULL_TURN) * 2 * PI * _WHEEL_RADIUS;
//   free(ticksArr);
//   return distanceWheelsTravelled;
// }
//
// double angle_change(double* distanceWheelsTravelled) {
//   return (*distanceWheelsTravelled - *(distanceWheelsTravelled+1)) / _WHEEL_BASE;
// }
//
// double radius_middle(double* distanceWheelsTravelled, double angleChange) {
//   double radiusLeft = *distanceWheelsTravelled/angleChange;
//   double radiusRight = *(distanceWheelsTravelled+1)/angleChange;
//   return (radiusLeft + radiusRight) / 2;
// }
//
// double* position_change(double* distanceWheelsTravelled, double currentAngle) {
//   double* positionCoordinates = malloc(3 * sizeof(double));
//   double angleChange = angle_change(distanceWheelsTravelled);
//   if (angleChange == 0) {
//     *positionCoordinates = 0;
//     *(positionCoordinates + 1) = *distanceWheelsTravelled;
//   }
//   else {
//     double radiusMiddle = radius_middle(distanceWheelsTravelled, angleChange);
//     *positionCoordinates = radiusMiddle*cos(currentAngle) - radiusMiddle*cos(currentAngle+angleChange);
//     *(positionCoordinates + 1) = radiusMiddle*sin(currentAngle+angleChange) - radiusMiddle*sin(currentAngle);
//   }
//   *(positionCoordinates + 2) = angleChange; // angle in radian
//   return positionCoordinates;
// }
//
// double distance_travelled(double* positionCoordinates) {
//   // Pythagora's theorem
//   return sqrt(pow(*positionCoordinates, 2) + pow(*(positionCoordinates + 1), 2));
// }

// SD CARD FUNCTIONS
/*
   void log_write(FILE* fp, double* position_coords) {
// position_coords 0, 1, 2
fprintf(fp, "%.2f %.2f %.2f\n", position_coords[0], position_coords[1], position_coords[2] * 180/PI);
}

double *log_read(FILE* fp) {
double *coords = (double*)malloc(3 * sizeof(double));
if (fscanf(fp, "%d %d %d", coords, coords+1, coords+2)) {
return coords;
}
else {
return NULL;
}
}
*/

// LINKED LIST FUNCTIONS

// Pushing to the list.
// void push(node_correctionSpd ** head, int val) {
//     node_correctionSpd * new_node;
//     new_node = malloc(sizeof(node_correctionSpd));
//
//     new_node->val = val;
//     new_node->next = *head;
//     *head = new_node;
// }

// RACE FUNCTIONS

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
}

void shortest_path(int *l, int *a) {
    // now, remove duplicate paths. Add first occurence of
    // each number to the array and then search each time.
    int search_arr[*l]; int arr_ptr = -1; int i = (*l)-1;
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

void parallel_align_left(int pingDist, int counter) {
    // Tries to adjust robot to be parallel to the left side of the wall, calls before each box movement.
    int newPingDist = 0;
    // Adjust towards the left. Value of -8 is equivalent to drive goto -2, 2.
    turn_pivot_function(-_PARALLEL_ALIGN_ANGLE);
    // Check new ping distance.
    pause(300);
    newPingDist = ping_cm(8);
    if ((pingDist - newPingDist) > 0) {
        // Continue aligning towards the left.
        counter++;
        return parallel_align_left(newPingDist, counter);
    }
    else {
        // Turn back, not a good position.
        turn_pivot_function(_PARALLEL_ALIGN_ANGLE);
        // Try adjusting towards the right if left side is not applicable.
        if (counter == 0) {
            return parallel_align_right(pingDist);
        }
    }
    return;
}

void parallel_align_right(int pingDist) {
    // Tries to adjust robot to be parallel to the left side of the wall, calls before each box movement.
    int newPingDist = 0;
    // Adjust towards the right.
    turn_pivot_function(_PARALLEL_ALIGN_ANGLE);
    // Check new ping distance.
    pause(300);
    newPingDist = ping_cm(8);
    if ((pingDist - newPingDist) > 0) {
        return parallel_align_right(newPingDist);
    }
    else {
        // Turn back, not a good position.
        turn_pivot_function(-_PARALLEL_ALIGN_ANGLE);
    }
    return;
}

int move_shortLeftCurve() {
    int left_dist = (int)( ((float)_MOVE_UNIT - (_WHEEL_BASE/3.25)) * PI / 4 );
    int right_dist = (int)( ((float)_MOVE_UNIT + (_WHEEL_BASE/3.25)) * PI / 4 );
    float ratio = (float)left_dist / (float)right_dist;
    drive_speed((int)( 128 * ratio ),128);
    int tick_acc[2];
    drive_getTicks(&tick_acc[0], &tick_acc[1]);
    int tmp[2] = {tick_acc[0], tick_acc[1]};
    while (( tmp[0] - tick_acc[0] < left_dist) && ( tmp[1] - tick_acc[1] < right_dist)) {
        pause(10);
        drive_getTicks(&tmp[0], &tmp[1]);
    }
    drive_speed(128,128);
    return (tmp[0] + tmp[1] - tick_acc[0] - tick_acc[1]) / 2;
}

int move_shortRightCurve() {
    int right_dist = (int)( ((float)_MOVE_UNIT - (_WHEEL_BASE/3.25) - 5) * PI / 4 );
    int left_dist = (int)( ((float)_MOVE_UNIT + (_WHEEL_BASE/3.25) - 5) * PI / 4 );
    float ratio = (float)right_dist / (float)left_dist;
    drive_speed(128, (int)( 128 * ratio ));
    int tick_acc[2];
    drive_getTicks(&tick_acc[0], &tick_acc[1]);
    int tmp[2] = {tick_acc[0], tick_acc[1]};
    while (( tmp[0] - tick_acc[0] < left_dist) && ( tmp[1] - tick_acc[1] < right_dist)) {
        pause(10);
        drive_getTicks(&tmp[0], &tmp[1]);
    }
    drive_speed(128,128);
    return (tmp[0] + tmp[1] - tick_acc[0] - tick_acc[1]) / 2;
}

int move_longRightCurve() {
    int left_dist = (int) ((PI / 4) * ((float)_MOVE_UNIT / 2 + (_WHEEL_BASE/3.25) ));
    int right_dist = (int) ((PI / 4) * ((float)_MOVE_UNIT / 2 - (_WHEEL_BASE/3.25) ));
    float ratio = (float) right_dist / (float) left_dist;

    drive_speed(128, (int)(128* ratio));
    int tick_acc[2];
    drive_getTicks(&tick_acc[0], &tick_acc[1]);
    int tmp[2] = {tick_acc[0], tick_acc[1]};
    // starting ticks, used at end to calc total ticks.
    int orig[2] = {tick_acc[0], tick_acc[1]};

    // turn right
    while (( tmp[0] - tick_acc[0] < left_dist) && ( tmp[1] - tick_acc[1] < right_dist)) {
        pause(10);
        drive_getTicks(&tmp[0], &tmp[1]);
    }

    // straight line, reset tick_acc.
    drive_speed(128,128);
    tick_acc[0] = tmp[0]; tick_acc[1] = tmp[1];

    int dist = (int)(2*(sqrt(2*_MOVE_UNIT) - (2 * (_WHEEL_BASE/3.25) * sin(PI / 8)) ));
    while ((tmp[0] - tick_acc[0]) < dist) {
        pause(10);
        drive_getTicks(&tmp[0], &tmp[1]);
    }

    // now ready to turn left. left/right swapped
    drive_speed((int)(128 * ratio), 128);
    tick_acc[0] = tmp[0]; tick_acc[1] = tmp[1];
    while (( tmp[0] - tick_acc[0] < right_dist) && ( tmp[1] - tick_acc[1] < left_dist)) {
        pause(10);
        drive_getTicks(&tmp[0], &tmp[1]);
    }
    
    //done.
    return (tmp[0] + tmp[1] - orig[0] - orig[1]) / 2;
}

int count_path_corners(int *pathRace, int pathRaceLength) {
    int numberOfCorners = 0;
    for (int i = 1; i < pathRaceLength-1; i++) {
        if ((pathRace[i] - pathRace[i-1]) != (pathRace[i+1] - pathRace[i])) {
            numberOfCorners++;
        }
    }
    return numberOfCorners;
}
