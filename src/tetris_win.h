#ifndef TETRIS_WIN
#define TETRIS_WIN
#include <ncurses.h>
#include "tetris.h"

typedef struct{
    WINDOW* win;
    tetris_t* tetris;
} tetris_win_t;

tetris_win_t* new_tetris_win(WINDOW* win, tetris_t* tetris);
#endif
