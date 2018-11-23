#include <string>
#include <sstream>
#include "grid.h"
using namespace std;

void Grid::init(size_t n) {

    dim = n;
    if (td) delete td;
    td = new TextDisplay(n);

    if (ob) delete ob;
    ob = new GraphicalDisplay(n);

    theGrid.clear();

    for (size_t i=0; i<dim; ++i) {
        theGrid.emplace_back(vector<Cell>());
        for (size_t j=0; j<dim; ++j) {
            theGrid[i].emplace_back(Cell(i,j));
        }
    }

    for (size_t i=0; i<dim; ++i) {
        for (size_t j=0; j<dim; ++j) {
            initObservers(i, j);
        }
    }

    setObserver(td); setObserver(ob);

    size_t t = dim/2 - 1;
    setPiece(t, t, Colour::Black);
    setPiece(t+1, t+1, Colour::Black);
    setPiece(t, t+1, Colour::White);
    setPiece(t+1, t, Colour::White);
}

void Grid::initObservers(int r, int c) {
    int d = dim;
    if (((r-1) >= 0) && ((c-1) >= 0)) theGrid[r][c].attach(&theGrid[r-1][c-1]);
    if ((r-1) >= 0) theGrid[r][c].attach(&theGrid[r-1][c]);
    if (((r-1) >= 0) && ((c+1) <= d-1)) theGrid[r][c].attach(&theGrid[r-1][c+1]);
    if ((c+1) <= d-1) theGrid[r][c].attach(&theGrid[r][c+1]);
    if (((r+1) <= d-1) && ((c+1) <= d-1)) theGrid[r][c].attach(&theGrid[r+1][c+1]);
    if ((r+1) <= d-1) theGrid[r][c].attach(&theGrid[r+1][c]);
    if (((r+1) <= d-1) && ((c-1) >= 0)) theGrid[r][c].attach(&theGrid[r+1][c-1]);
    if ((c-1) >= 0) theGrid[r][c].attach(&theGrid[r][c-1]);
}

void Grid::setObserver(Observer<Info, State> *ob) {
    for (size_t i=0; i<dim; ++i) {
        for (size_t j=0; j<dim; ++j) {
            theGrid[i][j].attach(ob);
        }
    }
}

void Grid::reset() {
    for (size_t i=0; i<dim; ++i) {
        for (size_t j=0; j<dim; ++j) {
            theGrid[i][j].setPiece(Colour::NoColour);
        }
    }
}

void Grid::setPiece(size_t r, size_t c, Colour colour) {
    reset();    
    if ((r >= dim) || (c >= dim)) { 
        throw "";
        return;
    }

    theGrid[r][c].setPiece(colour);
    if (colour != Colour::NoColour) updateCounter();
} 

int count(string s, char c) {
    size_t x = 0, len = s.size();
    for (size_t i=0; i<len; ++i) {
        if (s[i] == c) ++x;
    }
    return x;
}

void Grid::updateCounter() {
    stringstream ss;
    ss << *td;
    string s = ss.str();
    black = count(s, 'B');
    white = count(s, 'W');
}

void Grid::toggle(size_t r, size_t c) { theGrid[r][c].toggle(); }
bool Grid::isFull() const { return (white+black == dim*dim); }
Grid::~Grid() { delete td; delete ob; }

Colour Grid::whoWon() const {
    if (white>black) return Colour::White;
    else if (black>white) return Colour::Black;
    else return Colour::NoColour;
}

std::ostream &operator<<(ostream &out, const Grid &g) {
    out << *(g.td);
    return out;
}
























