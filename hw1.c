#include <stdio.h>
int main(){
    for(int i=1;i<=9;++i){
        for(int j=1;j<=9;++j){
            printf("%d x %d = %2.d, ",i,j,i*j);
        }
        printf("\n");
    }
}