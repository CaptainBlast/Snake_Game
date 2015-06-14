#include "Snake_Game.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <ncursesw/ncurses.h> // ncurses with utf-8 support
#include <locale.h>
#include "Snake_Game.hpp"
#include "kbhit.hpp"

static short row, col;

// when we have a key pressed
void Snake_Game::keyPressed()
{
    switch (getch())              // check what key it was and take action
    {
        case 'W':
        case 'w':
        case KEY_UP:
            if (SDir != 1 && SDir != 3) // make sure we aren't making an impossible move
                SDir = 1;               // set new direction
                break;
        case 'A':
        case 'a':
        case KEY_LEFT:
            if (SDir != 4 && SDir != 2)
                SDir = 4;
            break;
        case 'S':
        case 's':
        case KEY_DOWN:
            if (SDir != 3 && SDir != 1)
                SDir = 3;
            break;
        case 'D':
        case 'd':
        case KEY_RIGHT:
            if (SDir != 2 && SDir != 4)
                SDir = 2;
            break;
    }
}

// stop game for a certain time
inline void stop(unsigned millisec)
{
    const std::chrono::milliseconds timespan(millisec);
    std::this_thread::sleep_for(timespan);
}

// reset game(restart)
void Snake_Game::reset()
{
    appPos = 284, SHead = 223, SDir = 4, score = 0;
    gameMap.assign(420, ' ');
    speed = 125;

    snakePos = {224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234,235,236, 237, 267,297,296,295,294,293};

    gameMap[appPos] = APPLE;    // put onto map
    // make border on map
    for (auto i = 1; i <= 420; ++i)
    {
        if (i <= 30 || i > 390 || i % 30 == 1 || i % 30 == 0)
            gameMap[i-1] = WALL;
    }
}

// called when game is over
int Snake_Game::gameOver()
{
    attron(A_REVERSE | A_BOLD);                                   // Enable bold and reverse attribute
    graphics();                                                   // display game map
    stop(1150);                                                   // let user see his mistake
    attroff(A_REVERSE);                                           // Turn off reverse attribute
    erase();                                                      // clear screen

    mvprintw(row/2, col/2-12, "Game Over!!! \tScore: %d", score); // tell user his score
    attroff(A_BOLD);                                              // turn off bold and colour attribute

    refresh();                                                    // display on screen
    stop(2500);                                                   // pause game to let user read
    return -1;                                                    // return
}

// to refresh the snake's trail, new apple if old one is eaten
bool Snake_Game::processTrack(const int &v)
{
    const std::vector<unsigned> vi = snakePos;  // copy the previous vector

    gameMap.at(snakePos.back()) = ' ';         // remove last bit of tail

    for (auto i = 1; i != snakePos.size(); ++i) // update tail positions
    {
        snakePos[i] = vi[i -1];
    }

    snakePos.front() = SHead;                  // update tail
    SHead += v;                                 // update head

    // if snake hits herself, or borders
    if (gameMap[SHead] == WALL || gameMap[SHead] == S_TAIL)
    {
        gameOver(); // call gameOver
        return true; // stop processing
    }
    // if snake eats an apple
    if (gameMap[SHead] == APPLE)
    {
        score += 4;                             // increment score
        snakePos.push_back(vi.back());        // add another bit to the tail

        if (speed != 95)
            --speed;                     // increase snake's speed
        newAppPos();                            // generate new apple's position
        gameMap[appPos] = APPLE;                // add apple to the game's map
    }
    return false;
}

inline void printBlock(unsigned cnt = 1)
{
    while (cnt != 0)
    {
        addstr("\u25CF");
        --cnt;
    }
}

// output graphics
void Snake_Game::graphics()
{
    erase();                        // clear screen

    if (score < 10)
    {
        printBlock(10);
        printw(" Score: %d ", score);
        printBlock(10);
        addstr("\n");
    }
    else if (score < 100)
    {
        printBlock(10);
        printw(" Score: %d ", score);
        printBlock(9);
        addstr("\n");
    } else
    {
        printBlock(9);
        printw(" Score: %d ", score);
        printBlock(9);
        addstr("\n");
    }

    for (auto i = 1; i <= 420; ++i) // print out game map
    {
        if (i <= 30 || i > 390 || i % 30 == 1 || i % 30 == 0)                // print top border
            addch(ACS_CKBOARD | COLOR_PAIR(1));
        else                                 // print actual game map
        {
            if (gameMap[i-1] == APPLE)
            {
                attron(COLOR_PAIR(4));
                addstr("\u25CF");
                attroff(COLOR_PAIR(4));
            } else if (gameMap[i-1] == S_HEAD)
            {
                attron(COLOR_PAIR(2));
                addstr("\u2588");
                attroff(COLOR_PAIR(2));
            } else if (gameMap[i-1] == S_TAIL)
            {
                attron(COLOR_PAIR(2));
                addstr("\u2593");
                attroff(COLOR_PAIR(2));
            } else
            {
                attron(COLOR_PAIR(2));
                printw("%c", gameMap[i-1]);
                attroff(COLOR_PAIR(2));
            }
        }

        if (i != 0 && i % 30 == 0)  // new line for every 50th character
            printw("\n");
    }

    refresh();                      // display on screen
}

// processes game
void Snake_Game::gameLogic()
{
    int moveBlocks = -1;

    if (processTrack(moveBlocks)) {
        return;
    }

    // update game map
    gameMap[SHead] = S_HEAD;
    for (const auto i : snakePos)
    {
        gameMap[i] = S_TAIL;
    }

    newAppPos();

    std::clock_t start = std::clock();

    while (true)
    {
        if(kbhit())             // if a key has been pressed
            keyPressed();       // call keyPressed to process the key

        if ((std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) > speed)
        {
            // update snakes position
            if (SDir == 1)
                moveBlocks = -30;

            else if (SDir == 2)
                moveBlocks = 1;

            else if (SDir == 3)
                moveBlocks = 30;

            else if (SDir == 4)
                moveBlocks = -1;
            // check it it's game over
            if (processTrack(moveBlocks)) {
                return;
            }

            // update game map
            gameMap[SHead] = S_HEAD;
            for (const auto i : snakePos)
            {
                gameMap[i] = S_TAIL;
            }
            start = std::clock();
            if (SDir % 2 == 1)
                stop(20);
        }

        graphics(); // display to user
    }
}


inline void about()
{
    erase();

    mvprintw(row/2 -2, col/2 -23, "Ncurses Snake game. Designed by CaptainBlastXD");
    mvprintw(row/2, col/2 -9, "Under MIT license.");
    mvprintw(row/2 +1, col/2 -5, "Thank you");
    mvprintw(row/2 +4, col/2 -3, "Return");

    mvaddch(row/2 +4, col/2 -6, '>' | COLOR_PAIR(6));
    mvaddch(row/2 +4, col/2 +5, '<' | COLOR_PAIR(6));

    refresh();
    while(true)
    {
        if (kbhit())
        {
            switch(getch())
            {
                case KEY_BACKSPACE:
                case KEY_HOME:
                case '\n':
                    return;
            }
        }
    }
}
inline void outline(const unsigned short &ch)
{
    //init_pair(6,COLOR_RED, COLOR_CYAN);
    attron(COLOR_PAIR(6));

    static unsigned short last_val = 0;

    switch(last_val)
    {
        case 1:
            mvprintw(row/2-3, col/2-6, " ");
            mvprintw(row/2-3, col/2+3, " ");
            break;
        case 2:
            mvprintw(row/2-1, col/2-6, " ");
            mvprintw(row/2-1, col/2+3, " ");
            break;
        case 3:
            mvprintw(row/2+1, col/2-6, " ");
            mvprintw(row/2+1, col/2+3, " ");
            break;
        case 4:
            mvprintw(row/2+3, col/2-6, " ");
            mvprintw(row/2+3, col/2+3, " ");
            break;
    }

    switch(ch)
    {
        case 1:
            mvprintw(row/2-3, col/2-6, ">");
            mvprintw(row/2-3, col/2+3, "<");
            break;
        case 2:
            mvprintw(row/2-1, col/2-6, ">");
            mvprintw(row/2-1, col/2+3, "<");
            break;
        case 3:
            mvprintw(row/2+1, col/2-6, ">");
            mvprintw(row/2+1, col/2+3, "<");
            break;
        case 4:
            mvprintw(row/2+3, col/2-6, ">");
            mvprintw(row/2+3, col/2+3, "<");
            break;
        default:
            // should throw  excepion
            std::cerr << "Error invalid choice sent to outline.\nOnly 1-4" << std::endl;
            return;
    }

    last_val = ch;

    attroff(COLOR_PAIR(6));
}

void menu()
{
    init_pair(5,COLOR_WHITE, COLOR_CYAN);
    init_pair(6,COLOR_RED, COLOR_CYAN);

    Snake_Game game;
    unsigned short choice = 1;

    getmaxyx(stdscr,row,col);

    bkgd(COLOR_PAIR(5));
    mvprintw(row/2-3, col/2-3, "Play");
    mvprintw(row/2-1, col/2-3, "Mode");
    mvprintw(row/2+1, col/2-3, "Level");
    mvprintw(row/2+3, col/2-3, "About");

    while (true)
    {
        outline(choice);
        if(kbhit())
        {
            switch (getch())              // check what key it was and take action
            {
                case 'W':
                case 'w':
                case KEY_UP:
                    if (choice == 1)
                        choice = 4;
                    else
                        --choice;
                        break;

                case 'S':
                case 's':
                case KEY_DOWN:
                    if (choice == 4)
                        choice = 1;
                    else
                        ++choice;
                    break;

                case '\n':

                    switch (choice)
                    {
                        case 1:
                            game.start();
                            break;
                        case 2:
                            break;
                        case 3:
                            break;
                        case 4:
                            about();
                            break;
                        default:
                            ; // throw exception
                    }
                    erase();
                    bkgd(COLOR_PAIR(5));
                    mvprintw(row/2-3, col/2-3, "Play");
                    mvprintw(row/2-1, col/2-3, "Mode");
                    mvprintw(row/2+1, col/2-3, "Level");
                    mvprintw(row/2+3, col/2-3, "About");
                    break;
            }
        }
        refresh();
    }
}
