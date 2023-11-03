#include <stdio.h>

#define BOARD_SIZE 8
#define TOTAL_SLOTS 64
#define DISK_DATA_SIZE 2

typedef unsigned short uint16;
typedef char sint8;
typedef enum diskType{EMPTY, BLACK, WHITE} diskType;
typedef struct vec2{sint8 x, y;} vec2;

diskType getDisk(uint16 layout[8], vec2 pos) {
    return(layout[pos.x] >> DISK_DATA_SIZE * pos.y & 0b11);
    }

void setDisk(uint16 layout[8], vec2 pos, diskType color) {
    layout[pos.x] = layout[pos.x] & ~(0b11 << pos.y * DISK_DATA_SIZE) | color << pos.y * DISK_DATA_SIZE;
    }