#include "ghosts.h"
#include "movement.h"
#include <ncurses.h>
#include <string.h>
#define DEBUG 0


void debug_log(const char* message) {
    FILE* f = fopen("debug.txt", "a");
    if (f) {
        fprintf(f, "%s\n", message);
        fclose(f);
    }
}

int ghost_spawn(char grid[HEIGHT][WIDTH], me* ghost){
    if(DEBUG)debug_log("ghost_spawn is called\n");
    char* name = ghost->name;
    int x = ghost->x;
    int y = ghost->y;
    if(DEBUG)debug_log("ghost_spawn is called\n");
    if(!strcmp(name, "Blinky")){
        if(DEBUG)debug_log("B ifff\n");
        mvaddstr(x, y, ghost->avt);
        if(DEBUG)debug_log("ifff\n");
    }
    if(!strcmp(name, "Pinky")){
        mvaddstr(x, y, ghost->avt);
        if(DEBUG)debug_log("ifff\n");
    }
    if(!strcmp(name, "Inky")){
        if(DEBUG)debug_log("I ifff\n");
        mvaddstr(x, y, ghost->avt);
        if(DEBUG)debug_log("ifff\n");
    }
    if(!strcmp(name, "Clyde")){
        if(DEBUG)debug_log("C ifff\n");
        mvaddstr(x, y, ghost->avt);
        if(DEBUG)debug_log("ifff\n");
    }
    return 0;
}
int scatter(char grid[HEIGHT][WIDTH], me* ghosts[], int count){
    int x[] = {1, 1, 22, 22};
    int y[] = {1, 49, 49, 1};
    int notreached[] = {1, 1, 1, 1};
    for(int i = 0; i < count; i++){
        int r;
        if(notreached[i]) r = greedy_move(grid, ghosts[i], x[i], y[i]);
        if(!r) notreached[i] = 0;
    }
    return 0;
}

int target_pacman(char grid[HEIGHT][WIDTH], me* ply, me* ghost){
    greedy_move(grid, ghost, ply->x, ply->y);
    return 0;
}

int target_ahead(char grid[HEIGHT][WIDTH], int x, int y, me* ghost){
    greedy_move(grid, ghost, x, y);
    return 0;
}

int get_inky_pos(char grid[HEIGHT][WIDTH], me* inky, me* blinky, me* ply){
    int x, y;
    x = 4 + 2*(ply->x) - blinky->x; y = 4 + 2*(ply->y) - blinky->y;
    greedy_move(grid, inky, x, y);
    return 0;
}


int move_away(char grid[HEIGHT][WIDTH], me* ghost, me* ply){
    int x, y;
    x = 4 + 2*(ply->x) - ghost->x; y = 4 + 2*(ply->y) - ghost->y;
    greedy_move(grid, ghost, x, y);
    return 0;
}
