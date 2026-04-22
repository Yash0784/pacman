#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "movement.h"
#include "ghosts.h"
#include "level2_graph.h"
#include "level2_ghosts.h"
#include "pacman2.h"

#define WIDTH  55
#define HEIGHT 24
#define GHOST_COLOR_PAIR 6
#define EXIT         3
#define OPTIONS      2
#define YELLOW       34
#define GREEN_BLACK  1
#define BLUE_BLACK   6
#define RED_BLACK    7
#define MAGENTA_BLACK 67
#define CYAN_BLACK   68
#define RED_RED      69

static const char ghost_color[] = {RED_BLACK, MAGENTA_BLACK, CYAN_BLACK, YELLOW};
int  game2 = 1;
int  rem_lives2 = 3;
int  frightened2 = 0;
int  f_flag2 = 1;
int  buff_flags2[]= {1, 1, 1, 1};
int  ghost_deployed2[] = {1, 1, 1, 1};

static me* ply2;
char grid2[HEIGHT][WIDTH];
char new_grid2[HEIGHT][WIDTH];
static me*  ghosts2[4];


int graph_ready = 0;
void put_entities2(void);
char* get_avt2(char sym);
int   get_ind2(char sym);
void loadgrid2(void) {
    FILE* f = fopen("grid2copy.txt", "r");
    //if (!f) { return; }
    int h = 0;
    while (h < HEIGHT && fgets(grid2[h], WIDTH, f) != NULL) {
        grid2[h][strcspn(grid2[h], "\n")] = '\0';
        h++;
    }
    fclose(f);
}

void print_grid2(void) {
    int is_frightened = frightened2;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH - 3; x++) {
            //if(!(x >= 0 && x < 24 && y >=0 && y < 52)) continue;
            if (new_grid2[y][x] == '\0') continue;
            char c = new_grid2[y][x];
            if (c == '#') {
                mvaddch(y, x, ACS_CKBOARD | COLOR_PAIR(RED_RED));
            } else if (c == '.') {
                mvaddch(y, x, c | COLOR_PAIR(YELLOW));
            } else if (c == ' ') {
                mvaddch(y, x, c);
            } else if (c == 'p') {
                attron(COLOR_PAIR(GREEN_BLACK));
                mvaddstr(y, x, "ᗧ ");
                attroff(COLOR_PAIR(GREEN_BLACK));
                x++;
            } else if (grid2[y][x] == 'o') {
                attron(COLOR_PAIR(YELLOW));
                mvaddstr(y, x, "●");
                attroff(COLOR_PAIR(YELLOW));
            } else {
                if (!is_frightened) {
                    int idx = get_ind2(c);
                    attron(COLOR_PAIR(ghost_color[idx]));
                    mvaddstr(y, x, get_avt2(c));
                    attroff(COLOR_PAIR(ghost_color[idx]));
                } else {
                    attron(COLOR_PAIR(CYAN_BLACK));
                    mvaddstr(y, x, get_avt2(c));
                    attroff(COLOR_PAIR(CYAN_BLACK));
                }
                x++; 
            }
        }
    }
}

//start level2 heree
void pacman2() {
    if (!graph_ready) {
        graph_init("adjlist.txt");
        graph_ready = 1;
    }
    game2 = 1;
    rem_lives2 = 3;
    frightened2 = 0;
    f_flag2  = 1;
    buff_flags2[0] = buff_flags2[1] = buff_flags2[2] = buff_flags2[3] = 1;
    ghost_deployed2[0] = ghost_deployed2[1] = ghost_deployed2[2] = ghost_deployed2[3] = 1;

    ply2 = malloc(sizeof(me));
    for (int i = 0; i < 4; i++)
        ghosts2[i] = malloc(sizeof(me));

    spawn_p(ply2);
    spawn_B(ghosts2[0]);
    spawn_P(ghosts2[1]);
    spawn_I(ghosts2[2]);
    spawn_C(ghosts2[3]);

    srand((unsigned)time(NULL));
    loadgrid2();

    noecho();
    curs_set(0);
    cbreak();
    nodelay(stdscr, TRUE);
    init_pair(BLUE_BLACK, COLOR_BLUE, COLOR_BLACK);
    init_pair(RED_BLACK, COLOR_RED,  COLOR_BLACK);
    init_pair(MAGENTA_BLACK, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(CYAN_BLACK, COLOR_CYAN, COLOR_BLACK);
    init_pair(RED_RED, COLOR_RED, COLOR_RED);

    print_grid2();
    for(int x = 0; x < WIDTH; x++){
        for(int y = 0; y < HEIGHT; y++){
            char c = new_grid2[y][x];
            if (c == '#') {
                mvaddch(y, x, ACS_CKBOARD | COLOR_PAIR(RED_RED));
            }
        }
    }
    refresh();

    int scat = 0;
    int in = -1;
    int buff_in = -1;
    int curr_in;
    int started = 0;
    int blink = 0;

    time_t fri_str   = 0;
    time_t start_time = time(NULL);

    while (game2 && rem_lives2) {
        if(won()) {game2 = 0; continue;}
        time_t  current_time  = time(NULL);
        double  time_elapsed  = difftime(current_time, start_time);
        if (time_elapsed <=  5)                     scat = 1;
        if (time_elapsed >   5 && time_elapsed <= 25) scat = 0;
        if (time_elapsed >  25 && time_elapsed <= 30) scat = 1;
        if (time_elapsed >  30 && time_elapsed <= 50) scat = 0;
        if (time_elapsed >  50 && time_elapsed <= 55) scat = 1;
        if (time_elapsed >  55)                     scat = 0;

        curr_in = getch();
        if (curr_in != ERR) { in = curr_in; started = 1; }

        if (!started) {
            blink++;
            if (blink % 2) {
                mvaddstr(ply2->x, ply2->y, "  ");
            } else {
                mvaddstr(ply2->x, ply2->y, "ᗧ");
                for (int i = 0; i < 4; i++)
                    ghost_spawn(grid2, ghosts2[i]);
            }
            refresh();
            napms(100);
            continue;
        }

        /* Power-pellet eaten? */
        if (get_state(grid2, buff_flags2)) {
            fri_str    = time(NULL);
            frightened2 = 1;
        }
        if (difftime(current_time, fri_str) > 7) {
            frightened2 = 0;
            f_flag2    = 1;
            ghost_deployed2[0] = ghost_deployed2[1] =
            ghost_deployed2[2] = ghost_deployed2[3] = 1;
        }

        if (in == 'q') { game2 = 0; continue; }

        if (frightened2) scat = 0;
        if (scat) {
            l2_scatter(grid2, ghosts2, 4);
        } else if (frightened2) {
            if (f_flag2) {
                for (int i = 0; i < 4; i++) u_turn(grid2, ghosts2[i]);
                f_flag2 = 0;
            } else {
                for (int i = 0; i < 4; i++)
                    if (ghost_deployed2[i]) l2_move_away(grid2, ghosts2[i], ply2);
            }
        } else {
            /* Chase mode */
            if (ghost_deployed2[0])
                l2_target_pacman(grid2, ply2, ghosts2[0]);        // Blinky 
            if (ghost_deployed2[1])
                l2_target_ahead(grid2,
                                ply2->x + 4 * ply2->dx,
                                ply2->y + 4 * ply2->dy,
                                ghosts2[1]);                       /* Pinky  */
            if (ghost_deployed2[3]) {
                if (calc_man_dist(ply2->x, ply2->y,
                                  ghosts2[3]->x, ghosts2[3]->y) > 8)
                    l2_target_pacman(grid2, ply2, ghosts2[3]);     //Clyde
                else
                    dijk_move(grid2, ghosts2[3], 22, 1);           //Clyde
            }
            if (ghost_deployed2[2])
                l2_get_inky_pos(grid2, ghosts2[2], ghosts2[0], ply2); //I
        }

        if (in != -1) {
            int vturn = turn(grid2, (char)in, ply2);
            if (vturn)
                buff_in = in;
            else
                turn(grid2, (char)buff_in, ply2);
        }

        put_entities2();
        print_grid2();
        for (int i = 0; i < rem_lives2; i++)
            mvprintw(20, 80 + 2*i, "❤️");
        attron(COLOR_PAIR(OPTIONS));
        mvprintw(0, 70, "%-3d, %-3d", ply2->x, ply2->y);
        attroff(COLOR_PAIR(OPTIONS));

        refresh();
        int speed = 150;
        if ((char)buff_in != 'a' && (char)buff_in != 'd') speed *= 2;


        int hit[4] = {0, 0, 0, 0};
        for (int i = 0; i < 4; i++) {
            if ((abs(ply2->x - ghosts2[i]->x) + abs(ply2->y - ghosts2[i]->y) == 1 &&
                 ply2->dx == -ghosts2[i]->dx && ply2->dy == -ghosts2[i]->dy) ||
                (ply2->x == ghosts2[i]->x && ply2->y == ghosts2[i]->y))
                hit[i] = 1;
        }

        if (hit[0] || hit[1] || hit[2] || hit[3]) {
            if (!frightened2) {
                rem_lives2--;
                clear();
                if (rem_lives2 == 0) {
                    game2 = 0;
                } else {
                    start_time   = time(NULL);
                    time_elapsed = 0;
                    spawn_p(ply2);
                    spawn_B(ghosts2[0]); spawn_P(ghosts2[1]);
                    spawn_I(ghosts2[2]); spawn_C(ghosts2[3]);
                }
            } else {
                if (hit[0]) { spawn_B(ghosts2[0]); ghost_deployed2[0] = 0; }
                if (hit[1]) { spawn_P(ghosts2[1]); ghost_deployed2[1] = 0; }
                if (hit[2]) { spawn_I(ghosts2[2]); ghost_deployed2[2] = 0; }
                if (hit[3]) { spawn_C(ghosts2[3]); ghost_deployed2[3] = 0; }
            }
        }

        flushinp();
        napms(speed);
    }


    put_entities2();
    endwin();
    for (int i = 0; i < HEIGHT; i++) {
        new_grid2[i][strcspn(new_grid2[i], "\0")] = '\n';
        fputs(new_grid2[i], stdout);
    }

    free(ply2);
    for (int i = 0; i < 4; i++) {
        free(ghosts2[i]);
    }
    return;
}

void put_entities2(void) {
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            new_grid2[y][x] = grid2[y][x];

    new_grid2[ply2->x][ply2->y]   = ply2->sym;
    new_grid2[ply2->x][ply2->y+1] = ply2->sym;

    for (int i = 0; i < 4; i++) {
        new_grid2[ghosts2[i]->x][ghosts2[i]->y]   = ghosts2[i]->sym;
        new_grid2[ghosts2[i]->x][ghosts2[i]->y+1] = ghosts2[i]->sym;
    }
}

char* get_avt2(char sym) {
    if (sym == 'p') return ply2->avt;
    if (sym == 'B') return ghosts2[0]->avt;
    if (sym == 'P') return ghosts2[1]->avt;
    if (sym == 'I') return ghosts2[2]->avt;
    if (sym == 'C') return ghosts2[3]->avt;
    return " ";
}

int get_ind2(char sym) {
    if (sym == 'B') return 0;
    if (sym == 'P') return 1;
    if (sym == 'I') return 2;
    if (sym == 'C') return 3;
    return 0;
}

