#include "movement.h"
#include <ncurses.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>
//long long int Score = 0;
int Score = 0;
int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};
int chocolate(int a, int b){
    if(a>b){
        return 2;
    }
    return -1;
}
int calc_man_dist(int x1, int y1, int x2, int y2){
    return abs(x1 - x2) + abs(y1 - y2);
}

int min_idx(int dist[], int size){
    int idx = 0;
    int min = dist[idx];
    for(int i = 0; i < size; i++){
        if(dist[i] < min){
            min = dist[i];
            idx = i;
        }
    }
    return idx;
}

void change_pos(char grid[HEIGHT][WIDTH], me* ply){
    if(ply->nx == 11 && ply->ny == 51 && ply->dy == 1) ply->ny = 0;
    if(ply->nx == 11 && ply->ny == 0 && ply->dy == -1) ply->ny = 51;
    if(ply->sym == 'p'){
        grid[ply->x][ply->y] = ' ';
        grid[ply->x][ply->y + 1] = ' ';        
        if(grid[ply->nx][ply->y + chocolate(ply->ny, ply->y)] == '.'){
            Score++;
        }
    }
    attron(COLOR_PAIR(2));
    mvprintw(20, 100, "Score: %-3d", Score);
    attroff(COLOR_PAIR(2));
    //grid[ply->nx][ply->ny] = ply->sym;
    //grid[ply->nx][ply->ny + 1] = ply->sym;
    //mvaddstr(ply->x, ply->y, "  ");
    //mvaddstr(ply->nx, ply->ny, ply->avt);
    ply->x = ply->nx; ply->y = ply->ny;
    return;
}

int turn(char grid[HEIGHT][WIDTH], char plyin, me* ply){
    if(plyin == 'w'){
        ply->dx = -1;
        ply->dy = 0;
    }
    if(plyin == 's'){
        ply->dx = 1;
        ply->dy = 0;
    }
    if(plyin == 'a'){
        ply->dx = 0;
        ply->dy = -1;
    }
    if(plyin == 'd'){
        ply->dx = 0;
        ply->dy = 1;
    }
    ply->nx = ply->x + ply->speed*ply->dx; ply->ny = ply->y + ply->speed*ply->dy;
    if(grid[ply->nx][ply->ny] != '#' && grid[ply->nx][ply->ny + 1] != '#'){
        change_pos(grid, ply);
        return 1;
    }
    return 0;
}


int greedy_move(char grid[HEIGHT][WIDTH], me* ghost, int dest_x, int dest_y){
    if(ghost->x == dest_x && ghost->y == dest_y) return 0;
    
    int dist[4];
    int move_valid[] = {1, 1, 1, 1};
    int valid_moves = 0;
    
    for(int i = 0; i < 4; i++){
        if ((dx[i] == -(ghost->dx) && dy[i] == -(ghost->dy))) {
            move_valid[i] = 0;
            dist[i] = INT_MAX;
            continue;
        }

        if(grid[ghost->x + dx[i]][ghost->y + dy[i]] != '#' && 
           grid[ghost->x + dx[i]][ghost->y + dy[i] + 1] != '#'){
            
            dist[i] = calc_man_dist(ghost->x + dx[i], ghost->y + dy[i], dest_x, dest_y);
        }
        else {
            move_valid[i] = 0;
            dist[i] = INT_MAX;
        }
    }

    for(int i = 0; i < 4; i++){
        valid_moves += move_valid[i];
    }

    if(valid_moves == 1){
        for(int i = 0; i < 4; i++){
            if(move_valid[i]){
                ghost->dx = dx[i]; ghost->dy = dy[i];
            }
        }
        ghost->nx = ghost->x + ghost->dx; ghost->ny = ghost->y + ghost->dy;
        change_pos(grid, ghost);
        return 1;
    }

    if (valid_moves == 0) {
        ghost->dx = -(ghost->dx);
        ghost->dy = -(ghost->dy);
        ghost->nx = ghost->x + ghost->dx;
        ghost->ny = ghost->y + ghost->dy;
        change_pos(grid, ghost);
        return 1;
    }

 
    int j = min_idx(dist, 4);
    
    ghost->dx = dx[j];
    ghost->dy = dy[j];
    
    ghost->nx = ghost->x + ghost->dx; 
    ghost->ny = ghost->y + ghost->dy;
    
    change_pos(grid, ghost);
    
    if(ghost->dx) napms(100);

    return 1;
}

int pick_random_dir(char grid[HEIGHT][WIDTH], me* ghost, int valid_moves){
    int move_valid[] = {1, 1, 1, 1};
    valid_moves = 0;
    if(valid_moves = -1){
        for(int i = 0; i < 4; i++){
            if ((dx[i] == -(ghost->dx) && dy[i] == -(ghost->dy))) {
                move_valid[i] = 0;
                continue;
            }

            if(grid[ghost->x + dx[i]][ghost->y + dy[i]] == '#' || grid[ghost->x + dx[i]][ghost->y + dy[i] + 1] == '#'){ 
                move_valid[i] = 0;
            }
        }
        
        for(int i = 0; i < 4; i++){
            valid_moves += move_valid[i];
        }
    }

    if(valid_moves == 1){
        for(int i = 0; i < 4; i++){
            if(move_valid[i]){
                ghost->dx = dx[i]; ghost->dy = dy[i];
            }
        }
        ghost->nx = ghost->x + ghost->dx; ghost->ny = ghost->y + ghost->dy;
        change_pos(grid, ghost);
        return 1;
    }

    if (valid_moves == 0) {
        ghost->dx = -(ghost->dx);
        ghost->dy = -(ghost->dy);
        ghost->nx = ghost->x + ghost->dx;
        ghost->ny = ghost->y + ghost->dy;
        change_pos(grid, ghost);
        return 1;
    }

    if(valid_moves > 1){
        int min = 0;
        int max = 3;
        int ran = (rand() % (max - min + 1)) + min; 
        ghost->dx = dx[ran];
        ghost->dy = dy[ran];
        ghost->nx = ghost->x + ghost->dx; 
        ghost->ny = ghost->y + ghost->dy;
        if(grid[ghost->nx][ghost->ny] != '#' && grid[ghost->nx][ghost->ny + 1] != '#'){
            change_pos(grid, ghost);
            return 1;
        }
        pick_random_dir(grid, ghost, valid_moves);
        return 0;
    }
    
}

