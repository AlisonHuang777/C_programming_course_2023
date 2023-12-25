#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#pragma region //borrowed these from a mysterious source
typedef struct{char *ptr; unsigned len, siz;} String;

String *initStr() {
    String *str = malloc(sizeof(String));
    str->ptr = calloc(1, sizeof(char));
    str->len = 0;
    str->siz = 1;
    return(str);
}

void append(String *str, char c) {
    *(str->ptr + str->len++) = c;
    if(str->len == str->siz) {
        str->siz *= 2;
        str->ptr = realloc(str->ptr, str->siz * sizeof(char));
    }
    *(str->ptr + str->len) = 0;
}

void printStr(String *str) {
    for(unsigned i = 0; i < str->len; ++i)
        printf("%c", *(str->ptr + i));
    //printf("\n"); disabled
}
#pragma endregion

int main() {
    String *text = initStr();
    char input;
    while(scanf("%c", &input) != EOF) {
        if(isspace(input)) continue; //not accept space chars cuz 'em ruin alignment (or is it even exist?)
        append(text, input);
    }

    printf("Accepted: \"");
    printStr(text);
    printf("\"\n");

    #define c *(text->ptr+i)
    for(unsigned i = 0; c; ++i) { //print some properties of each char of text
        printf("\'%c\':\n", c);
        if(isalnum(c)) {
            printf("\t- is alphanumeric\n");
            if(isalpha(c)) printf("\t- is alphabetic\n");
            else if(isdigit(c)) printf("\t- is a decimal digit\n");
            else{
                printf("\t- wait what?\n");
                return(1); //this should not happen
            }
        }
        else if(ispunct(c)) printf("\t- is a punctuation character\n");
        else{
            printf("\t- tf did u just do?");
        }
    }
    #undef c
}