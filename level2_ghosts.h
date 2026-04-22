#ifndef LEVEL2_GHOSTS_H
#define LEVEL2_GHOSTS_H
#include "movement.h"
#include "level2_graph.h"


int dijk_move(char grid[HEIGHT][WIDTH], me* ghost, int dest_x, int dest_y);


int l2_target_pacman(char grid[HEIGHT][WIDTH], me* ply, me* ghost);
int l2_target_ahead(char grid[HEIGHT][WIDTH], int x, int y, me* ghost);
int l2_get_inky_pos(char grid[HEIGHT][WIDTH], me* inky, me* blinky, me* ply);
int l2_move_away(char grid[HEIGHT][WIDTH], me* ghost, me* ply);
int l2_scatter(char grid[HEIGHT][WIDTH], me* ghosts[], int count);

#endif

