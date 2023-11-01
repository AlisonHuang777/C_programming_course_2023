//NOTE: These codes are highly experimental to me. So they possibly contain some confusing or stupid structures.

#include <stdio.h>

#define MANUAL_INPUT 1 //for dubugging uses
#define BOARD_SIZE 8
#define TOTAL_SLOTS BOARD_SIZE * BOARD_SIZE
#define DISK_DATA_SIZE 2

typedef short unsigned int disk8x; //each row of disks uses 16 bits or 2 bytes
typedef char sint4;
typedef enum diskType{EMPTY, BLACK, WHITE} diskType;
typedef struct vec2{sint4 x, y;} vec2;

disk8x layout[BOARD_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0};

vec2 add(vec2 a, vec2 b) {return((vec2){a.x + b.x, a.y + b.y});}

vec2 constMul(vec2 v, sint4 n) {return((vec2){n * v.x, n * v.y});}

sint4 equal(vec2 a, vec2 b) {return(a.x == b.x && a.y == b.y);}

diskType getDisk(vec2 pos) {return(layout[pos.x] >> DISK_DATA_SIZE * pos.y & 0b11);}

void setDisk(vec2 pos, diskType color) { //may need optimization
    disk8x data = 0, copy = layout[pos.x];
    for(int i = 0; i < BOARD_SIZE; ++i) {
        data |= (i == pos.y)? color: copy & 0b11;
        data = data >> DISK_DATA_SIZE | data << DISK_DATA_SIZE * (BOARD_SIZE - 1);
        copy = copy >> DISK_DATA_SIZE | copy << DISK_DATA_SIZE * (BOARD_SIZE - 1);
    }
    layout[pos.x] = data;
}

vec2 checkPcb(vec2 pos, vec2 dir) { //hw4(?)
    for(int i = 1; i < BOARD_SIZE; ++i) {
        if( pos.x + i * dir.x < 0 || pos.x + i * dir.x >= BOARD_SIZE ||
            pos.y + i * dir.y < 0 || pos.y + i * dir.y >= BOARD_SIZE ||
            getDisk(add(pos, constMul(dir, i))) == getDisk(pos)) return((vec2){-1, -1});
        if (getDisk(add(pos, constMul(dir, i))) == EMPTY)
            return((i == 1)? (vec2){-1, -1}: add(pos, constMul(dir, i)));
    }
}

int main() {
    //initialize
    vec2 pcbList[2][TOTAL_SLOTS];
    sint4 listPtr[2] = {0, 0};
    for(sint4 i = 0; i < TOTAL_SLOTS; ++i) {
        pcbList[0][i] = (vec2){-1, -1};
        pcbList[1][i] = (vec2){-1, -1};
    }

    #if MANUAL_INPUT
    //input
    diskType input;
    for(sint4 x = 0; x < BOARD_SIZE; ++x) for(sint4 y = 0; y < BOARD_SIZE; ++y) {
            scanf("%d", &input);
            if(input == EMPTY) continue;
            setDisk((vec2){x, y}, input);
            }
    #endif
    
    //check for placeable slots
    #define idx getDisk(pos)-1
    for(sint4 i = 0; i < TOTAL_SLOTS; ++i) for(sint4 j = 0; j < 8; ++j) {
        vec2 pos = {i / 8, i % 8}, dir = {(j < 4)? j / 3 - 1: (j + 1) / 3 - 1, (j < 4)? j % 3 - 1: (j + 1) % 3 - 1};
        if (getDisk(pos) == EMPTY) continue;
        pcbList[idx][listPtr[idx]] = checkPcb(pos, dir);
        if(pcbList[idx][listPtr[idx]].x == -1) continue;
        ++listPtr[idx];
        for(sint4 k = 0; k < listPtr[idx] - 1; ++k)
            if(equal(pcbList[idx][k], pcbList[idx][listPtr[idx]])) {
                --listPtr[idx];
                pcbList[idx][listPtr[idx]] = (vec2){-1, -1};
                break;
            }
    }
    #undef idx

    //output
    printf(  "Black(1) disk(s) is/are placeable at: ");
    if (pcbList[0][0].x == -1) printf("(None).");
    else for(sint4 i = 0; i < listPtr[0]; ++i) {
        printf("(%hhd, %hhd)", pcbList[0][i].x, pcbList[0][i].y);
        (i == listPtr[0] - 1)? printf("."): printf(", ");
    }
    printf("\nWhite(2) disk(s) is/are placeable at: ");
    if (pcbList[1][0].x == -1) printf("(None).");
    else for(sint4 i = 0; i < listPtr[1]; ++i) {
        printf("(%hhd, %hhd)", pcbList[1][i].x, pcbList[1][i].y);
        (i == listPtr[1] - 1)? printf("."): printf(", ");
    }
}