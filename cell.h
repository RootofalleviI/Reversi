#ifndef _CELL_H_
#define _CELL_H_

#include <iostream>
#include <cstddef>

#include "subject.h"
#include "observer.h"
#include "state.h"
#include "info.h"
#include "textdisplay.h"

class Cell : public Subject<Info, State>, public Observer<Info, State> {
    const size_t r, c;
    Colour colour = Colour::NoColour;
public: 
    Cell(size_t r, size_t c);
    void setPiece(Colour colour);
    void toggle();
    void notify(Subject<Info, State> &whoFrom) override;
    Info getInfo() const override;

};

#endif


