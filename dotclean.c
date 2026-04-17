#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    FILE* gridf = fopen("grid2copy.txt", "r");
    char grid[40][100];
    int w, h;
    w = 0; h = 0;
    while(fgets(grid[h], 100, gridf) != NULL){
        for(int i = 0; i < strlen(grid[h]); i++){
            if(grid[h][i] == ' ') grid[h][i] = '.';
        }
        h++;
    }
    w = strlen(grid[0]);
    fclose(gridf);
    gridf = fopen("grid2copy.txt", "w");
    for(int i = 0; i < h; i++){
        fputs(grid[i], gridf);
    }
	printf("w = %d, h = %d\n", w, h);
    if(grid[0][w - 1] == '\n') printf("Yes");
    fclose(gridf);
}
