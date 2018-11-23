#include "cell.h"
#include <string>
using namespace std;

// #define DEBUG

/* Local helper: Given two Info objects, determine the direction */
Direction getDir(Info cur, Info src) {
    int rDiff = cur.row - src.row;
    int cDiff = cur.col - src.col;
    switch (rDiff) {
         case -1: // cur one row above src
            switch (cDiff) {
                case -1:    return Direction::NW;   break;
                case 0:     return Direction::N;    break;
                case 1:     return Direction::NE;   break;
            }
        case 0:  // cur same row as src
            switch (cDiff) {
                case -1:    return Direction::W;    break;
                case 1:     return Direction::E;    break;
            }
        case 1:  // cur one row below src
            switch (cDiff) {
                case -1:    return Direction::SW;   break;
                case 0:     return Direction::S;    break;
                case 1:     return Direction::SE;   break;
            }
        default:
            throw "shouldn't get here!";
    }
}

/* Local helper: Given a direction, determine the opposite direction */
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


#ifdef DEBUG
void printCoord(string s, size_t r, size_t c) {
    cout << s << "(" << r << "," << c << ")" << endl;
}
#endif


/* Ctor */
Cell::Cell(size_t r, size_t c) : r(r), c(c) {
    State s;
    s.direction = Direction::N;
    s.colour = Colour::NoColour;
    s.type = StateType::Relay;
    setState(s);
}


/* Trivial */
Info Cell::getInfo() const {
    Info i;
    i.row = r;
    i.col = c;
    i.colour = colour;
    return i;
}


/* Flip the color of current cell, 2nd important cell-level function */
void Cell::toggle() {

    /* First, flip color */
    if (colour == Colour::Black) {
        colour = Colour::White;
    } else {
        colour = Colour::Black;
    }

    /*
    // Set current cell as a newPiece and see if it changes anything
    State s;
    s.type = StateType::NewPiece;
    s.direction = Direction::N;
    s.colour = colour;
    setState(s);

    #ifdef DEBUG
    printCoord("Cell::toggle(): Flipping ", r, c);
    cout << "Notifying my observers!" << endl;
    #endif

   notifyObservers();
    */
}


/* Set the color of current cell, or reset current cell type */
void Cell::setPiece(Colour colour) {

    /* NoColour indicates we are resetting cell type. */
    if (colour == Colour::NoColour) {
        State s;
        s.type = StateType::Relay;
        s.colour = this->colour;
        s.direction = Direction::N;
        setState(s);
    }

    /* If the cell has color already, raise an error indicating occupied */
    else if (this->colour != Colour::NoColour) {

        #ifdef DEBUG
        printCoord("Cell::setPiece: Occupied already ", r, c);
        #endif
        throw "";
        return;
    }

    /* Otherwise, we are actually playing a new piece. */
    else {
        this->colour = colour;

        State s;
        s.direction = Direction::N;
        s.colour =  colour;
        s.type = StateType::NewPiece;
        setState(s);

        #ifdef DEBUG
        printCoord("Cell:setPiece: New Piece at ", r, c);
        #endif 

        notifyObservers();
    }
}


/* The most important function! */
void Cell::notify(Subject <Info, State> &whoFrom) {

    Info srcInfo = whoFrom.getInfo();
    State srcState = whoFrom.getState();
    State curState = getState();

    #ifdef DEBUG
    cout << "===================================" << endl;
    printCoord("I am cell ", r, c);
    printCoord("I was notified by ", r, c);

    cout << "srcState has type " << srcState.type;
    cout << "srcState has color " << srcState.colour; 
    cout << "srcState has direction " << srcState.direction;
    cout << "My color is " << colour;
    #endif


    // Termination condition 1. Wrong direction
    // 1.0 First, which direction am I in wich respect to the source?
    Direction myDir = getDir(getInfo(), srcInfo);

    /* 1.1 If the source is a newPiece, we have the following conclusion
     *  a) I am within 1 unit of distance with the newPiece.
     *  b) We are starting a new round of search and the srcState.direction
     *     is garbage value. 
     * Therefore, my direction with respect to the source determines 
     * which direction we want to go. For example, if my left adjacent cell
     * is a newPiece and I got notified, it means we are searching towards its
     * right (Direction::E) in this round, i.e.e want to check if there exists
     * a cell with color equals to srcState.color on my right. Thus, I want
     * to update my direction so that when I notify my observers, only the
     * cell to the right of me would have the matching direction; others
     * will be ignored. This is the basic idea of my cell-level recursion.
     */

    if (srcState.type == StateType::NewPiece) {

        curState.direction = myDir;
        setState(curState);

        #ifdef DEBUG
        cout << "I am called by a newPiece.";
        cout << "Calculating and updating my direction." << endl;
        cout << "Diretion for this round of search is: " << curState.direction;
        #endif

    }

    /* 1.2 If the source is relaying, it means that srcState.direction is 
     * computed already. We want to check whether the current cell is on
     * the desired direction. If not, return directly.
     */
    else if (myDir != srcState.direction) {

        #ifdef DEBUG
        cout << "I am called by a relaying piece and I have wrong direction.";
        cout << endl;
        cout << "My direction with respect to the source is " << myDir;
        cout << "But the source direction is " << srcState.direction;
        cout << "Terminate." << endl;
        #endif

        return;
    }

    // Termination condition 2. I am not initialized (I have no color).
    if (colour == Colour::NoColour) {

        #ifdef DEBUG
        cout << "I am not initialized yet. Terminate." << endl;
        #endif

        return;
    }

    // Termination condition 3. I am the newPiece. 
    if (curState.type == StateType::NewPiece) {

        #ifdef DEBUG
        cout << "I am a new piece. Terminate." << endl;
        #endif
        
        return;
    }

    // Termination condition 4. I have reply type.
    // This indicates my job in this round of game has been completed.
    if (curState.type == StateType::Reply) {

        #ifdef DEBUG
        cout << "I have replied already. Terminate." << endl;
        #endif
        
        return;
    }

    /* If we have reached this point, we know the following things:
     * 1. I am in the right direction and I should be notified.
     * 2. I must have type relay.
     * Our job now is to figure whether we are doing forward searching, i.e.
     * searching to see if there is a cell of the desired color in this line,
     * or are we doing backtrack, i.e. we have already found a cell that has
     * the desired color, and we are doing backward recursion, stop when reach
     * the original newPiece. To determine this, we shall check whoFrom's 
     * type. If it is a newPiece or a relay, we are searching; if it is reply,
     * we are backtracking.
     */

    // Searching!
    if (srcState.type != StateType::Reply) {

        #ifdef DEBUG
        cout << "Searching..." << endl;
        #endif
        
        /* If I have the same color as the srcState.color, this round of 
         * searching is over. We update our type to reply, reverse the 
         * direction of propagation, and start backtracking. */
        if (colour == srcState.colour) {
            curState.direction = oppDir(myDir);
            curState.type = StateType::Reply;
            setState(curState);

            // I put notifyObservers() in each block in the if stmt.
            // These repetitions of code are for clarity. I'll optimize
            // my code in the clean version.
            notifyObservers(); 

            #ifdef DEBUG
            cout << "You found me! I have the same color as srcState!" << endl;
            cout << "Time to reverse the direction to: " << curState.direction;
            cout << "Time to update my type to: " << curState.type;
            #endif
        }

        /* If I don't have the same color as srcState.color, we need to
         * pass the srcState information, including color and direction,
         * to my observers. My type would stay relay.
         */
        else {

            // If the source is a newPiece, then its direction is garbage,
            // The direction I calculated for myself is the correct one.
            // However if the source is relay, I must copy its direction.
            if (srcState.type != StateType::NewPiece) {
                curState.direction = srcState.direction;
            }
            curState.colour = srcState.colour;
            curState.type = StateType::Relay;
            setState(curState);
            notifyObservers();

            #ifdef DEBUG
            cout << "Sorry, I don't have the color you want." << endl;
            cout << "I'll notify my neighbors and help you!" << endl;
            cout << "I updated state direction to: " << curState.direction;
            cout << "I updated state type to: " << curState.type;
            cout << "I updated state color to: " << curState.colour;
            #endif
        }
    } 

    /* Now we are backtracking. The source has a state reply. All we need 
     * to do is to update my state to reply (since I know that I am the 
     * cell on the right direction). I want to pass the reply back until I 
     * reach the newPiece. Note that I notified my observers first before
     * changing color. This is just to make sure that I first get back to
     * the new piece before dirty recursion starts. toggle() would set me
     * to type newPiece and see if my update would influence other cells
     * on the board. */
    else {
        setState(srcState);

        #ifdef DEBUG
        cout << "Backtracking." << endl;
        cout << "I'll notify my neighbors until get to the new piece" << endl;
        cout << "I updated state direction to: " << srcState.direction;
        cout << "I updated state type to: " << srcState.type;
        cout << "I updated state color to: " << srcState.colour;
        #endif

        notifyObservers();
        toggle();

        
    }
}










