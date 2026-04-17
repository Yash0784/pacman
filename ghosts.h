#ifndef GHOSTS_H
#include "movement.h"
#define GHOSTS_H
#define HEIGHT 24
#define WIDTH 55
//❤️
int ghost_spawn(char grid[HEIGHT][WIDTH], me* ghost);
int scatter(char grid[HEIGHT][WIDTH], me* ghosts[], int count);
int target_pacman(char grid[HEIGHT][WIDTH], me* ply, me* ghost);
int target_ahead(char grid[HEIGHT][WIDTH], int x, int y, me* ghost);
int get_inky_pos(char grid[HEIGHT][WIDTH], me* inky, me* blinky, me* ply);
#endif