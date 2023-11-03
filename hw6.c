#include <stdio.h>

#define BOARD_SIZE 8
#define TOTAL_SLOTS BOARD_SIZE * BOARD_SIZE
#define DISK_DATA_SIZE 2

typedef unsigned short uint16;
typedef char sint8;
typedef enum diskType{EMPTY, BLACK, WHITE} diskType;
typedef struct vec2{sint8 x, y;} vec2;