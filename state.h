#ifndef _STATE_H_
#define _STATE_H_

#include <iostream>

enum class StateType { NewPiece, Relay, Reply };
enum class Colour { NoColour, Black, White };
enum class Direction { NW, N, NE, E, SE, S, SW, W };

struct State {
  StateType type;  
  Colour colour;   
  Direction direction;
};

#endif
