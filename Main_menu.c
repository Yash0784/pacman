#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <stdint.h>
#include "movement.h"
#include "ghosts.h"
#include "pacman.h"
#define GHOST_COLOR_PAIR 6
#define WIDTH 55
#define HEIGHT 24
#define wall_char '#'
#define EXIT 3
#define OPTIONS 2
#define YELLOW 34
#define GREEN_BLACK 1 
#define BLUE_BLACK 6
#define RED_BLACK 7
//Include all the header files here
int game_running = 1;
int main(){ 
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    //nodelay(stdscr, TRUE);
    cbreak();
    start_color();
    init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);;
    init_pair(GREEN_BLACK, COLOR_GREEN, COLOR_BLACK);
    init_pair(OPTIONS, COLOR_BLUE, COLOR_BLACK);
    init_pair(EXIT, COLOR_RED, COLOR_BLACK);
    curs_set(0); 
    keypad(stdscr, TRUE); 
    
    const char* choices[] = { "Start Game", "Options", "Quit" };
    int n_choices = 3;
    int highlight = 0;
    int choice = -1;

    while(game_running) {
        clear();
        int yMax, xMax;
        getmaxyx(stdscr, yMax, xMax);

        
        for(int i = 0; i < n_choices; i++) {
            if(i == highlight) {
                attron(COLOR_PAIR(i+1));// highlight and color ts!!
                mvaddch(yMax/2 + i, (xMax - strlen(choices[i]))/2 - 1, '>');
            }
            
            // Print the text (in the middle of the screen for some reason)
            mvprintw(yMax/2 + i, (xMax - strlen(choices[i]))/2, "%s", choices[i]);
            
            attroff(COLOR_PAIR(i+1));
        }
        refresh();

   
        int c = getch();
        if(c == 'q'){
            game_running = 0;
            break;
        }
        switch(c) {
            case KEY_UP:
                highlight = (highlight == 0) ? n_choices - 1 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == n_choices - 1) ? 0 : highlight + 1;
                break;
            case 10: 
                choice = highlight;
                break;
            case 'q': 
                endwin();
                refresh();
                break;
            default:
                break;
        }

        if(choice != -1) 
            break;
    }
    if(choice == 0){
        clear();
        pacman();
        //game();// this is the main part just put whatever function that runs here. like main(); or somn
    }

    // The next window ~uwu~
    if(game_running && choice != 0){
    clear();
    mvprintw(0, 0, "Chocolate: %s", choices[choice]);
    refresh();
    getch();
    }
    clear();
    refresh();
    endwin();
    return 0;
}
