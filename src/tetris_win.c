#include <stdlib.h>
#include "tetris_win.h"

int tetris_callback(void* this, tetris_t* tetris, int ev_type);
void print(tetris_win_t* t_win, tetris_t* tetris);
int on_move();

tetris_win_t* new_tetris_win(WINDOW* win, tetris_t* tetris){
    tetris_win_t* tetris_win = malloc(sizeof(*tetris_win));
    tetris_win->tetris = tetris;
    tetris_win->win = win;

    //Set up observer
    tetris_observer_t* obs = malloc(sizeof(*obs));
    obs->instance = tetris_win;
    obs->callback = tetris_callback;

    set_tetris_observer(tetris, obs);

    nodelay(win, TRUE);
    wrefresh(win);

    return tetris_win;
}

int tetris_callback(void* this, tetris_t* tetris, int ev_type){
    tetris_win_t* t_win = (tetris_win_t*)this;
    switch(ev_type){
        case TETRIS_EV_STEP:
            print(t_win, tetris);
            break;
        case TETRIS_EV_MOVE:
            print(t_win, tetris);
            return on_move(t_win);
            break;
    }
    return 0;
}

void print(tetris_win_t* t_win, tetris_t* tetris){
    werase(t_win->win);
    box(t_win->win, 0, 0);

    //Print moving piece
    piece_t piece = tetris->piece;
    int posx = piece.position.x, posy = piece.position.y;
    for(int i = 0; i<piece.qnt_blocks; i++){
        point_t p = piece.points[i];
        wmove(t_win->win, posy+p.y+1, posx+p.x+1);
        waddch(t_win->win, 'C');
    }

    //Print board blocks
    for(int x = 0; x<tetris->width; x++){
        for(int y = 0; y<tetris->height; y++){
            if(tetris->board[x][y]){
                wmove(t_win->win, y+1, x+1);
                waddch(t_win->win, 'C');
            }
        }
    }
    wmove(t_win->win, 0, 0);
    wrefresh(t_win->win);
}

int on_move(tetris_win_t* t_win){
    int movement = 0;
    int ch;
    while((ch = wgetch(t_win->win)) != ERR){
        //fprintf(stderr, "Pressing %c\n", ch);
        if(ch == 'h') movement = movement | TETRIS_MOV_LEFT;
        else if(ch == 'l') movement = movement | TETRIS_MOV_RIGHT;
        else if(ch == 'j') movement = movement | TETRIS_MOV_DOWN;
    }
    //TODO: This will must be used when routine movements are done, instead of return.
    //if(movement) move_piece_tetris(t_win->tetris, movement);
    return movement;
}
