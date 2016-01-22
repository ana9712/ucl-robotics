/**
 * LIBROBOT
 *
 * A library for automation
 *
 * Matthew Bell and Wayne Tsui
 */

#include "simpletools.h"
#include "abdrive.h"
#include <math.h>
#include <stdint.h>

#define _WHEEL_BASE 105.8
#define _TICK_LENGTH 3.25

int calculate_arc_length(int angle);

int calculate_pivot_length(int angle);

void turn_function(int angle);

void turn_pivot_function(int angle);

