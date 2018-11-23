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

    /* Inheried from Subject 
     * std::vector<Observer<Info, State>*> observers;
     * State state;
     * setState(State newS);
     */

public: 
    Cell(size_t r, size_t c);
    void setPiece(Colour colour);
    void toggle();
    void notify(Subject<Info, State> &whoFrom) override;
    Info getInfo() const override;

    /* Inherited from Subject
     * void attach(Observer<Info, State> *o);
     * void notifyObservers();
     * State getState() const;
     */
};

#endif


