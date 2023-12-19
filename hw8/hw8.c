#include <stdio.h>
#include <stdint.h>
#include "stack.h"

#define PATH 0
#define WALL 1
#define TAKN 2

#define DEBUGGING 1

#if DEBUGGING
void output(uint8_t maze[MAZE_SIZE][MAZE_SIZE], Stack *memory) {
    printf("maze:\n");
    for(uint32_t i = 0; i < MAZE_SIZE; ++i) {
        for(uint32_t j = 0; j < MAZE_SIZE; ++j) {
            switch(maze[i][j]) {
                case 0: printf("  "); break;
                case 1: printf("[]"); break;
                case 2: printf("><"); break;
            }
        }
        printf("\n");
    }
    printf("memory:\n");
    for(uint64_t i = 0; i < memory->pointer + 1; ++i)
        printf("(%u, %u)\n", memory->stack[i].x, memory->stack[i].y);
}
#endif

int main() {
    //initialize
    uint8_t maze[MAZE_SIZE][MAZE_SIZE];
    Stack memory;
    Coordinate startPos;

    //input
    scanf("%u %u", &startPos.x, &startPos.y);
    for(uint64_t i = 0; i < MAZE_SIZE * MAZE_SIZE; ++i)
        scanf("%hhu", &maze[i / MAZE_SIZE][i % MAZE_SIZE]);
    
    //process
    initStack(&memory, startPos);
    for(;;) {
        uint8_t deadEnd = 1;
        Coordinate nowPos = getStackTop(&memory);

        //terminate condition
        if((nowPos.x == startPos.x && nowPos.y == startPos.y && maze[nowPos.x][nowPos.y] == TAKN)
        || (nowPos.x == 0 || nowPos.x == MAZE_SIZE - 1 || nowPos.y == 0 || nowPos.y == MAZE_SIZE - 1)) break;

        maze[nowPos.x][nowPos.y] = TAKN;

        //check untaken path
        if(maze[nowPos.x][nowPos.y - 1] == PATH) {
            pushStack(&memory, (Coordinate){nowPos.x, nowPos.y - 1});
            deadEnd = 0;
        }
        if(maze[nowPos.x][nowPos.y + 1] == PATH) {
            pushStack(&memory, (Coordinate){nowPos.x, nowPos.y + 1});
            deadEnd = 0;
        }
        if(maze[nowPos.x - 1][nowPos.y] == PATH) {
            pushStack(&memory, (Coordinate){nowPos.x - 1, nowPos.y});
            deadEnd = 0;
        }
        if(maze[nowPos.x + 1][nowPos.y] == PATH) {
            pushStack(&memory, (Coordinate){nowPos.x + 1, nowPos.y});
            deadEnd = 0;
        }
        if(deadEnd) pullStack(&memory);
    }

    //output
    output(maze, &memory);
}