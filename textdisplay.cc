#include "textdisplay.h"
using namespace std;

TextDisplay::TextDisplay(int n) : gridSize(n) {
    for (int i=0; i<gridSize; ++i) {
        theDisplay.emplace_back(vector<char>());
        for (int j=0; j<gridSize; ++j) {
            theDisplay[i].emplace_back('-');
        }
    }
}

void TextDisplay::notify(Subject<Info,State> &whoNotified) {
    Info i = whoNotified.getInfo();
    if (i.colour == Colour::Black) {
        theDisplay[i.row][i.col] = 'B';
    } else { 
        theDisplay[i.row][i.col] = 'W';
    }
}

std::ostream &operator<<(std::ostream &out, const TextDisplay &td) {
    for (auto i=0; i<td.gridSize; ++i) {
        for (auto j=0; j<td.gridSize; ++j) {
            out << td.theDisplay[i][j];
        }
        out << endl;
    }
    return out;
}

