#include "cell.h"
#include <string>
using namespace std;

Direction getDir(Info cur, Info src) {
    int rDiff = cur.row - src.row;
    int cDiff = cur.col - src.col;
    switch (rDiff) {
         case -1: 
            switch (cDiff) {
                case -1:    return Direction::NW;   break;
                case 0:     return Direction::N;    break;
                case 1:     return Direction::NE;   break;
            }
        case 0:  
            switch (cDiff) {
                case -1:    return Direction::W;    break;
                case 1:     return Direction::E;    break;
            }
        case 1:  
            switch (cDiff) {
                case -1:    return Direction::SW;   break;
                case 0:     return Direction::S;    break;
                case 1:     return Direction::SE;   break;
            }
        default: throw;
    }
}


Direction oppDir(Direction d) {
    if (d == Direction::N) return Direction::S;
    else if (d == Direction::NE) return Direction::SW;
    else if (d == Direction::E) return Direction::W;
    else if (d == Direction::SE) return Direction::NW;
    else if (d == Direction::S) return Direction::N;
    else if (d == Direction::SW) return Direction::NE;
    else if (d == Direction::W) return Direction::E;
    else return Direction::SE;
}


Cell::Cell(size_t r, size_t c) : r(r), c(c) {
    State s;
    s.direction = Direction::N;
    s.colour = Colour::NoColour;
    s.type = StateType::Relay;
    setState(s);
}


Info Cell::getInfo() const {
    Info i;
    i.row = r;
    i.col = c;
    i.colour = colour;
    return i;
}


void Cell::toggle() {
    if (colour == Colour::Black) colour = Colour::White;
    else colour = Colour::Black;
}


void Cell::setPiece(Colour colour) {

    if (colour == Colour::NoColour) {
        State s;
        s.type = StateType::Relay;
        s.colour = this->colour;
        s.direction = Direction::N;
        setState(s);
    }

    else if (this->colour != Colour::NoColour) {
        throw "";
        return;
    }

    else {
        this->colour = colour;
        State s;
        s.direction = Direction::N;
        s.colour =  colour;
        s.type = StateType::NewPiece;
        setState(s);
        notifyObservers();
    }
}

void Cell::notify(Subject <Info, State> &whoFrom) {

    Info srcInfo = whoFrom.getInfo();
    State srcState = whoFrom.getState();
    State curState = getState();

    Direction myDir = getDir(getInfo(), srcInfo);

    if (srcState.type == StateType::NewPiece) {
        curState.direction = myDir;
        setState(curState);
    }

    else if (myDir != srcState.direction) return;

    if (colour == Colour::NoColour) return;

    if (curState.type == StateType::NewPiece) return;

    if (curState.type == StateType::Reply) return;    

    if (srcState.type != StateType::Reply) {
        if (colour == srcState.colour) {
            curState.direction = oppDir(myDir);
            curState.type = StateType::Reply;
            setState(curState);
            notifyObservers(); 
        } else {
            if (srcState.type != StateType::NewPiece) {
                curState.direction = srcState.direction;
            }
            curState.colour = srcState.colour;
            curState.type = StateType::Relay;
            setState(curState);
            notifyObservers();
        }
    } 
    else {
        setState(srcState);
        notifyObservers();
        toggle();
    }
}










