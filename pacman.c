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
#define GHOST_COLOR_PAIR 6
#define EXIT 3
#define OPTIONS 2
#define YELLOW 34
#define GREEN_BLACK 1 
#define BLUE_BLACK 6
#define RED_BLACK 7
#define MAGENTA_BLACK 67
#define CYAN_BLACK 68
#define RED_RED 69
char ghost_color[] = {RED_BLACK, MAGENTA_BLACK, CYAN_BLACK, YELLOW};
int game = 1;
int rem_lives = 3;
int frightened = 0;
int f_flag = 1;
int buff_flags[] = {1, 1, 1, 1};
int ghost_deployed[] = {1, 1, 1, 1};

me* ply;
char grid[HEIGHT][WIDTH];
char new_grid[HEIGHT][WIDTH];
me* ghosts[4];
void put_entities();
char* get_avt(char sym);
int get_ind(char sym);
void loadgrid(){
    FILE* gridf = fopen("grid2copy.txt", "r");
    int h = 0;
    while(fgets(grid[h], 55, gridf) != NULL){
        grid[h][strcspn(grid[h], "\n")] = '\0';
        h++;
    }
    fclose(gridf);
}

void print_grid(){
    int is_frightened = frightened;
    for(int y = 0; y < HEIGHT; y++){
        for(int x = 0; x < WIDTH - 3; x++){
            if(new_grid[y][x] != '\0' && new_grid[y][x] != '\r' && new_grid[y][x] != '\n'){
                if(new_grid[y][x] == '#'){
                    mvaddch(y, x, ACS_CKBOARD | COLOR_PAIR(RED_RED));
                }
                else{
                    if(new_grid[y][x] == '.'){
                        mvaddch(y, x, new_grid[y][x] | COLOR_PAIR(YELLOW));
                    }
                    else{
                        if(new_grid[y][x] == ' '){
                            mvaddch(y, x, new_grid[y][x]);
                        }
                        else{
                            //if(!(x >= 0 && x < 24 && y >=0 && y < 52)) continue;
                            if(new_grid[y][x] == 'p'){
                                attron(COLOR_PAIR(GREEN_BLACK));
                                mvaddstr(y, x, get_avt(new_grid[y][x]));
                                attroff(COLOR_PAIR(GREEN_BLACK));
                            }
                            else if(grid[y][x] == 'o'){
                                attron(COLOR_PAIR(YELLOW)); // Yellow
                                mvaddstr(y, x, "●");
                                attroff(COLOR_PAIR(YELLOW));
                            }
                            else{
                                if(!is_frightened){
                                    attron(COLOR_PAIR(ghost_color[get_ind(new_grid[y][x])]));
                                    mvaddstr(y, x, get_avt(new_grid[y][x]));
                                    attroff(COLOR_PAIR(ghost_color[get_ind(new_grid[y][x])]));
                                }
                                else{
                                    attron(COLOR_PAIR(CYAN_BLACK));
                                    mvaddstr(y, x, get_avt(new_grid[y][x]));
                                    attroff(COLOR_PAIR(CYAN_BLACK));
                                }
                                
                            }
                            //debug here
                            x++;
                        }
                    }
                }
            }
        }
    }
}

void pacman(){
    ply = (me*)malloc(sizeof(me));
    for(int i = 0; i < 4; i++){
        ghosts[i] = (me*)malloc(sizeof(me));
    }
    spawn_p(ply);
    spawn_B(ghosts[0]);
    spawn_P(ghosts[1]);
    spawn_I(ghosts[2]);
    spawn_C(ghosts[3]);
    srand(time(NULL));
    //setlocale(LC_ALL, "");
    loadgrid();
    //initscr();
    noecho();
    curs_set(0);
    cbreak();
    nodelay(stdscr, TRUE);
    //start_color();
    init_pair(BLUE_BLACK, COLOR_BLUE, COLOR_BLACK);
    init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK);
    init_pair(MAGENTA_BLACK, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(CYAN_BLACK, COLOR_CYAN, COLOR_BLACK);
    init_pair(RED_RED, COLOR_RED, COLOR_RED);
    print_grid();
    refresh();
    int scat = 0;
    int buff_in = -1;
    int in = -1;
    int curr_in;
    int started = 0;
    int blink = 0;
    time_t fri_str;
    time_t start_time = time(NULL);
    while(game && rem_lives){
        if (won() == 1) {game = 0; continue;}
        //for(int i = 0; i < rem_lives; i++){
          //  mvprintw(20, 80 + 2*i, "❤️");
        //}
        time_t current_time = time(NULL);
        double time_elapsed = difftime(current_time, start_time);
        if(time_elapsed <= 5 && time_elapsed >=0) scat = 1;
        if(time_elapsed <= 25 && time_elapsed > 5) scat = 0;
        if(time_elapsed <= 30 && time_elapsed > 25) scat = 1;
        if(time_elapsed <= 50 && time_elapsed > 30) scat = 0;
        if(time_elapsed <= 55 && time_elapsed > 50) scat = 1;
        if(time_elapsed > 55) scat = 0;
        curr_in = getch();
        if(curr_in != ERR && (curr_in == 'w' || curr_in == 'a' || curr_in == 's' || curr_in == 'd' || curr_in == 'q')){
            in = curr_in;
            started = 1;
        }
        if(!started){
            blink++;
            if(blink%2){
                mvaddstr(ply->x, ply->y, "  ");
            }
            else{
                mvaddstr(ply->x, ply->y, "ᗧ");
                for(int i = 0; i < 4; i++){
                    ghost_spawn(grid, ghosts[i]);
                }
            }
            refresh();
            napms(100);
        }
        if(get_state(grid, buff_flags)){
            fri_str = time(NULL);
            frightened = 1;
        }
        if(difftime(current_time, fri_str) > 7){
            frightened = 0;
            f_flag = 1;
            ghost_deployed[0] = 1;
            ghost_deployed[1] = 1;
            ghost_deployed[2] = 1;
            ghost_deployed[3] = 1;

        }
        if(in == 'q'){
            game = 0;
            continue;
        }
        if(frightened) scat = 0;
        if(1){//tspmoooooooooooooooooooooooooo
            if(scat){
                scatter(grid, ghosts, 4);
            }
            else{
                if(frightened){
                    if(f_flag){
                        for(int i = 0; i < 4; i++){
                            u_turn(grid, ghosts[i]);
                        }
                        f_flag = 0;
                    }
                    else{
                        for(int i = 0; i < 4; i++){
                            if(ghost_deployed[i])move_away(grid, ghosts[i], ply);
                        }
                    }
                }
                else{
                    if(ghost_deployed[0])
                        target_pacman(grid, ply, ghosts[0]);
                    if(ghost_deployed[1])
                        target_ahead(grid, ply->x + 4*ply->dx, ply->y + 4*ply->dy, ghosts[1]);
                    if(ghost_deployed[3]){
                        if(calc_man_dist(ply->x, ply->y, ghosts[3]->x, ghosts[3]->y) > 8){
                            target_pacman(grid, ply, ghosts[3]);
                        }
                        else{
                            greedy_move(grid, ghosts[3], 22, 1);
                        }
                    }
                    if(ghost_deployed[2])
                        get_inky_pos(grid, ghosts[2], ghosts[0], ply);
                }
            }
        }

        int vturn;
        if(in != -1){
            vturn = turn(grid, (char)in, ply);
            if(vturn){
                buff_in = in;
            }
            else{ 
                turn(grid, (char)buff_in, ply);
            }
        }
        put_entities();
        print_grid();   
        for(int i = 0; i < rem_lives; i++){
            mvprintw(20, 80 + 2*i, "❤️");
        }
        refresh();
        int speed = 150;
        if((char)buff_in != 'a' && (char)buff_in != 'd'){
            speed *= 2;
        }
        attron(COLOR_PAIR(2));
        mvprintw(0, 70, "%-3d, %-3d", ply->x, ply->y);
        attroff(COLOR_PAIR(2));
        int c[] = {0, 0, 0, 0};
        for(int i = 0; i < 4; i++){
            if(((abs(ply->x - ghosts[i]->x) + abs(ply->y - ghosts[i]->y)) == 1 && ply->dx == - ghosts[i]->dx && ply->dy == - ghosts[i]->dy) || (ply->x == ghosts[i]->x && ply->y == ghosts[i]->y)) c[i] = 1;
        }
        if(c[0] || c[1] || c[2] || c[3]){
            if(!frightened){
                rem_lives--;
                clear();
                if(rem_lives == 0){
                    game = 0;
                }
                else{
                    start_time = time(NULL);
                    time_elapsed = 0;
                    spawn_p(ply);
                    spawn_B(ghosts[0]);
                    spawn_P(ghosts[1]);
                    spawn_I(ghosts[2]);
                    spawn_C(ghosts[3]);
                }
            }
            else{
                for(int i = 0; i < 4; i++){
                    if(i == 0&&c[i]){spawn_B(ghosts[0]); ghost_deployed[0] = 0;}
                    if(i == 1&&c[i]){spawn_P(ghosts[1]); ghost_deployed[1] = 0;}
                    if(i == 2&&c[i]){spawn_I(ghosts[2]); ghost_deployed[2] = 0;}
                    if(i == 3&&c[i]){spawn_C(ghosts[3]); ghost_deployed[3] = 0;} 
                }
            }
        }
        flushinp();
        napms(speed);
    }
    put_entities();
    endwin();
    for(int i = 0; i < HEIGHT; i++){
        new_grid[i][strcspn(new_grid[i], "\0")] = '\n';
        fputs(new_grid[i], stdout);
    }
    return ;
}
void put_entities(){
    for(int y = 0; y < HEIGHT; y++){
        for(int x = 0; x < WIDTH; x++){
            new_grid[y][x] = grid[y][x];
        }
    }
    new_grid[ply->x][ply->y] = ply->sym;
    new_grid[ply->x][ply->y+1] = ply->sym;
    for(int i = 0; i < 4; i++){ 
        new_grid[ghosts[i]->x][ghosts[i]->y] = ghosts[i]->sym;
        new_grid[ghosts[i]->x][ghosts[i]->y + 1] = ghosts[i]->sym;
    }
    return;
}
char* get_avt(char sym){
    if(sym == 'p'){
        return ply->avt;
    }
    if(sym == 'B'){
        return ghosts[0]->avt;
    }
    if(sym == 'P'){
        return ghosts[1]->avt;
    }
    if(sym == 'I'){
        return ghosts[2]->avt;
    }
    if(sym == 'C'){
        return ghosts[3]->avt;
    }
    return NULL;
}
int get_ind(char sym){
    if(sym == 'B'){
        return 0;
    }
    if(sym == 'P'){
        return 1;
    }
    if(sym == 'I'){
        return 2;
    }
    if(sym == 'C'){
        return 3;
    }
    return 0;
}
