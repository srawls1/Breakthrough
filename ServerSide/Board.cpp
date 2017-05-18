#include "Board.h"
#include <stdexcept>
#include <stdio.h>

//default constructor
Board::Board() {
  spaces = new Space[64];//set up new spaces array
  int i=0;
  for (; i<16; ++i) { //setup black
    spaces[i] = BLACK;
  }
  for (; i<48; ++i) { //setup empty space between
    spaces[i] = EMPTY;
  }
  for (; i<64; ++i) { //setup white
    spaces[i] = WHITE;
  }
}

//copy constructor
Board::Board(const Board& b) {
  spaces = new Space[64];
  for (int i=0; i<64; ++i) {
    spaces[i] = b.spaces[i];
  }
}

//comparision of two boards
Board& Board::operator=(const Board& b) {
  for (int i=0; i<64; ++i) {
    spaces[i] = b.spaces[i];
  }
  return *this;
}

//destructor
Board::~Board() {
  delete[] spaces;//delete spaces
}

//retreives space at location (x,y)
Space Board::spaceAt(int x, int y) const {
  if (x < 0 || y < 0 || x >= 8 || y >= 8) { //check bounds
    stringstream ss;
    ss << "Indices out of range: (" << x << ',' << y << ')';
    throw range_error(ss.str());//throw error
  }
  return spaces[y*8+x];//return value
}

//list locations of pieces
vector<Loc> Board::listLocs(Space s)
{
  vector<Loc> locs = vector<Loc>();//create vector
  for (unsigned int i = 0; i < 64; i++)//find locations of space s
    if(spaces[i] == s)
      locs.push_back(Loc(i%8,i/8));//add to vector

  return locs;
}

//list valid moves of pieces
vector<Loc> Board::listMoves(Space s)
{
  vector<Loc> locs = listLocs(s);//get location of pices
  vector<Loc> moves = vector<Loc>();//empty vector to return
  for (unsigned int i = 0; i < locs.size(); i++)//for each piece
  {
	int x = (locs[i]).x;//get location x & y
	int y = (locs[i]).y;
	int xEnd, yEnd;

	yEnd = s == WHITE ? y-1 : y+1;//get direction
    if(validMove(x, y, x, yEnd))
	  moves.push_back(Loc(x, y, FWD));//check forward move

	yEnd = s == WHITE ? y-1 : y+1;//get direction
	xEnd = s == WHITE ? x+1 : x-1;
	if(validMove(x,y, xEnd, yEnd))//check right move
	  moves.push_back(Loc(x, y, RIGHT));

  yEnd = s == WHITE ? y-1 : y+1;//get direction
	xEnd = s == WHITE ? x-1 : x+1;
	if(validMove(x,y, xEnd, yEnd))//check left move
	  moves.push_back(Loc(x,y,LEFT));
  }
  return moves;
}

//set space (x,y) to new Space s
void Board::setSpace(int x, int y, Space s) {
  if (x < 0 || y < 0 || x > 7 || y > 7) {//check bounds and possibly through error
    stringstream ss;
    ss << "Indices out of range: (" << x << ',' << y << ')';
    throw range_error(ss.str());
  }
  spaces[y*8+x] = s;//update space
}

//move piece at (x,y) to (xend,yend)
bool Board::movePiece(int xStart, int yStart, int xEnd, int yEnd) {
  //cerr << "FROM: [" << xStart << "][" << yStart << "] to [" << xEnd << "][" << yEnd << "]\n";
  if (validMove(xStart, yStart, xEnd, yEnd)) { //check move
    Space temp = spaceAt(xStart, yStart);//get space
    setSpace(xStart, yStart, EMPTY);//set previous place to empty
    setSpace(xEnd, yEnd, temp);//update new place
    return true;
  }
  else {
    return false;
  }
}

//move piece at (x,y) in direction d, turn is specified to determine if correct player
bool Board::movePiece(int x, int y, dir d, Space turn) {
	if(d == FWD) { //if fwd
		int yEnd = turn == WHITE ? y-1 : y+1;//get direction
		return (movePiece(x, y, x, yEnd));//make move
	}
	else if (d == RIGHT) {//if right
		int yEnd = turn == WHITE ? y-1 : y+1;//get direction
		int xEnd = turn == WHITE ? x+1 : x-1;
		return (movePiece(x, y, xEnd, yEnd));//make move
	}
	else if (d == LEFT) {//if left
		int yEnd = turn == WHITE ? y-1 : y+1;//get direction
		int xEnd = turn == WHITE ? x-1 : x+1;
		return (movePiece(x, y, xEnd, yEnd));//make move
	}
	else {
    //cout << "movePiece returned false" << endl;
    return false;
  }
}

// returns WHITE if white wins, BLACK if black wins, EMPTY if no one wins
Space Board::victory() const {
  // if white is at the end of the board
  for (int i=0; i<8; ++i)
    if (spaces[i] == WHITE)
      return WHITE;
  // if black is at the end of the board
  for (int i=56; i<64; ++i)
    if (spaces[i] == BLACK)
      return BLACK;
  // if one side has no pieces left on the board
  bool wflag = false;
  bool bflag = false;
  for (int i=0; i<64; ++i) {
    if (spaces[i] == WHITE)
      wflag = true;
    if (spaces[i] == BLACK)
      bflag = true;
  }
  if (!wflag)
    return BLACK;
  if (!bflag)
    return WHITE;
  return EMPTY;
}

//check if valid move from (x,y) to (xend,yend)
bool Board::validMove(int xStart, int yStart, int xEnd, int yEnd) const {
  //cout << (char)(xStart+'A') << (char)(yStart+'1') << "->" << (char)(xEnd+'A') << (char)(yEnd+'1') << endl;
  if (xStart < 0 || yStart < 0 || xStart > 7 || yStart > 7) {//board bounds
    //cout << "x bounds" << endl;
    return false;
  }
  if (xEnd < 0 || yEnd < 0 || xEnd > 7 || yEnd > 7) {//board bounds
    //cout << "y bounds" << endl;
    return false;
  }
  if (spaceAt(xStart, yStart) == EMPTY) {//if no piece there
    //cout << "no piece there" << endl;
    return false;
  }
  if (abs(xStart-xEnd) > 1) {//if distance is to far
    //cout << "too far x" << endl;
    return false;
  }
  if (spaceAt(xStart, yStart) == WHITE && yStart-yEnd != 1) {//check y bound distance & direction
    //cout << "y wrong direction/too far: white" << endl;
    return false;
  }
  if (spaceAt(xStart, yStart) == BLACK && yEnd-yStart != 1) {//check y bound distance & direction
    //cout << "y wrong direction/ too far: black" << endl;
    return false;
  }
  if (xStart-xEnd == 0 && spaceAt(xEnd, yEnd) != EMPTY) {//check forward move
    //cout << "moving forward into other piece" << endl;
    return false;
  }
  if (spaceAt(xStart, yStart) == spaceAt(xEnd, yEnd)) {//check if same color
    //cout << "moving into own color" << endl;
    return false;
  }

  return true;//if no errors found
}

//converts space to char
char Board::toCharacter(Space s) const {
  switch (s) {
    case BLACK:
      return 'X';
    case WHITE:
      return 'O';
    case EMPTY:
      return '_';
    default:
      return '@';
  }
}

//prints board to screen
string Board::toString() const {
  stringstream ss;
  ss << ";    A B C D E F G H  \n";//title
  for (int y=0; y<8; ++y) { //inc by row
    ss << "; " << (8-y) << " |";
    for (int x=0; x<8; ++x) { //inc by col
      ss << toCharacter(spaceAt(x, y)) << '|';
    }
    ss <<'\n';
  }
  return ss.str();//convert to string
}
