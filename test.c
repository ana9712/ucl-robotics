#include <stdio.h>

#define _START_PEG 'C'
#define _NO_OF_DISKS 4

char pos = _START_PEG;

void move_disc(char source, char dest) {
    char dist = source - pos;
    printf("Moving %d units to %c\n",(int)dist,source);
    printf("Pickup a disk\n");
    dist = dest - source;
    printf("Moving %d units to %c\n",(int)dist,dest);
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

    return 0;
}
