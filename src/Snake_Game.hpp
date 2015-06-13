#ifndef SNAKE_GAME_HPP
#define SNAKE_GAME_HPP

#include <string>
#include <vector>
#include <ncursesw/ncurses.h>

class Snake_Game
{
public:
    // initialise and start game
    Snake_Game() { reset(); }

    inline void start();
    void reset(); // reset game
private:
    bool processTrack(const int &); // update head and tail
    void keyPressed();              // check what key was pressed
    void newAppPos();               // generate new position for apple
    void graphics();                // graphics
    void gameLogic();               // ...
    int gameOver();                 // called if the user wants to stop playing or they hit something

    unsigned short appPos = 173, SHead = 223;   // apple position and Snake Head's position
    unsigned short SDir = 4;                    // Snake length and direction snake is headed(1,2,3,4)
    unsigned score = 0;                   // score
    std::string gameMap;
    double speed = 1;
    std::vector<unsigned> snakePos;             // positions of tail
};

inline void Snake_Game::start()
{
    reset();
    init_pair(1, COLOR_GREEN, COLOR_WHITE);         // colour for border
    init_pair(2, COLOR_RED, COLOR_GREEN);           // colour for game map excluding border
    init_pair(3, COLOR_WHITE, COLOR_YELLOW);
    init_pair(4, COLOR_BLUE, COLOR_GREEN);          // colour for game over text

    bkgd(COLOR_PAIR(3));
    gameLogic();
}

void menu();

#endif
