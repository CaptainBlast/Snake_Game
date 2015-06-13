#include <ncursesw/ncurses.h>
#include "Snake_Game.hpp"

int main()
{
    setlocale(LC_ALL,"");       // utf-8 support

    initscr();

    cbreak();
    noecho();
    start_color();
    curs_set(0);                    // remove cursor
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    scrollok(stdscr, TRUE);

    menu();

    endwin();

    return 0;
}
