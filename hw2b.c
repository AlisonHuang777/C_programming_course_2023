#include <stdio.h>
#include <stdbool.h>
#define BOARD_SIZE 8
int main() {
    unsigned char reversi[BOARD_SIZE][BOARD_SIZE], posX, posY, sltDisk, sltColor;
    int i, j, k;
    bool found=false;
    for (i=0; i<BOARD_SIZE; ++i)
        for (j=0; j<BOARD_SIZE; ++j)
            scanf("%hhu", &(reversi[i][j]));
    scanf("%hhu %hhu %hhu", &posX, &posY, &sltColor);
    sltDisk=reversi[posX][posY];
    switch (sltDisk) {
    case 0:
        printf("There is no disk at (%hhu, %hhu).\nThe ", posX, posY);
        if (sltColor==1) printf("black ");
        else printf("white ");
        for (i=-1; i<=1; ++i)
            for (j=-1; j<=1; ++j) {
                if (i==0 && j==0) continue;
                for (k=1; k<=BOARD_SIZE; ++k) {
                    if (posX+i*k<0 || posX+i*k>=BOARD_SIZE ||
                        posY+j*k<0 || posY+j*k>=BOARD_SIZE ||
                        reversi[posX+i*k][posY+j*k]==0) break;
                    if (reversi[posX+i*k][posY+j*k]==sltColor) {
                        if (k!=1) found=true;
                        break;
                    }
                }
                if (found) break;
            }
            if (found) break;
        if (found) printf("disk can be placed here.");
        else printf("disk can\'t be placed here");
        break;
    case 1:
        printf("The disk at (%hhu, %hhu) is black.\nNo new disk can be placed here.", posX, posY);
        break;
    case 2:
        printf("The disk at (%hhu, %hhu) is white.\nNo new disk can be placed here.", posX, posY);
        break;
    }
}