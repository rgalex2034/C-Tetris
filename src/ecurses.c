#include "ecurses.h"
#include <string.h>
#include <stdarg.h>

void waddstr_center(WINDOW *w, int argc, ...){
    va_list args;
    int x, y, max_x, max_y;

    getyx(w, y, x);
    getmaxyx(w, max_y, max_x);
    va_start(args, argc);

    for(int i = 0; i<argc; i++){
        char* str = va_arg(args, char*);
        wmove(w, y, max_x/2-strlen(str)/2);
        waddstr(w, str);
        y++;
    }
}
