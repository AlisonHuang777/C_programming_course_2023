#include <stdio.h>
#include <stdbool.h>
#define BOARD_SIZE 8
int main() {
    unsigned char reversi[BOARD_SIZE][BOARD_SIZE], posX, posY, sltDisk; //This game is called "Reversi" (in English), not "Chess".
    int i, j, k;
    bool noOutput=true;
    for (i=0; i<BOARD_SIZE; ++i)
        for (j=0; j<BOARD_SIZE; ++j)
            scanf("%hhu", &(reversi[i][j]));
    scanf("%hhu %hhu", &posX, &posY);
    sltDisk=reversi[posX][posY];
    if (sltDisk!=0)
        for (i=-1; i<=1; ++i)
            for (j=-1; j<=1; ++j) {
                if (i==0 && j==0) continue;
                for (k=1; k<=BOARD_SIZE; ++k) {
                    if (posX+i*k<0 || posX+i*k>=BOARD_SIZE ||
                        posY+j*k<0 || posY+j*k>=BOARD_SIZE ||
                        reversi[posX+i*k][posY+j*k]==sltDisk) break;
                    if (reversi[posX+i*k][posY+j*k]==0) {
                        if (k!=1) {
                            if (!noOutput) printf(", ");
                            printf("(%hhu,%hhu)", posX+i*k, posY+j*k);
                            noOutput=false;
                            }
                        break;
                    }
                }
            }
    if (noOutput) printf("No valid places found.");
}