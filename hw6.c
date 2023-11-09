#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define BOARD_SIZE 8
#define TOTAL_SLOTS BOARD_SIZE*BOARD_SIZE
#define DIRECTIONS 8

typedef enum diskType{EMPTY, BLACK, WHITE} diskType;
typedef struct placeableLocation{uint8_t posX, posY, potential, dirFlag;} pcbLoc;

#define DBS 2 //disk bit size: the least bit size required to represent all the possible states of a disk
diskType getDisk(uint16_t layout[BOARD_SIZE], uint8_t x, uint8_t y) {
    return(layout[x] >> DBS * y & 0b11);
}

void setDisk(uint16_t layout[BOARD_SIZE], uint8_t x, uint8_t y, diskType clr) {
    layout[x] = layout[x] & ~(0b11 << y * DBS) | clr << y * DBS;
}
#undef DBS

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
#define OUT_BOUND xd<0||xd>BOARD_SIZE-1||yd<0||yd>BOARD_SIZE-1
void checkPcb(uint16_t layout[BOARD_SIZE], pcbLoc pcbList[TOTAL_SLOTS], diskType color) {
    for(uint16_t i = 0; i < TOTAL_SLOTS * DIRECTIONS; ++i) {
        if (gd(layout, pos) != EMPTY) {
            i += DIRECTIONS - 1;
            continue;
        }
        for(uint8_t s = 1; s < BOARD_SIZE; ++s) {
            if(OUT_BOUND || gd(layout, dpp) == EMPTY) break;
            if(gd(layout, dpp) == color) {
                for(uint8_t i_ = 0; i_ < TOTAL_SLOTS; ++i_) {
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
void update(uint16_t layout[BOARD_SIZE], pcbLoc selected, diskType color, uint8_t counter[2]) {
    setDisk(layout, pos, color);
    ++counter[color - 1];
    for(uint8_t d = 0; d < DIRECTIONS; ++d) {
        if ((selected.dirFlag >> d & 1) == 0) continue;
        for(uint8_t s = 1; s < BOARD_SIZE; ++s) {
            if(getDisk(layout, dpp) == color) break;
            setDisk(layout, dpp, color);
            ++counter[color - 1];
            --counter[(color ^ 0b11) - 1];
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

uint8_t cptChoice(pcbLoc chs[TOTAL_SLOTS]) {
    uint8_t maxPot = 0, nBst = 0, bstIdx[TOTAL_SLOTS];
    for(uint8_t i = 0; i < TOTAL_SLOTS; ++i) {
        if(chs[i].potential == 0) break;
        maxPot = (chs[i].potential > maxPot)? chs[i].potential: maxPot;
    }
    for(uint8_t i = 0, i_ = 0; i < TOTAL_SLOTS; ++i) {
        if(chs[i].potential == 0) break;
        if(chs[i].potential == maxPot) {
            bstIdx[i_] = i;
            ++nBst;
            ++i_;
        }
    }
    return(bstIdx[rand() % nBst]);
}

//configurations
#define BLACK_SYMBOL "><" //symbol for rendering black disks
#define WHITE_SYMBOL "[]" //symbol for rendering white disks
#define PRINT_PCB_INFO 0  //whether or not to show info about placeable locations
#define DELAY 100         //milliseconds between each loop for simulation mode

int main() {
    
    //initialize
    uint8_t gameMode, turns = 0, choice, totalDisks[2] = {2, 2};
    uint16_t board[BOARD_SIZE] = {0, 0, 0, 576, 384, 0, 0, 0};
    clock_t timestamp;
    pcbLoc choices[TOTAL_SLOTS] = {};
    diskType curPlayer = BLACK;
    printf("This program is not responsible for undefined inputs.\n");
    srand(time(NULL));

    //input gameMode
    printf("Pick a game mode:\n"
           "\t0 for PvP (multiplayer),\n"
           "\t1 for CvP (computer goes first),\n"
           "\t2 for PvC (player goes first),\n"
           "\t3 for CvC (simulation).\n> ");
    scanf("%hhu", &gameMode);
    printf("\n");

    //main loop
    for(;;) {

        //check valid choices for current player
        checkPcb(board, choices, curPlayer);

        //print board
        #define x i/BOARD_SIZE
        #define y i%BOARD_SIZE
        printf("  y 0 1 2 3 4 5 6 7\nx  ________________\n");
        for(uint8_t i = 0, i_ = 0; i < TOTAL_SLOTS; ++i) {
            if(y == 0) printf("%hhu |", x);
            if(choices[i_].potential != 0 && choices[i_].posX == x && choices[i_].posY == y) {
                printf("%02hhu", i_);
                ++i_;
            }
            else switch (getDisk(board, x, y)){
                case EMPTY: printf("  ");         break;
                case BLACK: printf(BLACK_SYMBOL); break;
                case WHITE: printf(WHITE_SYMBOL); break;
                default:    printf("ER");
            }
            if(y == BOARD_SIZE - 1) printf("\n");
        }
        printf("===================\n<Turn #%02hhu> P1\"" BLACK_SYMBOL "\" %02hhu - %02hhu \"" WHITE_SYMBOL "\"P2\n",
               turns++, totalDisks[0], totalDisks[1]);
        #undef x
        #undef y

        //exits if no valid choices
        if(choices[0].potential == 0) break;

        //print choices
        printf("[Player %d\'s turn] Disk symbol: \"", curPlayer);
        if(curPlayer == BLACK) printf(BLACK_SYMBOL);
        else                   printf(WHITE_SYMBOL);
        printf("\"\n");
        #if PRINT_PCB_INFO
            for(uint8_t i = 0; i < TOTAL_SLOTS; ++i) {
                if(choices[i].potential == 0) break;
                printf("\t#%02hhu: (%hhu, %hhu), %hhu;\n", i, choices[i].posX, choices[i].posY, choices[i].potential);
            }
        #endif
        printf("> ");

        //for simulation mode delay between each cycle
        #if DELAY > 0
        if(gameMode == 3) {
            timestamp = clock();
            while(clock() < timestamp + DELAY) {;}
        }
        #endif

        //input choice
        if((gameMode >> curPlayer - 1 & 1) == 1) {
            choice = cptChoice(choices);
            printf("%hhu (by computer)\n", choice);
        }
        else scanf("%hhu", &choice);
        printf("\n");

        //update board
        update(board, choices[choice], curPlayer, totalDisks);

        //change player
        curPlayer ^= 0b11;

        //clear choices
        for(uint8_t i = 0; i < TOTAL_SLOTS; ++i) {
            if(choices[i].potential == 0) break;
            choices[i] = (pcbLoc){0, 0, 0, 0};
        }
    }

    //game over
    printf("[GAMEOVER] ");
    if(totalDisks[0] == totalDisks[1]) printf("Tie.");
    if(totalDisks[0] >  totalDisks[1]) printf("Player 1 wins!");
    else                               printf("Player 2 wins!");
}

/*
The code for delay (line 186 ~ 189) requires re-work;
for some reason the time interval of the delay is somewhat inconsistent,
and it also takes a lot of computational power for the "do nothing" part (line 188).
Currently I have no idea how to improve it.
Also, line 182 is expected to print stuff before delay,
but for whatever reason it's not doint that.
Maybe it has something to do with buffer?
*/