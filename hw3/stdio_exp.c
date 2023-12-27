#include <stdio.h>

int main() {
    char text[100];
    printf("text (length < 100): "); //print normal string w/ printf()
    scanf("%s", text); //get string w/ scanf()
    printf("first char of text = \'%c\'\nn = ", *text); //print formatted string w/ printf()
    scanf("%s", text);
    int i;
    sscanf(text, "%d", &i); //get (formatted) string w/ sscanf(); can be used to parse int (?)
    printf("parsed n = %d\n", i);
    scanf("%c", text);
    printf("excess char: %hhd (in ASCII)\n", *text); //idk why but after scanf() there is always a '\n' left behind
    printf("text: ");
    gets(text); //get string w/ gets()
    printf("get text: ");
    puts(text); //print string w/ puts()
}