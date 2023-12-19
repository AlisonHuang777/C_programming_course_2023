#include <stdint.h>
#define MAZE_SIZE 10 // < 4,294,967,296 = 2 ^ 32

typedef struct{uint32_t x, y;} Coordinate;
typedef struct{Coordinate stack[MAZE_SIZE * MAZE_SIZE]; uint64_t pointer;} Stack;

void initStack(Stack *stk, Coordinate initItem);
void pushStack(Stack *stk, Coordinate item);
Coordinate pullStack(Stack *stk);
Coordinate getStackTop(Stack *stk);