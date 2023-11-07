#include <stdio.h>

#define BOARD_SIZE 8
#define TOTAL_SLOTS BOARD_SIZE*BOARD_SIZE
#define DIRECTIONS 8
#define DISK_TYPES 2

typedef unsigned short uint16;
typedef unsigned char uint8;
typedef enum diskType{EMPTY, BLACK, WHITE} diskType;
typedef struct placeableLocation{uint8 posX, posY, potential, dirFlag;} pcbLoc;

#define dbs 2 //disk bit size: the least bit size required to represent all the possible states of a disk
diskType getDisk(uint16 bd[BOARD_SIZE], uint8 x, uint8 y) {return(bd[x] >> dbs * y & 3);}
void setDisk(uint16 bd[BOARD_SIZE], uint8 x, uint8 y, diskType clr) {bd[x] = bd[x] & ~(3 << y * dbs) | clr << y * dbs;}
#undef dbs

#define ip i/DIRECTIONS               //index of position
#define id i%DIRECTIONS               //index of direction
#define x ip/BOARD_SIZE               //x position
#define y ip%BOARD_SIZE               //y position
#define dx ((id<4)?id/3-1:(id+1)/3-1) //x direction
#define dy ((id<4)?id%3-1:(id+1)%3-1) //y direction
#define xd x+s*dx                     //x displaced
#define yd y+s*dy                     //y displaced
#define pos x,y                       //position
#define dpp xd,yd                     //displaced position
#define gd getDisk                    //abbreviated name for getDisk()
#define pot s-1                       //potential: the amount of flippable disks
void checkPcb(uint16 layout[BOARD_SIZE], pcbLoc pcbList[TOTAL_SLOTS], diskType color) {
    for(uint16 i = 0; i < TOTAL_SLOTS * DIRECTIONS; ++i) {
        if (gd(layout, pos) != EMPTY) {
            i += DIRECTIONS - 1;
            continue;
        }
        for(uint8 s = 1; s < BOARD_SIZE; ++s) {
            if (xd < 0 || xd >= BOARD_SIZE ||
                yd < 0 || yd >= BOARD_SIZE ||
                gd(layout, dpp) == EMPTY) break;
            if (gd(layout, dpp) == color) {
                for(uint8 i_ = 0; i_ < TOTAL_SLOTS; ++i_) {
                    if(pcbList[i_].posX == x && pcbList[i_].posY == y) {
                        pcbList[i_].potential += pot;
                        pcbList[i_].dirFlag |= 1 << id;
                        break;
                    }
                    else if(pcbList[i_].potential == 0) {
                        pcbList[i_] = (pcbLoc){pos, pot, 1 << id};
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
    //initialize
    uint16 board[BOARD_SIZE] = {0, 0, 0, 576, 384, 0, 0, 0};
    uint8 choice;
    pcbLoc choices[TOTAL_SLOTS] = {};
    diskType curPlayer = BLACK;

    //main loop
    for(;;) {
        checkPcb(board, choices, curPlayer);

        //print board
        #define x i/BOARD_SIZE
        #define y i%BOARD_SIZE
        printf(" y 0 1 2 3 4 5 6 7\nx\\________________\n");
        for(uint8 i = 0; i < TOTAL_SLOTS; ++i) {
            if(y == 0) printf("%hhu|", x);
            for(uint8 i_ = 0; i_ < TOTAL_SLOTS; ++i_) {
                if(choices[i_].potential == 0) break;
                if(choices[i_].posX == x && choices[i_].posY == y) {
                    printf("%02hhu", i_);
                    goto skipDefaultRendering; //forgive me
                }
            }
            switch (getDisk(board, x, y)){
                case EMPTY: printf("  "); break;
                case BLACK: printf("><"); break;
                case WHITE: printf("[]"); break;
                default:    printf("ER");
            }
            skipDefaultRendering:
            if(y == BOARD_SIZE - 1) printf("\n");
        }
        printf("__________________\n");
        #undef x
        #undef y

        //print choices
        printf("<player%d>", curPlayer);
        for(uint8 i = 0; i < TOTAL_SLOTS; ++i) {
            if(choices[i].potential == 0) break;
            printf("(%hhu, %hhu, %hhu)", choices[i].posX, choices[i].posY, choices[i].potential);
        }
        printf("\n");

        //input
        printf("<test>");
        scanf("%hhu", &choice);

        //update board
        choices[choice].potential = curPlayer;
        update(board, choices[choice]);

        //change player
        curPlayer = (curPlayer == 1)? 2: 1;

        //clear choices
        for(uint8 i = 0; i < TOTAL_SLOTS; ++i) {
            if(choices[i].potential == 0) break;
            choices[i] = (pcbLoc){0, 0, 0, 0};
        }
    }
}