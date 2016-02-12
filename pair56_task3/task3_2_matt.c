#include <stdio.h>
#include "simpletools.h"
#include "abdrive.h"
#include "librobot.h"

#define _START_PEG 'C'
#define _NO_OF_DISKS 4

const int ONE_UNIT = 130;

char pos = _START_PEG;

void signalStart() {
  high(26);
  pause(2000);
  low(26);
}

// Turn left to face in then turn back right.
void signalEnd() {
  turn_pivot_function(-90);
  turn_pivot_function(90);
}

void move_disc(char source, char dest) {
    char dist = source - pos;
    printf("Moving %d units to %c\n",(int)dist,source);
    drive_goto(ONE_UNIT*dist,ONE_UNIT*pos);
    signalStart();
    dist = dest - source;
    printf("Moving %d units to %c\n",(int)dist,dest);
    drive_goto(ONE_UNIT*dist,ONE_UNIT*pos);
    signalEnd();
    pos = dest;
}

void hanoi(int n, char source, char via, char dest) {
    if (n > 0) {
        hanoi(n-1,source, dest, via);
        printf("Current position: %c\n",pos);
        move_disc(source,dest);
        hanoi(n-1, via, source, dest);
    }
}

int main() {
    char via = 'B';
    char dest = 'C';
    switch (_START_PEG) {
        case 'A':
            via = 'B';
            dest = 'C';
            break;
        case 'B':
            via = 'A';
            dest = 'C';
            break;
        case 'C':
            via = 'B';
            dest = 'A';
        break;
    }
    printf("Start point: %c\n",_START_PEG);
    printf("Via point: %c\n",via);
    printf("End point: %c\n",dest);
    hanoi(_NO_OF_DISKS,_START_PEG,via,dest);
    turn_pivot_function(90);
    return 0;
}
