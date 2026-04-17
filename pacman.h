#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "movement.h"
#include "ghosts.h"
#include <stdlib.h>
#include <time.h>
//#define GHOST_COLOR_PAIR 1
#define WIDTH 55
#define HEIGHT 24
#define wall_char '#'

void loadgrid();
void print_grid();
void put_entities();
void pacman();
char* get_avt(char sym);
int get_ind(char sym);