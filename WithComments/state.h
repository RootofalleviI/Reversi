#ifndef _STATE_H_
#define _STATE_H_

#include <iostream>
// #define DEBUG

enum class StateType { NewPiece, Relay, Reply };
enum class Colour { NoColour, Black, White };
enum class Direction { NW, N, NE, E, SE, S, SW, W };

struct State {
  StateType type;  
  Colour colour;   
  Direction direction;
};


#ifdef DEBUG
inline std::ostream& operator<<(std::ostream& out, StateType s) {
  if (s == StateType::Relay) {
    out << "> State: Relay" << std::endl;
  } else if (s == StateType::Reply) {
    out << "> State: Reply" << std::endl;
  } else {
    out << "> State: NewPiece" << std::endl;
  }
  return out;
}

inline std::ostream& operator<<(std::ostream& out, Colour c) {
  if (c == Colour::Black) {
    out << "> Color: Black" << std::endl;
  } else if (c == Colour::White) {
    out << "> Color: White" << std::endl;
  } else {
    out << "> Color: NoColour" << std::endl;
  }
  return out;
}

inline std::ostream& operator<<(std::ostream& out, Direction d) {
    if (d == Direction::N) out << "> Direction: N" << std::endl;
    else if (d == Direction::NE) out << "> Direction: NE" << std::endl;
    else if (d == Direction::E) out << "> Direction: E" << std::endl;
    else if (d == Direction::SE) out << "> Direction: SE" << std::endl;
    else if (d == Direction::S) out << "> Direction: S" << std::endl;
    else if (d == Direction::SW) out << "> Direction: SW" << std::endl;
    else if (d == Direction::W) out << "> Direction: W" << std::endl;
    else out << "> Direction: NW" << std::endl;
    return out;
}

inline std::ostream& operator<<(std::ostream& out, State s) {
  out << s.type;
  out << s.colour;
  out << s.direction;
  return out;
}
#endif

#endif
