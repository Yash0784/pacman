#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct node{
    int id;
    int x;
    int y;
    int n1;
    int n2;
    int n3;
    int n4;
}gnode;

void neighcheck(gnode* adjlist[], int nodecount){
    for(int i = 0; i < nodecount; i++){
        for(int j = 0; j < nodecount; j++){
            int dx = (adjlist[j])->x - (adjlist[i])->x;
            int dy = (adjlist[j])->y - (adjlist[i])->y;
            if(dx == -1 && dy == 0) (adjlist[i])->n1 = (adjlist[j])->id;
            if(dx == 0 && dy == 1) (adjlist[i])->n2 = (adjlist[j])->id;
            if(dx == 1 && dy == 0) (adjlist[i])->n3 = (adjlist[j])->id;
            if(dx == 0 && dy == -1) (adjlist[i])->n4 = (adjlist[j])->id;
        }
    }
}


int main(){
    FILE* fptr = fopen("grid2copy.txt", "r");
    char grid[24][53];
    int h = 0;
    while(fgets(grid[h], 100, fptr) != NULL){
        h++;
    }
    fclose(fptr);
    int nodecount = 0;
    gnode* adjlist[1000];
    for(int i = 0; i < h; i++){
        for(int j = 0; j < 53; j++){
            if(grid[i][j] == '.' && grid[i][j + 1] == '.'){
                gnode* node = (gnode*)malloc(sizeof(gnode));
                if (node != NULL) {
                    node->id = nodecount;
                    node->x = i;
                    node->y = j;
                    node->n1 = -1;
                    node->n2 = -1;
                    node->n3 = -1;
                    node->n4 = -1;
                }
                adjlist[nodecount] = node;
                nodecount++;
            }
        }
    }
    neighcheck(adjlist, nodecount);
    FILE* fout = fopen("adjlist.txt", "w");
    for(int i = 0; i < nodecount; i++){
        fprintf(fout, "%d -> %d %d %d %d\n", adjlist[i]->id, adjlist[i]->n1, adjlist[i]->n2, adjlist[i]->n3, adjlist[i]->n4);
    }
    printf("%d\n", nodecount);
    fclose(fout);
}