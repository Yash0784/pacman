// Source - https://stackoverflow.com/q/60532492
// Posted by Ryan Keough, modified by community. See post 'Timeline' for change history
// Retrieved 2026-04-11, License - CC BY-SA 4.0
#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define GHOST_COLOR_PAIR 1
#define WIDTH 53
#define HEIGHT 24
#define wall_char '#'
int game_running = 1;
char grid[HEIGHT][WIDTH];
int curr_x = 6;
int half_curr_x = 0;
int curr_y = 6;
void print_grid(){
    for(int y = 0; y < HEIGHT; y++){
        for(int x = 0; x < WIDTH; x++){
            if(grid[y][x] != '\0'){
                if(grid[y][x] == '#'){
                    mvaddch(5+y, 5+x, ACS_CKBOARD | COLOR_PAIR(2));
                }
                else{
                    mvaddch(5+y, 5+x, grid[y][x]);
                }
            }
        }
    } 
}
void initialize_grid(){
    FILE* Grid = fopen("grid2copy.txt", "r");
    int y = 0;
    while(fgets(grid[y], WIDTH+2, Grid) != NULL){
        grid[y][strcspn(grid[y], "\r\n")] = '\0';
        y++;
    }
    fclose(Grid);
}
chtype get_chtype(int y, int x){
    return mvinch(y, x);
}
char get_char(int y, int x){

    return grid[y-5][x-5];
}
char Get_char(int y, int x){
    chtype character = mvinch(y, x);
    char char_at_dest = character & (A_ALTCHARSET | A_CHARTEXT);;
    return char_at_dest;
}
int change_pos(int y, int x, int ch){
    
    switch(ch){
        case KEY_UP: 
        if(get_char(curr_y-1, curr_x) != wall_char && get_char(curr_y-1, curr_x+1)!=wall_char){
            mvaddstr(curr_y, curr_x, "  ");
            //grid[y][x] = ' '; 
            //grid[y-1][x] = "👻";
            curr_y -= 1;
            mvaddstr(curr_y, curr_x,"👻");
            return 1;
        }
        return 0;
        break;
        case KEY_DOWN: 
        if(get_char(curr_y+1, curr_x)!=wall_char && get_char(curr_y+1, curr_x+1)!=wall_char){
            mvaddstr(curr_y, curr_x, "  ");
            //grid[y][x] = ' '; 
            //grid[y+1][x] = "👻";
            curr_y += 1;
            mvaddstr(curr_y, curr_x, "👻");
            return 1;
        }
        return 0;
        break;
        case KEY_RIGHT:
        
            if(get_char(curr_y, curr_x+2)!=wall_char){
                mvaddstr(curr_y, curr_x, "  ");
                mvaddstr(curr_y, curr_x+1, "👻");
                curr_x += 1;
                return 1;
                //half_curr_x += 1;
                //half_curr_x %= 2;
            }
            return 0;
        
        /*else{
            if(get_char(curr_y, 2*curr_x+2)!='#'){
                mvaddstr(curr_y, 2*curr_x, "  ");
                grid[y][x] = ' '; 
                grid[y][x+1] = "👻";
                half_curr_x += 1;
                half_curr_x %= 2;
                mvaddstr(curr_y, 2*curr_x+1, "👻");
            }
        }*/
        break;
        case KEY_LEFT: 
            if(get_char(curr_y, curr_x-1)!=wall_char){
                mvaddstr(curr_y, curr_x, "  ");
                mvaddstr(curr_y, curr_x-1, "👻");
                curr_x -= 1;
                return 1;
            }
            return 0;
  
        /*else{
            if(get_char(curr_y, 2*curr_x-1)!='#'){
                mvaddstr(curr_y, 2*curr_x, "  ");
                grid[y][x] = ' '; 
                grid[y][x-1] = "👻";
                curr_x -= 1;
                half_curr_x += 1;
                half_curr_x %= 2;
                mvaddstr(curr_y, 2*curr_x-1, "👻");
            }
        }*/
        break;
        default: return 0;
    }
}

int main()
{
  initialize_grid();
  setlocale(LC_ALL, "");
  initscr();
  nodelay(stdscr, TRUE);
  cbreak();
  curs_set(0);
  noecho();


  keypad(stdscr, TRUE);

  start_color();
  refresh();
  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_RED);
  print_grid();
  int prev_ch = -1;
  int ch = -1;
  int curr_ch;
  int started = 0;
  int blink = 0;
  while(game_running){
    curr_ch = getch();
    if(curr_ch != ERR){
        ch = curr_ch;
        started = 1;
    }
    if(!started){
        blink++;
        if(blink%2){
            mvaddstr(curr_y, curr_x, "  ");
        }
        else{
            mvaddstr(curr_y, curr_x, "👻");
        }
        refresh();
        napms(150);
    }
    if(ch == 'q'){
        game_running = 0;
        continue;
    }
    if(ch != -1){
        int valid_dir = change_pos(curr_y, curr_x, ch);
        if(valid_dir){
            prev_ch = ch;
        }
        else{
            if(prev_ch != -1){
                int if_not_valid = change_pos(curr_y, curr_x, prev_ch);
            }
        }
    }
    refresh();
    int speed = 100;
    if(prev_ch == KEY_UP || prev_ch == KEY_DOWN){
        speed = 200;
    }
    attron(COLOR_PAIR(1));
    mvprintw(0, 50, "%d, %d", curr_y, curr_x);
    attroff(COLOR_PAIR(1));
    napms(speed);
  }

  /*getch();
  printw(" chocolate");
  refresh();
  
  
  mvaddch(5, 3, ACS_CKBOARD|COLOR_PAIR(1));
  mvaddstr(7, 3, "👻");
  getch();
  refresh();
  getch();*/
  endwin();

  return 0;
}

