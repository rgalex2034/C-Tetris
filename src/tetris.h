#ifndef TETRIS
#define TETRIS
/**
 * Constant definitions
 */
#define TETRIS_PIECE_EMPTY 0
#define TETRIS_PIECE_SQUARE  1
#define TETRIS_PIECE_LINE  2
#define TETRIS_PIECE_L  3
#define TETRIS_PIECE_INVERTED_L  4
#define TETRIS_PIECE_Z  5
#define TETRIS_PIECE_INVERTED_Z  6
#define TETRIS_PIECE_T  7

#define TETRIS_MAX_PIECES 7

//Tetris events
#define TETRIS_EV_STEP 0
#define TETRIS_EV_MOVE 1

//Tetris piece movement
#define TETRIS_MOV_LEFT 0b001
#define TETRIS_MOV_RIGHT 0b010
#define TETRIS_MOV_DOWN 0b100

/**
 * Base data definition for tetris
 */
typedef struct {
    int x;
    int y;
} point_t;

typedef struct {
    int type;
    point_t position;
    point_t* points;
    int qnt_blocks;
} piece_t;

typedef struct tetris_t{
    char* state;
    piece_t piece;
    int** board;
    int width;
    int height;
    int score;
    struct tetris_observer_t* observer;
} tetris_t;

/**
 * Observer structure
 */
typedef int (*tetris_event_fp)(void* this, tetris_t* tetris, int event_type);
typedef struct tetris_observer_t{
    void* instance;
    tetris_event_fp callback;
} tetris_observer_t;

/**
 * Function declarations
 */
tetris_t* new_tetris(int width, int height);
tetris_t* seed_new_tetris(int width, int height, unsigned int seed);
int move_piece_tetris(tetris_t* tetris, int movement);
int step_tetris(tetris_t*);
void run_tetris(tetris_t*);
void set_tetris_observer(tetris_t* tetris, tetris_observer_t* obs);
void free_tetris(tetris_t* tetris);
#endif
