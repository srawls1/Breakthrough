#ifndef BOARD_H
#define BOARD_H
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <iostream>

using namespace std;

//direction of move
enum dir {
  FWD,
  RIGHT,
  LEFT,
  NONE
};

//location space on board
enum Space {
  BLACK,
  WHITE,
  EMPTY
};

//location and possible direction of move
struct Loc {
  int x;
  int y;
  dir d = NONE;
  Loc(int xx, int yy):x(xx),y(yy){}//constructors
  Loc(int xx, int yy, dir dd):x(xx),y(yy),d(dd){}
};

class Board {
  Space* spaces;//board data
public:
  Board();//default constructor
  Board(const Board& b);//coopy constructor
  Board& operator=(const Board& b);//equal comparision
  ~Board();//desstructor
  Space spaceAt(int x, int y) const;//retreives space at location (x,y)
  vector<Loc> listLocs(Space s);//list locations of pieces
  vector<Loc> listMoves(Space s);//list valid moves of pieces
  void setSpace(int x, int y, Space s);//set space (x,y) to new Space s
  bool movePiece(int x, int y, dir d, Space turn);//move piece at (x,y) in direction d, turn is specified to determine if correct player
  bool movePiece(int xStart, int yStart, int xEnd, int yEnd);//move piece at (x,y) to (xend,yend)
  bool validMove(int xStart, int yStart, int xEnd, int yEnd) const;//check if valid move from (x,y) to (xend,yend)
  Space victory() const; // returns WHITE if white wins, BLACK if black wins, EMPTY if no one wins
  char toCharacter(Space s) const;//converts space to char
  string toString() const;//prints board to screen
};

#endif
