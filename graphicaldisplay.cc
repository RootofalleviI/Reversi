#include "graphicaldisplay.h"

GraphicalDisplay::GraphicalDisplay(int n) : gridSize(n), w{} {
	w.fillRectangle(0, 0, 500, 500, Xwindow::Blue);
}

/* 
row, col: coordinates.
len: side length of each cell.
I used double type because of the potential rounding errors.
The int type can support gridSize up to 32; after that the center seems to shift too 
much even human eyes can notice; double type fixes this problem.

Known bug: there might be a very small white box shows up on the bottom right corner.
Sometimes it appears and sometimes it does not. I have no clue why it happens.
If you see it, try to create a new game and see if it is still there.
If yes, rerun the executable. 
*/

void GraphicalDisplay::notify(Subject<Info, State> &whoNotified) {
	Info i = whoNotified.getInfo();
	double row = i.row;
	double col = i.col;
	double len = 500.0 / gridSize;
	if (i.colour == Colour::Black) {
		w.fillRectangle(col*len, row*len, len, len, Xwindow::Black);
	} else if (i.colour == Colour::White) {
		w.fillRectangle(col*len, row*len, len, len, Xwindow::White);
	}
}

