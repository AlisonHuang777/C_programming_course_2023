#include <string.h>
#include <stdio.h>
#define MAX_LEN 100

int main() {
    char text0[MAX_LEN], text1[MAX_LEN];
    printf("text0 (max %d chars): ", MAX_LEN - 1);
    scanf("%s", text0);
    printf("text1 (max %d chars): ", MAX_LEN - 1);
    scanf("%s", text1);
    printf("Length of text0: %lu\nLength of text1: %lu\n", strlen(text0), strlen(text1));
    printf("Is text0 equal to text1? ");
    if(strcmp(text0, text1) == 0)
        printf("Yes\n");
    else
        printf("No\n");
    printf("Does text0 contains text1? ");
    if(strstr(text0, text1) == NULL)
        printf("No\n");
    else
        printf("Yes\n");
    strcpy(text1, text0);
    printf("Copied from text0 to text1: ");
    puts(text1);
    strcat(text0, text1);
    printf("Appended text1 after text0: ");
    puts(text0);
}