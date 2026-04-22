#include "level2_ghosts.h"
#include "movement.h"
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

int dijk_move(char grid[HEIGHT][WIDTH], me* ghost, int dest_x, int dest_y) {
    if (ghost->x == dest_x && ghost->y == dest_y) return 0;

    int src  = node_at(ghost->x, ghost->y);
    int dest = node_at(dest_x, dest_y);
    // go only if dir is validd
    if (src == NO_NODE || dest == NO_NODE) {
        return greedy_move(grid, ghost, dest_x, dest_y);
    }

    signed char dir = dir_table[src][dest];
    if (dir == -1) {
        return greedy_move(grid, ghost, dest_x, dest_y);
    }
    int ndx = DIR_DX[(int)dir];
    int ndy = DIR_DY[(int)dir];
    int nx  = ghost->x + ndx;
    int ny  = ghost->y + ndy;
    if (grid[nx][ny] == '#' || grid[nx][ny + 1] == '#') {
        return greedy_move(grid, ghost, dest_x, dest_y);
    }
    ghost->dx = ndx;
    ghost->dy = ndy;
    ghost->nx = nx;
    ghost->ny = ny;
    change_pos(grid, ghost);
    return 1;
}


int l2_target_pacman(char grid[HEIGHT][WIDTH], me* ply, me* ghost) {
    return dijk_move(grid, ghost, ply->x, ply->y);
}


int l2_target_ahead(char grid[HEIGHT][WIDTH], int x, int y, me* ghost) {
    return dijk_move(grid, ghost, x, y);
}


int l2_get_inky_pos(char grid[HEIGHT][WIDTH], me* inky, me* blinky, me* ply) {
    int x = 2 * ply->x - blinky->x;   /* same formula as original +4 offset */
    int y = 2 * ply->y - blinky->y;
    return dijk_move(grid, inky, x, y);
}


int l2_move_away(char grid[HEIGHT][WIDTH], me* ghost, me* ply) {
    int ghost_node = node_at(ghost->x, ghost->y);
    int ply_node   = node_at(ply->x,   ply->y);

    if (ghost_node == NO_NODE || ply_node == NO_NODE) {
        int rx = 2 * ghost->x - ply->x;
        int ry = 2 * ghost->y - ply->y;
        return greedy_move(grid, ghost, rx, ry);
    }
    int best_dir  = -1;
    int best_dist = -1;

    for (int d = 0; d < 4; d++) {
        int v = g_nodes[ghost_node].nb[d];
        if (v == NO_NODE || v == -1) continue;

        //Don't allow u turn
        if (DIR_DX[d] == -(ghost->dx) && DIR_DY[d] == -(ghost->dy)) continue;
        int dist_to_ply = 0;
        dist_to_ply = abs(g_nodes[v].x - ply->x) + abs(g_nodes[v].y - ply->y);
        int nx = ghost->x + DIR_DX[d];
        int ny = ghost->y + DIR_DY[d];
        if (nx < 0 || nx >= HEIGHT || ny < 0 || ny + 1 >= WIDTH) continue;
        if (grid[nx][ny] == '#' || grid[nx][ny + 1] == '#') continue;

        if (dist_to_ply > best_dist) {
            best_dist = dist_to_ply;
            best_dir  = d;
        }
    }

    if (best_dir == -1) {
        ghost->dx = -(ghost->dx);
        ghost->dy = -(ghost->dy);
        ghost->nx = ghost->x + ghost->dx;
        ghost->ny = ghost->y + ghost->dy;
        change_pos(grid, ghost);
        return 1;
    }

    ghost->dx = DIR_DX[best_dir];
    ghost->dy = DIR_DY[best_dir];
    ghost->nx = ghost->x + ghost->dx;
    ghost->ny = ghost->y + ghost->dy;
    change_pos(grid, ghost);
    return 1;
}

int l2_scatter(char grid[HEIGHT][WIDTH], me* ghosts[], int count) {
    static const int cx[] = {1,  1, 22, 22};
    static const int cy[] = {1, 49, 49,  1};

    for (int i = 0; i < count; i++) {
        dijk_move(grid, ghosts[i], cx[i], cy[i]);
    }
    return 0;
}

