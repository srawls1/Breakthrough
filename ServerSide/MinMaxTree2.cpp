#include "MinMaxTree2.h"
#include <cstdlib>

int evaluate_leaf(Board& b, Space side) {
  int yours = 0;
  int opponents = 0;
  for (int y=0; y<8; ++y) {
    for (int x=0; x<8; ++x) {
      if (b.spaceAt(x, y) == side) // if it's your piece
        ++yours;
      else if (b.spaceAt(x, y) != EMPTY) // it's not your piece, but it's not empty, so it must be your opponent's
        ++opponents;
    }
  }

  //cout << b.toString() << yours-opponents << endl;
  return yours-opponents;
}

bool prune(int a, int b, bool max) { // a being the parent node's value, b being the current node's value
  if (max && a<=b) {
    return true;
  }
  if (!max && b<=a) {
    return true;
  }
  return false;
}

int evaluate_root(Board& b, Space side, int depth, bool max, bool pruning, int parentval) {
  Space v = b.victory();
  if (v == side) { // if you won
    return 16;
  }

  if (v != EMPTY) { // you didn't win, but someone did, so it must be opponent
    return -16;
  }

  if (depth == 0) {
    int val = evaluate_leaf(b, side);
    //cout << b.toString() << val << endl;
    return val;
  }

  Space otherSide = side == BLACK ? WHITE : BLACK;

  int optimal = max ? -17 : 17;

  vector<Loc> moves = b.listMoves(max ? side : otherSide);

  for (Loc move : moves) {
    Board b1 = b;
    b1.movePiece(move.x, move.y, move.d, max ? side : otherSide);
    int val = evaluate_root(b1, side, depth-1, !max, pruning, optimal);
    if ((max && val>optimal) ||  (!max && val<optimal)) {
      optimal = val;
    }
    if (pruning && prune(parentval, optimal, max))
      break;
  }

  return optimal;
}

Loc get_optimal_move(Board& b, Space side, int depth, bool pruning) {
  vector<Loc> moves = b.listMoves(side);
  vector<int> scores(moves.size());
  int max = -17;
  Space otherSide = side == BLACK ? WHITE : BLACK;
  for (int i=0; i< moves.size(); ++i) {
    Board b1 = b;
    Loc l = moves[i];
    /*cout << "" << (char)('A'+l.x) << (char)('1'+l.y);
    switch(l.d)
    {
        case(0): cout << " FWD\n"; break;
        case(1): cout << " RIGHT\n"; break;
        case(2): cout << " LEFT\n"; break;
        default: cout << " NONE\n"; break;//should never be used
    }
    cout << endl;*/
    b1.movePiece(l.x, l.y, l.d, side);
    //cout << b1.toString();
    //cout << endl;
    scores[i] = evaluate_root(b1, side, depth-1, false, pruning, -17);
    if (scores[i] > max)
      max = scores[i];
  }

  // pick from optimal moves randomly
  vector<Loc> optimalmoves;
  for (int i=0; i<scores.size(); ++i) {
    if (scores[i] == max)
      optimalmoves.push_back(moves[i]);
  }

  Loc l = optimalmoves[rand()%optimalmoves.size()];
  /*cout << "" << (char)('A'+l.x) << (char)('1'+l.y);
  switch(l.d)
  {
      case(0): cout << " FWD\n"; break;
      case(1): cout << " RIGHT\n"; break;
      case(2): cout << " LEFT\n"; break;
      default: cout << " NONE\n"; break;//should never be used
  }
  cout << " is the optimal move with a score of " << max << endl;*/

  return l;
}
