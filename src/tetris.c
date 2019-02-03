#include "tetris.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int** new_board(int width, int height);
piece_t next_piece(tetris_t* tetris);
piece_t new_piece(int type);

/**
 * Public functions
 */
tetris_t* new_tetris(int width, int height){
    return seed_new_tetris(width, height, time(NULL));
}

tetris_t* seed_new_tetris(int width, int height, unsigned int seed){
    tetris_t* tetris = malloc(sizeof(*tetris));
    //Initialize state
    int state_size = 32;
    tetris->state = malloc(sizeof(*(tetris->state))*state_size);
    initstate(seed, tetris->state, state_size);

    //Initialize board
    tetris->board = new_board(width, height);
    tetris->width = width;
    tetris->height = height;
    tetris->score = 0;

    //Initialize piece
    tetris->piece = next_piece(tetris);

    return tetris;
}

void set_tetris_observer(tetris_t* tetris, tetris_observer_t* obs){
    tetris->observer = obs;
}

int step_tetris(tetris_t* tetris){
    int end_game = 0;

    //Try to move the piece down
    if(move_piece_tetris(tetris, TETRIS_MOV_DOWN));
    //In case not, we get a new piece and fix the blocks on the board, if we are not on top
    else if(tetris->piece.position.y != 0){
        for(int i = 0; i < tetris->piece.qnt_blocks; i++){
            point_t point = tetris->piece.points[i];
            int x = point.x + tetris->piece.position.x;
            int y = point.y + tetris->piece.position.y;
            tetris->board[x][y] = tetris->piece.type;
        }
        free(tetris->piece.points);
        tetris->piece = next_piece(tetris);
    }else{
        //If we can't move the piece, and the piece is at top, end the game.
        if(tetris->piece.position.y == 0) end_game = 1;
    }

    tetris->observer->callback(tetris->observer->instance, tetris, TETRIS_EV_STEP);
    return end_game;
}

int move_piece_tetris(tetris_t* tetris, int movement){
    int move = 1;
    //Check if the piece can't be moved to te next block
    for(int i = 0; i < tetris->piece.qnt_blocks; i++){
        point_t point = tetris->piece.points[i];
        int x = point.x + tetris->piece.position.x;
        int y = point.y + tetris->piece.position.y;
        if(movement & TETRIS_MOV_LEFT) x--;
        if(movement & TETRIS_MOV_RIGHT) x++;
        if(movement & TETRIS_MOV_DOWN) y++;
        if(x < 0 || x >= tetris->width || y >= tetris->height || tetris->board[x][y] != 0) move = 0;
    }

    //If can me moved, we step one on the specified position
    if(move){
        if(movement & TETRIS_MOV_LEFT) tetris->piece.position.x--;
        if(movement & TETRIS_MOV_RIGHT) tetris->piece.position.x++;
        if(movement & TETRIS_MOV_DOWN) tetris->piece.position.y++;
    }

    return move;
}

void run_tetris(tetris_t* tetris){
    //Game status variables
    int end = 0;

    //Time limit variables
    long speed = 5e8;
    long speed_acc = 0;

    //Time holding vairables
    struct timespec before = {
        .tv_sec = 0,
        .tv_nsec = 0
    };
    struct timespec now;
    long delta = 0;

    //Game loop
    while(1){
        //Set time variables
        clock_gettime(CLOCK_MONOTONIC, &now);
        if(before.tv_sec){
            delta = (now.tv_sec - before.tv_sec) * 1e9 + (now.tv_nsec - before.tv_nsec);
        }

        //Add up accumulators
        speed_acc += delta;

        //Make step
        for(;speed_acc > speed || !before.tv_sec; speed_acc -= speed){
            end = step_tetris(tetris);
            if(!before.tv_sec) break;
        }
        if(end) break;

        //Trigger frame event
        tetris->observer->callback(tetris->observer->instance, tetris, TETRIS_EV_FRAME);

        //Reset time accumulators if necessary
        if(speed_acc < 0) speed_acc = 0;

        //Force thread to wait and set time before as now
        usleep(16666);
        before = now;
    }
}

void free_tetris(tetris_t* tetris){
    free(tetris->state);
    for(int i = 0; i<tetris->width; i++)
        free(tetris->board[i]);
    free(tetris->board);
    free(tetris->piece.points);
    free(tetris);
}

/**
 * Private functions
 */
int** new_board(int width, int height){
    int** board = malloc(sizeof(*board)*width);
    for(int i = 0; i<width; i++){
        board[i] = calloc(height, sizeof(**board));
    }
    return board;
}

piece_t next_piece(tetris_t* tetris){
    int piece_type = random()%TETRIS_MAX_PIECES+1;//Evade empty piece
    tetris->state = setstate(tetris->state);

    //Create a piece and set it's position to the board's center
    piece_t piece = new_piece(piece_type);
    piece.position.x = tetris->width/2;
    piece.position.y = 0;

    return piece;
}

piece_t new_piece(int type){
    piece_t piece;
    point_t* points;
    int qnt_blocks;

    switch(type){
        case TETRIS_PIECE_SQUARE:
            qnt_blocks = 4;

            points = malloc(sizeof(*points)*qnt_blocks);
            points[0].x = 0; points[0].y = 0;
            points[1].x = 0; points[1].y = 1;
            points[2].x = 1; points[2].y = 0;
            points[3].x = 1; points[3].y = 1;
            break;
        case TETRIS_PIECE_LINE:
            qnt_blocks = 4;

            points = malloc(sizeof(*points)*qnt_blocks);
            points[0].x = 0; points[0].y = 0;
            points[1].x = 0; points[1].y = 1;
            points[2].x = 0; points[2].y = 2;
            points[3].x = 0; points[3].y = 3;
            break;
        case TETRIS_PIECE_L:
            qnt_blocks = 4;

            points = malloc(sizeof(*points)*qnt_blocks);
            points[0].x = 0; points[0].y = 0;
            points[1].x = 0; points[1].y = 1;
            points[2].x = 0; points[2].y = 2;
            points[3].x = 1; points[3].y = 2;
            break;
        case TETRIS_PIECE_INVERTED_L:
            qnt_blocks = 4;

            points = malloc(sizeof(*points)*qnt_blocks);
            points[0].x = 1; points[0].y = 0;
            points[1].x = 1; points[1].y = 1;
            points[2].x = 1; points[2].y = 2;
            points[3].x = 0; points[3].y = 2;
            break;
            break;
        case TETRIS_PIECE_Z:
            qnt_blocks = 4;

            points = malloc(sizeof(*points)*qnt_blocks);
            points[0].x = 0; points[0].y = 0;
            points[1].x = 0; points[1].y = 1;
            points[2].x = 1; points[2].y = 1;
            points[3].x = 1; points[3].y = 2;
            break;
        case TETRIS_PIECE_INVERTED_Z:
            qnt_blocks = 4;

            points = malloc(sizeof(*points)*qnt_blocks);
            points[0].x = 1; points[0].y = 0;
            points[1].x = 1; points[1].y = 1;
            points[2].x = 0; points[2].y = 1;
            points[3].x = 0; points[3].y = 2;
            break;
        case TETRIS_PIECE_T:
            qnt_blocks = 4;

            points = malloc(sizeof(*points)*qnt_blocks);
            points[0].x = 0; points[0].y = 0;
            points[1].x = 1; points[1].y = 0;
            points[2].x = 2; points[2].y = 0;
            points[3].x = 1; points[3].y = 1;
            break;
        default:
            qnt_blocks = 0;
            break;
    }

    piece.type = type;
    piece.points = points;
    piece.qnt_blocks = qnt_blocks;
    return piece;
}
