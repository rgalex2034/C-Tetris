#ifndef ECURSES
#define ECURSES
#include <curses.h>
#define addstr_center(argc, ...) waddstr_center(stdscr, argc, __VA_ARGS__)
void waddstr_center(WINDOW *w, int argc, ...);
#endif
