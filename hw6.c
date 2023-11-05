#include <stdio.h>

#define BOARD_SIZE 8
#define TOTAL_SLOTS 64

typedef unsigned short uint16;
typedef char sint8;
typedef enum diskType{EMPTY, BLACK, WHITE} diskType;
typedef struct pcbLoc{sint8 posX, posY, potential;} pcbLoc;

#define dbs 2 //disk bit size: the least bit size required to represent all the possible states of a disk
diskType getDisk(uint16 bd[BOARD_SIZE], sint8 x, sint8 y) {return(bd[x] >> dbs * y & 3);}
void setDisk(uint16 bd[BOARD_SIZE], sint8 x, sint8 y, diskType clr) {bd[x] = bd[x] & ~(3 << y * dbs) | clr << y * dbs;}
#undef dbs

#define ip i/8                      //index of position
#define id i%8                      //index of direction
#define x ip/8                      //x position
#define y ip%8                      //y position
#define dx (id<4)?id/3-1:(id+1)/3-1 //x direction
#define dy (id<4)?id%3-1:(id+1)%3-1 //y direction
#define xd x+s*(dx)                 //x displaced
#define yd y+s*(dy)                 //y displaced
#define pos x,y                     //position
#define dpp xd,yd                   //displaced position
#define gd getDisk                  //abbreviated name for getDisk()
#define idx gd(layout,dpp)-1        //convert diskType index to pcbList index
#define p s-1                       //potential: the amount of flippable disks
void checkPcb(uint16 layout[BOARD_SIZE], pcbLoc pcbList[2][TOTAL_SLOTS]) {
    for(sint8 i = 0; i < TOTAL_SLOTS * 8; ++i) {
        if (gd(layout, pos) == EMPTY) continue;
        for(sint8 s = 0; s < BOARD_SIZE; ++s) {
            if (xd < 0 || xd >= BOARD_SIZE ||
                yd < 0 || yd >= BOARD_SIZE ||
                gd(layout, dpp) == gd(layout, pos)) break;
            if (gd(layout, dpp) == EMPTY) {
                if(s == 1) break;
                for(sint8 i_ = 0; i_ < TOTAL_SLOTS; ++i_) {
                    if(pcbList[idx][i_].posX == xd && pcbList[idx][i_].posY == yd) {
                        pcbList[idx][i_].potential += p;
                        break;
                    }
                    else if(pcbList[idx][i_].potential == 0) {
                        pcbList[idx][i_] = (pcbLoc){dpp, p};
                        break;
                    }
                }
                break;
            }
        }
    }
}
#undef ip
#undef id
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
#undef p