#ifndef SNAKE_GAME_HPP
#define SNAKE_GAME_HPP

#include <string>
#include <vector>
#include <ncursesw/ncurses.h>
#include <random>
#include <chrono>

#define S_TAIL '+'
#define S_HEAD 'o'
#define APPLE '*'
#define RED_APPLE '$'
#define WALL '-'

class Snake_Game
{
public:
    // initialise and start_game game
    Snake_Game() { reset(); }

    inline void start();
    inline void start_game();
    void reset(); // reset game
private:
    bool processTrack(const int &); // update head and tail
    void keyPressed();              // check what key was pressed
    inline unsigned short newPos(); // generate new position for apple
    void graphics();                // graphics
    void gameLogic();               // ...
    int gameOver();                 // called if the user wants to stop playing or they hit something

    unsigned short SHead = 223;   // apple position and Snake Head's position
    unsigned short SDir = 4, redAppCnt = 0, multiplier = 0, redAppPos = 0;                    // Snake length and direction snake is headed(1,2,3,4)
    unsigned score = 0;                   // score
    std::string gameMap;
    unsigned speed = 125;
    std::vector<unsigned> snakePos;             // positions of tail
    bool redApp = false;
};

inline void Snake_Game::start_game()
{
    reset();
    init_pair(1, COLOR_GREEN, COLOR_WHITE);         // colour for border
    init_pair(2, COLOR_RED, COLOR_GREEN);           // colour for game map excluding border
    init_pair(3, COLOR_WHITE, COLOR_YELLOW);        // background
    init_pair(4, COLOR_BLUE, COLOR_GREEN);          // colour for game over text
    init_pair(7,COLOR_RED, COLOR_YELLOW);           // Counter colour
    init_pair(8,COLOR_BLUE, COLOR_YELLOW);           // Counter colour


    bkgd(COLOR_PAIR(3));
    gameLogic();
}

// generates new position for apple
inline unsigned short Snake_Game::newPos()
{
    static std::random_device rd;
    static std::mt19937 mt(rd());

    unsigned short pos = 0;

    do {
        std::uniform_int_distribution<int> dist(52, 389); // generate a number betweeen 52 and 649
        pos = dist(mt);                                // assign to apple's position
    } while(gameMap[pos] != ' ');                      // make sure we aren't replacing anything
    return pos;
}

void menu(Snake_Game &);

inline void Snake_Game::start()
{
    menu(*this);
}

#endif
