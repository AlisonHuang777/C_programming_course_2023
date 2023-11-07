#include <stdio.h>

#define BOARD_SIZE 8
#define TOTAL_SLOTS BOARD_SIZE*BOARD_SIZE
#define DIRECTIONS 8
#define DISK_TYPES 2

typedef unsigned short uint16;
typedef unsigned char uint8;
typedef enum diskType{EMPTY, BLACK, WHITE} diskType;
typedef struct pcbLoc{uint8 posX, posY, potential, dirFlag;} pcbLoc;

#define dbs 2 //disk bit size: the least bit size required to represent all the possible states of a disk
diskType getDisk(uint16 bd[BOARD_SIZE], uint8 x, uint8 y) {return(bd[x] >> dbs * y & 3);}
void setDisk(uint16 bd[BOARD_SIZE], uint8 x, uint8 y, diskType clr) {bd[x] = bd[x] & ~(3 << y * dbs) | clr << y * dbs;}
#undef dbs

#define ip i/8                        //index of position
#define id i%8                        //index of direction
#define x ip/8                        //x position
#define y ip%8                        //y position
#define dx ((id<4)?id/3-1:(id+1)/3-1) //x direction
#define dy ((id<4)?id%3-1:(id+1)%3-1) //y direction
#define xd x+s*dx                     //x displaced
#define yd y+s*dy                     //y displaced
#define pos x,y                       //position
#define dpp xd,yd                     //displaced position
#define gd getDisk                    //abbreviated name for getDisk()
#define idx gd(layout,dpp)-1          //convert diskType index to pcbList index
#define pot s-1                       //potential: the amount of flippable disks
void checkPcb(uint16 layout[BOARD_SIZE], pcbLoc pcbList[DISK_TYPES][TOTAL_SLOTS]) {
    for(uint16 i = 0; i < TOTAL_SLOTS * DIRECTIONS; ++i) {
        if (gd(layout, pos) != EMPTY) continue;
        for(uint8 s = 1; s < BOARD_SIZE; ++s) {
            if (xd < 0 || xd >= BOARD_SIZE ||
                yd < 0 || yd >= BOARD_SIZE ||
                gd(layout, dpp) == EMPTY) break;
            if (gd(layout, dpp) != gd(layout, x + dx, y + dy)) {
                for(uint8 i_ = 0; i_ < TOTAL_SLOTS; ++i_) {
                    if(pcbList[idx][i_].posX == x && pcbList[idx][i_].posY == y) {
                        pcbList[idx][i_].potential += pot;
                        pcbList[idx][i_].dirFlag |= 1 << id;
                        break;
                    }
                    else if(pcbList[idx][i_].potential == 0) {
                        pcbList[idx][i_] = (pcbLoc){pos, pot, 1 << id};
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
#undef gd
#undef idx
#undef pot
#define x selected.posX
#define y selected.posY
#define dx ((d<4)?d/3-1:(d+1)/3-1)
#define dy ((d<4)?d%3-1:(d+1)%3-1)
#define clr selected.potential //color
void update(uint16 layout[BOARD_SIZE], pcbLoc selected) {
    setDisk(layout, pos, clr);
    for(uint8 d = 0; d < DIRECTIONS; ++d) {
        if ((selected.dirFlag >> d & 1) == 0) continue;
        for(uint8 s = 1; s < BOARD_SIZE; ++s) {
            if(getDisk(layout, dpp) == clr) break;
            setDisk(layout, dpp, clr);
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

int main() {
    uint16 board[8] = {};
    pcbLoc choices[2][TOTAL_SLOTS] = {};
    uint8 x, y, c;
    while(scanf("%hhu %hhu %hhu", &x, &y, &c) != EOF) {
        setDisk(board, x, y, c);
        for(int i = 0; i < BOARD_SIZE; ++i) {
            for(int j = 0; j < BOARD_SIZE; ++j) {
                if(j != 0) printf(", ");
                printf("%d", getDisk(board, i, j));
            }
            printf("\n");
        }
        checkPcb(board, choices);
        for(int i = 0; i < DISK_TYPES; ++i) {
            printf("%d: ", i + 1);
            for(int j = 0; j < TOTAL_SLOTS; ++j) {
                if(choices[i][j].potential == 0) break;
                if(j != 0) printf(", ");
                printf("(%hhu, %hhu: %hhu, ",choices[i][j].posX, choices[i][j].posY, choices[i][j].potential);
                for(int k = 0; k < DIRECTIONS; ++k) printf("%d", choices[i][j].dirFlag >> 7 - k & 1);
                printf(")");
            }
            printf("\n");
        }
        scanf("%hhu %hhu", &x, &y);
        if(x != 0) {
            choices[x - 1][y].potential = x;
            update(board, choices[x - 1][y]);
        }
        for(int i = 0; i < 2 * TOTAL_SLOTS; ++i) choices[i % 2][i / 2] = (pcbLoc){0, 0, 0, 0};
        for(int i = 0; i < BOARD_SIZE; ++i) {
            for(int j = 0; j < BOARD_SIZE; ++j) {
                if(j != 0) printf(", ");
                printf("%d", getDisk(board, i, j));
            }
            printf("\n");
        }
        printf(""); //temp bp anchor
    }
}