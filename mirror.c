#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(){
    FILE* fptr = fopen("grid2copy.txt", "r");
    char grid[40][100];
    int w, h;
    w = 0; h = 0;
    while(fgets(grid[h], 100, fptr) != NULL){
        for(int i = 0; i < strlen(grid[h]); i++){
            if(grid[h][i] == '.') grid[h][i] = ' ';
        }
        h++;
    }
    w = strlen(grid[0]);
    fclose(fptr);
    for(int i = 0; i < h; i++){
        int j;
        for(j = 0; j < w - 1; j++){
            grid[i][w - 1 + j] = grid[i][w - 2 - j];
        }
        grid[i][w - 1 + j] = '\n';
        grid[i][w + j] = '\0';
    }
    fptr = fopen("grid2copy.txt", "w");
    for(int i = 0; i < h; i++){
        fputs(grid[i], fptr);
    }
	printf("w = %d, h = %d\n", 2*w - 1, h);
    fclose(fptr);
}
