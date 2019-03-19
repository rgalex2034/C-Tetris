#include <ncurses.h>
#include "ecurses.h"
#include "tetris.h"
#include "tetris_win.h"

int main(int argc, char* args[]){
    initscr();
    cbreak(); //Disable tty buffer, so a key is received without waiting for "return" key
    noecho(); //Do not print getch() input
    keypad(stdscr, TRUE); //Enable function keys (such as arrows)

    int width = 20, height = 20;
    WINDOW* tetris_scr = newwin(height+2, width+2, 1, 2);
    tetris_t* tetris = new_tetris(width, height);
    tetris_win_t * tetris_win = new_tetris_win(tetris_scr, tetris);

    run_tetris(tetris);
    free_tetris(tetris);
    //wgetch(tetris_scr);
    getch();
    delwin(tetris_scr);
    endwin();
}
