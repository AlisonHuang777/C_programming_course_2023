#include <stdlib.h>
#include <stdio.h>

typedef struct{char *ptr; unsigned len, siz;} String;

String *initStr() { //initialize String
    String *str = malloc(sizeof(String)); //allocate memory for String struct
    str->ptr = calloc(1, sizeof(char)); //allocate memory for char array, with initial value "\0"
    str->len = 0;
    str->siz = 1;
    return(str);
}

void append(String *str, char c) { //append a char to String
    *(str->ptr + str->len++) = c;
    if(str->len == str->siz) {
        str->siz *= 2;
        str->ptr = realloc(str->ptr, str->siz * sizeof(char)); //allocate new memory as the String grows
    }
    *(str->ptr + str->len) = 0;
}

void printStr(String *str) { //print String
    for(unsigned i = 0; i < str->len; ++i)
        printf("%c", *(str->ptr + i));
    printf("\n");
}

void freeStr(String *str) { //free the memory allocated for the String
    free(str->ptr);
    free(str);
    str = NULL;
}

int main() {
    String *str = initStr();
    char input;
    while(scanf("%c", &input) != EOF)
        append(str, input);
    printStr(str);
    freeStr(str);
    exit(0); //terminates the process
}