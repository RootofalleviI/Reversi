#ifndef _TEXTDISPLAY_H_
#define _TEXTDISPLAY_H_

#include <iostream>
#include <vector>
#include "observer.h"
#include "state.h"
#include "info.h"
#include "subject.h"

class Cell;

class TextDisplay: public Observer<Info, State> {
  std::vector<std::vector<char>> theDisplay;
  const int gridSize;
 public:
  TextDisplay(int n);
  void notify(Subject<Info, State> &whoNotified) override;
  friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

#endif
