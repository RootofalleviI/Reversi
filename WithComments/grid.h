#ifndef _GRID_H_
#define _GRID_H_

#include <iostream>
#include <vector>
#include <cstddef>

#include "cell.h"
#include "state.h"
#include "info.h"
#include "textdisplay.h"

// #define DEBUG

template <typename InfoType, typename StateType> class Observer;

class TextDisplay;
class InvalidMove{};    

class Grid {
    size_t dim = 0;
    size_t black = 0;
    size_t white = 0;
    TextDisplay *td = nullptr;
    Observer<Info, State> *ob = nullptr;
    std::vector<std::vector<Cell>> theGrid;

    void reset();
    void updateCounter();
    void initObservers(int r, int c);  
public:
    void init(size_t n);
    void setObserver(Observer<Info, State> *ob);

    bool isFull() const;
    Colour whoWon() const;
    void toggle(size_t r, size_t c);
    void setPiece(size_t r, size_t c, Colour colour);

    friend std::ostream &operator<<(std::ostream &out, const Grid &g);

    ~Grid();
    
    #ifdef DEBUG
    void print() {
        std::cout << "#Black: " << black << std::endl;
        std::cout << "#White: " << white << std::endl;
        std::cout << std::endl;
    }
    #endif
};

#endif
