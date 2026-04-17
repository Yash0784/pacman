#ifndef PACMAN_H
#define PACMAN_H

//#define GHOST_COLOR_PAIR 1
#define WIDTH 55
#define HEIGHT 24
#define wall_char '#'
typedef struct movingEntity{
    char* name;
    char* avt;
    char sym;
    int x;
    int y;
    int dx;
    int dy;
    int speed;
    int nx;
    int ny;
}me;

//speed should always be a whole number and is in the units of no of tiles per call
int min_idx(int dist[], int size);
int calc_man_dist(int x1, int y1, int x2, int y2);
void change_pos(char grid[HEIGHT][WIDTH], me* ply);
int turn(char grid[HEIGHT][WIDTH], char plyin, me* ply);
int greedy_move(char grid[HEIGHT][WIDTH], me* ghost, int dest_x, int dest_y);
int pick_random_dir(char grid[HEIGHT][WIDTH], me* ply, int valid_moves);
int chocolate(int a, int b);
#endif
