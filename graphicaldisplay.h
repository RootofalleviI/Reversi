#ifndef _GRAPHICAL_
#define _GRAPHICAL_

#include "window.h"
#include "observer.h"
#include "subject.h"
#include "state.h"
#include "info.h"
#include <iostream>
#include <vector>

class GraphicalDisplay : public Observer<Info, State> {
	const int gridSize;
	Xwindow w;
public:
	GraphicalDisplay(int n);
	void notify(Subject<Info, State> &whoNotified) override;
};

#endif

