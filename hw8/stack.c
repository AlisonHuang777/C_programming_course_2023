#include "stack.h"

void initStack(Stack *stk, Coordinate initItem) {
    stk->stack[0] = initItem;
    stk->pointer = 0;
}

void pushStack(Stack *stk, Coordinate item) {
    ++stk->pointer;
    stk->stack[stk->pointer] = item;
}

Coordinate pullStack(Stack *stk) {
    --stk->pointer;
    return(stk->stack[stk->pointer + 1]);
}

Coordinate getStackTop(Stack *stk) {
    return(stk->stack[stk->pointer]);
}