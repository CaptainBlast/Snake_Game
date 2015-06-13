#ifndef kbhit_h
#define kbhit_h
#include <ncursesw/ncurses.h>

// checks if a key a has been pressed(non-blocking)
int kbhit()
{
    int ch = getch();
    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 2;
    }
}

#endif
