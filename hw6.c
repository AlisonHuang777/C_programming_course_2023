#include <stdio.h>

#define BOARD_SIZE 8
#define TOTAL_SLOTS 64
#define DISK_DATA_SIZE 2

typedef unsigned short uint16;
typedef char sint8;
typedef enum diskType{EMPTY, BLACK, WHITE} diskType;
typedef struct pcbLoc{sint8 posX, posY, potential;} pcbLoc;

diskType getDisk(uint16 layout[BOARD_SIZE], sint8 x, sint8 y) {
    return(layout[x] >> DISK_DATA_SIZE * y & 0b11);
}

void setDisk(uint16 layout[BOARD_SIZE], sint8 x, sint8 y, diskType clr) {
    layout[x] = layout[x] & ~(0b11 << y * DISK_DATA_SIZE) | clr << y * DISK_DATA_SIZE;
}

//"functions" of function
#define x i/8
#define y i%8
#define dx ((j<4)?j/3-1:(j+1)/3-1) //direction of x
#define dy ((j<4)?j%3-1:(j+1)%3-1) //direction of y
#define xd x+k*dx                  //x displaced
#define yd y+k*dy                  //y displaced
#define pos x,y                    //position
#define dpp xd,yd                  //displaced position
#define gd getDisk
#define idx gd(layout,dpp)-1
void checkPcb(uint16 layout[BOARD_SIZE], pcbLoc pcbList[2][TOTAL_SLOTS]) {
    sint8 listPtr[2] = {0, 0};
    for(sint8 i = 0; i < TOTAL_SLOTS; ++i) {
        if (gd(layout, pos) == EMPTY) continue;
        for(sint8 j = 0; j < 8; ++j) {
            for(sint8 k = 0; k < BOARD_SIZE; ++k) {
                if (xd < 0 || xd >= BOARD_SIZE ||
                    yd < 0 || yd >= BOARD_SIZE ||
                    gd(layout, dpp) == gd(layout, pos)) break;
                if (gd(layout, dpp) == EMPTY) {
                    if(k == 1) break;
                    pcbList[idx][listPtr[idx]] = (pcbLoc){xd, yd, k - 1};
                    ++listPtr[idx];
                    for(sint8 l = 0; l < listPtr[idx]; ++l) {
                        if(pcbList[idx][l].posX == xd && pcbList[idx][l].posY == yd) {
                            pcbList[idx][l].potential += k - 1;
                            --listPtr[idx];
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
}
#undef x
#undef y
#undef dx
#undef dy
#undef xd
#undef yd
#undef pos
#undef dpp
#undef gd
#undef idx