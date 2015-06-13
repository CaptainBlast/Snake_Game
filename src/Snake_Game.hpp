#ifndef SNAKE_GAME_HPP
#define SNAKE_GAME_HPP

#include <string>
#include <vector>
#include <ncursesw/ncurses.h>
#include <random>

#define S_TAIL '+'
#define S_HEAD 'o'
#define APPLE '*'
#define WALL '-'

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
    inline void newAppPos();               // generate new position for apple
    void graphics();                // graphics
    void gameLogic();               // ...
    int gameOver();                 // called if the user wants to stop playing or they hit something

    unsigned short appPos = 215, SHead = 223;   // apple position and Snake Head's position
    unsigned short SDir = 4;                    // Snake length and direction snake is headed(1,2,3,4)
    unsigned score = 0;                   // score
    std::string gameMap;
    unsigned speed = 115;
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

// generates new position for apple
inline void Snake_Game::newAppPos()
{
    static std::random_device rd;
    static std::mt19937 mt(rd());

    do {
        std::uniform_int_distribution<int> dist(52, 649); // generate a number betweeen 52 and 649
        appPos = dist(mt);                                // assign to apple's position
    } while(gameMap[appPos] != ' ');                      // make sure we aren't replacing anything(apple,snake,wall)
}


void menu();

#endif
