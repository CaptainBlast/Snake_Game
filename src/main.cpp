// clang++ -std=c++11 main.cpp -lncursesw -Wall -c
// clang++ -std=c++11 main.o Snake_Game.cpp -lncursesw -Wall -o t

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

    Snake_Game game;
    game.start();

    endwin();

    return 0;
}
