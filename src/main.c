#include <ncurses.h>
#include "ecurses.h"
#include "tetris.h"
#include "tetris_win.h"

void print_piece(piece_t piece, int offset_x, int offset_y);

int rows;
int columns;

int main(int argc, char args[]){
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    int width = 20, height = 20;
    WINDOW* tetris_scr = newwin(height+2, width+2, 1, 2);
    tetris_t* tetris = new_tetris(width, height);
    tetris_win_t * tetris_win = new_tetris_win(tetris_scr, tetris);

    run_tetris(tetris);
    //wgetch(tetris_scr);
    getch();
    delwin(tetris_scr);
    endwin();
}

void print_piece(piece_t piece, int offset_x, int offset_y){
    point_t point;
    for(int i = 0; i < piece.qnt_blocks; i++){
        point = piece.points[i];
        mvprintw(offset_y+point.y, offset_x+point.x, "X");
    }
}
