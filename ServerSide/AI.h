#ifndef AI_H
#define AI_H
#include "Board.h"
#include "MinMaxTree2.h"
#include <vector>
#include <sstream>

class AI {
  int difficulty;//which algorithm to use
  Space side;//which color to use
  string moveMade;//last made move
public:
  AI():AI(8, BLACK) {}//default constructor
  AI(int diff, Space s): difficulty(diff), side(s) {}//constructor
  string getMove();//return previous moveMade

  //find ai move
  void simple_ai_move(Board& b);//easy ai
  void medium_ai_move(Board& b);//medium ai
  void hard_ai_move(Board& b);//hard ai
  void ai_move(Board& b);//calls one of the above based on difficulty
};

#endif
