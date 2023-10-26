//NOTE: These codes are highly experimental to me. So they possibly contain some confusing or stupid structures.

#include <stdio.h>

#define MANUAL_INPUT 0 //for dubugging uses
#define BOARD_SIZE 8
#define TOTAL_SLOTS BOARD_SIZE * BOARD_SIZE

typedef short unsigned int disk8x;
typedef enum diskType{EMPTY, BLACK, WHITE} diskType;
typedef struct vec2{char x, y;} vec2;

disk8x layout[BOARD_SIZE] = {0, 0, 0, 0b0000000110000000, 0b0000001001000000, 0, 0, 0};

vec2 add(vec2 a, vec2 b) {return((vec2){a.x + b.x, a.y + b.y});}

vec2 cstMtp(vec2 v, char n) {return((vec2){n * v.x, n * v.y});}

diskType getDisk(vec2 pos) {return(layout[pos.x] >> 2*pos.y & 0b11);}

void setDisk(vec2 pos, diskType color) {
    disk8x data = 0, copy = layout[pos.x];
    for(int i = 0; i < BOARD_SIZE; ++i) {
        data |= (i == pos.y)? color: copy & 0b11;
        data <<= 2;
        copy = copy << 2 | copy >> 14;
    }
    layout[pos.x] = data;
}

vec2 checkPlaceable(vec2 pos, vec2 dir) {
    for(int i = 1; i < BOARD_SIZE; ++i) {
        if( pos.x + i * dir.x < 0 || pos.x + i * dir.x >= BOARD_SIZE ||
            pos.y + i * dir.y < 0 || pos.y + i * dir.y >= BOARD_SIZE ||
            getDisk(add(pos, cstMtp(dir, i))) == getDisk(pos)) return((vec2){-1, -1});
        if (getDisk(add(pos, cstMtp(dir, i))) == EMPTY)
            return((i == 1)? (vec2){-1, -1}: add(pos, cstMtp(dir, i)));
    }
}

int main() {
    //initialize
    vec2 blackPcb[TOTAL_SLOTS], whitePcb[TOTAL_SLOTS];
    char blackPtr = 0, whitePtr = 0, temp;
    for(int i = 0; i < TOTAL_SLOTS; ++i) {
        blackPcb[i] = (vec2){-1, -1};
        whitePcb[i] = (vec2){-1, -1};
        }

    #if MANUAL_INPUT
    //input
    for(char x = 0; x < BOARD_SIZE; ++x)
        for(char y = 0; y < BOARD_SIZE; ++y) {
            scanf("%hhd", &temp);
            setDisk((vec2){x, y}, temp);
            }
    #endif

    //check placeable locations for each disk types
    for(char x = 0; x < BOARD_SIZE; ++x)
        for(char y = 0; y < BOARD_SIZE; ++y)
            for(char dx = -1; dx <= 1; ++dx)
                for(char dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) continue;
                    switch (getDisk((vec2){x, y})) {
                    case BLACK:
                        blackPcb[blackPtr] = checkPlaceable((vec2){x, y}, (vec2){dx, dy});
                        if(blackPcb[blackPtr].x == -1) continue;
                        ++blackPtr;
                        for(char i = 0; i < blackPtr - 1; ++i)
                            if(blackPcb[i].x == blackPcb[blackPtr].x && blackPcb[i].y == blackPcb[blackPtr].y) {
                                --blackPtr;
                                blackPcb[blackPtr] = (vec2){-1, -1};
                                break;
                            }
                        break;
                    case WHITE:
                        whitePcb[whitePtr] = checkPlaceable((vec2){x, y}, (vec2){dx, dy});
                        if(whitePcb[whitePtr].x == -1) continue;
                        ++whitePtr;
                        for(char i = 0; i < whitePtr - 1; ++i)
                            if(whitePcb[i].x == whitePcb[whitePtr].x && whitePcb[i].y == whitePcb[whitePtr].y) {
                                --whitePtr;
                                whitePcb[whitePtr] = (vec2){-1, -1};
                                break;
                            }
                        break;
                    default:
                        continue;
                    }
                }
    
    //output
    printf("Black disks placeable at: ");
    if(blackPcb[0].x == -1) printf("None");
    else for(char i = 0; i < blackPtr; ++i) {
        if (i != 0) printf(", ");
        printf("(%hhd, %hhd)", blackPcb[i].x, blackPcb[i].y);
    }
    printf("\nWhite disks placeable at: ");
    if(whitePcb[0].x == -1) printf("None");
    else for(char i = 0; i < whitePtr; ++i) {
        if (i != 0) printf(", ");
        printf("(%hhd, %hhd)", whitePcb[i].x, whitePcb[i].y);
    }
}