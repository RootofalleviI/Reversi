#include <string>
#include <sstream>
#include "grid.h"
using namespace std;
// #define DEBUG

/* Initialize the grid */
void Grid::init(size_t n) {

    /* clear the original grid and td then set up the private variables */
    dim = n;
    if (td) delete td;
    td = new TextDisplay(n);
    theGrid.clear();

    /* Initialize Grid::theGrid */
    for (size_t i=0; i<dim; ++i) {
        theGrid.emplace_back(vector<Cell>());
        for (size_t j=0; j<dim; ++j) {
            theGrid[i].emplace_back(Cell(i,j));
        }
    }

    /* Set up neighbors for each cell */
    for (size_t i=0; i<dim; ++i) {
        for (size_t j=0; j<dim; ++j) {
            initObservers(i, j);
        }
    }

    /* TextDisplay td is an observer of every cell */
    setObserver(td);

    /* Finally, setup the middle four pieces */
    size_t t = dim/2 - 1;
    setPiece(t, t, Colour::Black);
    setPiece(t+1, t+1, Colour::Black);
    setPiece(t, t+1, Colour::White);
    setPiece(t+1, t, Colour::White);
}


/* Set up neighbors for each cell */
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


/* Attach ob as an observer to every cell */
void Grid::setObserver(Observer<Info, State> *ob) {
    for (size_t i=0; i<dim; ++i) {
        for (size_t j=0; j<dim; ++j) {
            theGrid[i][j].attach(ob);
        }
    }
}


/* Reset each cell type to Relay
 *
 * Read the documentation on Cell::setPiece.
 * Assigning a cell with NoColour does not modify its color, rather, it tells
 * the cell that this round of searching/backtracking is over, I need you to 
 * go back to Relay type and get reach for the next round. 
 */
void Grid::reset() {
    for (size_t i=0; i<dim; ++i) {
        for (size_t j=0; j<dim; ++j) {
            theGrid[i][j].setPiece(Colour::NoColour);
        }
    }
}



/* The most important grid-level function */
void Grid::setPiece(size_t r, size_t c, Colour colour) {
    
    // Before we begin setting piece, we need to reset the entire board.
    reset();    

    // Input validation: r and c valid?
    if ((r >= dim) || (c >= dim)) { 
        
        #ifdef DEBUG
        cout << "Invalid Position: (" << r << ", " << c << ")" << std::endl;
        #endif 

        throw "";
        return;

    }

    // Call the cell-level set piece.
    // By the time this operation is done, everything should be set.

    theGrid[r][c].setPiece(colour);

    // If we are not resetting, we need to update counters.
    if (colour != Colour::NoColour) updateCounter();

    #ifdef DEBUG
    print();
    #endif
} 


/* Helper for updateCounter */
int count(string s, char c) {
    size_t x = 0, len = s.size();
    for (size_t i=0; i<len; ++i) {
        if (s[i] == c) ++x;
    }
    return x;
}


/* Update counter 
 * 
 * I get that this is rather a bad way of updating counter, since with the ideal
 * observer pattern implementation, counter should be updated size_tmatically when
 * we call the grid-level toggle. However, in my implementation, everything is
 * handled by recursion in cell-level toggle and setPiece, so nothing can be
 * passed back. Besides reading from *td (shouldn't affect too much running time
 * as the gridSize is small), I can only think of hacking the return type of 
 * overridden function to return the number of cells flipped and I feel that is
 * probably even worse as it completely break encapsulation.
 */

void Grid::updateCounter() {
    stringstream ss;
    ss << *td;
    string s = ss.str();
    black = count(s, 'B');
    white = count(s, 'W');
}


/* Some trivial functions */
void Grid::toggle(size_t r, size_t c) { theGrid[r][c].toggle(); }
bool Grid::isFull() const { return (white+black == dim*dim); }
Grid::~Grid() { delete td; }

Colour Grid::whoWon() const {
    if (white>black) return Colour::White;
    else if (black>white) return Colour::Black;
    else return Colour::NoColour;   // indicating a tie
}

std::ostream &operator<<(ostream &out, const Grid &g) {
    out << *(g.td);
    return out;
}
























