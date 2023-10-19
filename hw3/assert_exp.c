#include <stdio.h>
#include <assert.h>

int factorial(int n) {
    assert(n>=1); //factorial only works for n >= 1
    return(n==1? 1: n*factorial(n-1));
}

int main() {
    int n;
    printf("n = ");
    scanf("%d", &n);
    printf("n! = %d", factorial(n));
}