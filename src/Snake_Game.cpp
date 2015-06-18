#include "Snake_Game.hpp"
#include <iostream>
#include <cstdlib>
#include <thread>
#include <stdexcept>
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

// reset game(restart_game)
void Snake_Game::reset()
{
    redApp = false;
    redAppCnt = 0, multiplier = 0, SHead = 223, SDir = 4, score = 0;
    gameMap.reserve(420);
    gameMap.assign(420, ' ');
    speed = 135;

    snakePos = {224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234,235,236, 237, 267,297};

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

    mvprintw(row/2-1, col/2-9, "Game Over!!! Score: %d", score); // tell user his score
    mvprintw(row/2+1, col/2-11, "Press any key to continue.");
    attroff(A_BOLD);                                              // turn off bold and colour attribute

    refresh();                                                    // display on screen
    std::cin.get();                                               // press any key to continue
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
        ++redAppCnt;

        snakePos.push_back(vi.back());        // add another bit to the tail

        if (speed != 100)
            --speed;                     // increase snake's speed
        gameMap[newPos()] = APPLE;                // add new apple to the game's map
        if (redAppCnt == 6)
        {
            redApp = true;
            gameMap[(redAppPos = newPos())] = RED_APPLE;
            redAppCnt = 0;
        }
    }
    else if (gameMap[SHead] == RED_APPLE)
    {
        score += (4*multiplier);                   // increase score
        multiplier = 0;
        redApp = false;
    }

    return false;
}

inline void printCircle(unsigned short cnt = 1)
{
    while (cnt != 0)
    {
        addstr("\u25CF");
        --cnt;
    }
}
inline void printSideBlock(unsigned short cnt = 2)
{
    if (cnt == 0)
    {
        return;
    }
    else if (cnt % 2 == 0)
    {
        while (cnt != 0)
        {
            addstr("\u2588");
            --cnt;
        }
    }
    else
    {
        while (cnt != 1)
        {
            addstr("\u2588");
            --cnt;
        }
        addstr("\u258B");
    }
}


// output graphics
void Snake_Game::graphics()
{
    static auto redAppTimer = std::chrono::high_resolution_clock::now();

    erase();                        // clear screen

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
            }
            else if (gameMap[i-1] == RED_APPLE)
            {
                attron(COLOR_PAIR(2));
                addstr("\u25C9");
                attroff(COLOR_PAIR(2));
            }
            else if (gameMap[i-1] == S_HEAD)
            {
                attron(COLOR_PAIR(2));
                addstr("\u2588");
                attroff(COLOR_PAIR(2));
            }
            else if (gameMap[i-1] == S_TAIL)
            {
                attron(COLOR_PAIR(2));
                addstr("\u2593");
                attroff(COLOR_PAIR(2));
            }
            else
            {
                attron(COLOR_PAIR(2));
                printw("%c", gameMap[i-1]);
                attroff(COLOR_PAIR(2));
            }
        }

        if (i != 0 && i % 30 == 0)  // new line for every 50th character
            printw("\n");
    }

    // print boarder
    attron(COLOR_PAIR(8));
    for (auto cnt = 30; cnt != 0; --cnt)
    {
        addstr("\u2580");
    }

    mvprintw(15, 3, "%d\n", score);
    mvprintw(15, 26, "%d\n", redAppCnt);

    for (auto cnt = 30; cnt != 0; --cnt)
    {
        addstr("\u2584");
    }

    mvprintw(14, 0, "\u2588");
    mvprintw(15, 0, "\u2588");
    mvprintw(16, 0, "\u2588");
    mvprintw(14, 29, "\u2588");
    mvprintw(15, 29, "\u2588");
    mvprintw(16, 29, "\u2588");
    attroff(COLOR_PAIR(8));

    if (redApp)
    {
        if (multiplier == 0)
            multiplier = 10;

        if (std::chrono::duration_cast<std::chrono::milliseconds>(
            (std::chrono::high_resolution_clock::now() - redAppTimer)).count() > 400)
        {
            --multiplier;
            if (multiplier == 0)
            {
                if (gameMap[redAppPos] == RED_APPLE)
                    gameMap[redAppPos] = ' ';
                redApp = false;
            }
            redAppTimer = std::chrono::high_resolution_clock::now();
        }

    }
    if (multiplier > 0)
    {
        attron(COLOR_PAIR(7) | A_BOLD);
        mvprintw(15, 10, "%d ", multiplier);
        printSideBlock(multiplier);
        attroff(COLOR_PAIR(7) | A_BOLD);
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

    gameMap[newPos()] = APPLE;
    auto start_game = std::chrono::high_resolution_clock::now();

    while (true)
    {
        if(kbhit())             // if a key has been pressed
            keyPressed();       // call keyPressed to process the key

        if (std::chrono::duration_cast<std::chrono::milliseconds>(
            (std::chrono::high_resolution_clock::now() - start_game)).count() > (speed+65))
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
            start_game = std::chrono::high_resolution_clock::now();
        }
        else if (std::chrono::duration_cast<std::chrono::milliseconds>(
            (std::chrono::high_resolution_clock::now() - start_game)).count() > (speed))
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
            start_game = std::chrono::high_resolution_clock::now();
        }
        graphics(); // display to user
    }
}


inline void checkSize(const short &row, const short &col)
{
    if (row < 16 || col < 31)
        throw std::runtime_error("Terminal to small! Increase size and restart_game application.");

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
inline void point(const unsigned short &ch)
{
    attron(COLOR_PAIR(6));

    static unsigned short last_val = 0;
    // remove last selected
    switch(last_val)
    {
        case 1:
            mvprintw(row/2-4, col/2-6, " ");
            mvprintw(row/2-4, col/2+3, " ");
            break;
        case 2:
            mvprintw(row/2-2, col/2-6, " ");
            mvprintw(row/2-2, col/2+3, " ");
            break;
        case 3:
            mvprintw(row/2, col/2-6, " ");
            mvprintw(row/2, col/2+3, " ");
            break;
        case 4:
            mvprintw(row/2+2, col/2-6, " ");
            mvprintw(row/2+2, col/2+3, " ");
            break;
        case 5:
            mvprintw(row/2+4, col/2-6, " ");
            mvprintw(row/2+4, col/2+3, " ");
            break;
    }
    // point to new choice
    switch(ch)
    {
        case 1:
            mvprintw(row/2-4, col/2-6, ">");
            mvprintw(row/2-4, col/2+3, "<");
            break;
        case 2:
            mvprintw(row/2-2, col/2-6, ">");
            mvprintw(row/2-2, col/2+3, "<");
            break;
        case 3:
            mvprintw(row/2, col/2-6, ">");
            mvprintw(row/2, col/2+3, "<");
            break;
        case 4:
            mvprintw(row/2+2, col/2-6, ">");
            mvprintw(row/2+2, col/2+3, "<");
            break;
        case 5:
            mvprintw(row/2+4, col/2-6, ">");
            mvprintw(row/2+4, col/2+3, "<");
            break;
        default:
            // should throw  excepion
            std::cerr << "Error invalid choice sent to outline.\nOnly 1-4" << std::endl;
            return;
    }

    last_val = ch; // save last choice

    attroff(COLOR_PAIR(6));
}

inline void printMenu()
{
    mvprintw(row/2-4, col/2-3, "Play");
    mvprintw(row/2-2, col/2-3, "Mode");
    mvprintw(row/2, col/2-3, "Level");
    mvprintw(row/2+2, col/2-3, "About");
    mvprintw(row/2+4, col/2-3, "Exit");

}

void menu(Snake_Game &game)
{
    init_pair(5,COLOR_WHITE, COLOR_CYAN); // Menu colour
    init_pair(6,COLOR_RED, COLOR_CYAN);   // arrow colour

    unsigned short choice = 1;            // holds choice from user

    getmaxyx(stdscr,row,col);             // get size of the terminal
    checkSize(row,col);                   // check size of console

    // print out menu
    bkgd(COLOR_PAIR(5));
    printMenu();

    while (true)
    {
        point(choice);                    // print arrows to current choice
        // if a key was pressed
        if(kbhit())
        {
            switch (getch())              // check what key it was and take action
            {
                case 'W':
                case 'w':
                case KEY_UP:
                    if (choice == 1)      // move
                        choice = 5;
                    else
                        --choice;         // choice
                        break;

                case 'S':
                case 's':
                case KEY_DOWN:
                    if (choice == 5)
                        choice = 1;
                    else
                        ++choice;
                    break;

                case '\n':                 // if key was enter
                    // take action on the choice last
                    switch (choice)
                    {
                        case 1:
                            game.start_game();  // start_game game
                            break;
                        case 2:
                            break;
                        case 3:
                            break;
                        case 4:
                            about();       // about
                            break;
                        case 5:
                            return;
                    }
                    // clear sreen and print menu again
                    erase();
                    bkgd(COLOR_PAIR(5));
                    printMenu();
                    break;
            }
        }
        refresh();
    }
}
