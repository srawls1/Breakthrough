#include "MinMaxTree.h"
#include <queue>
#include <climits>
int cnt = 0;
int minmax(Node*n, int depth, bool maximizing){
  if (depth == 1)
    return n->eval();
  if(maximizing){
    int v = INT_MIN;
    const vector<Node*>& nodes = n->get_children();
    for(int i = 0; i < nodes.size(); ++i){

        int val = minmax(nodes[i],depth-1,false);
        v = max(v,val);
    }
    return v;
  }
  else{
    int v = INT_MAX;
    const vector<Node*>& nodes = n->get_children();
    for(int i = 0; i < nodes.size(); ++i){

        int val = minmax(nodes[i],depth-1,true);
        v = min(v,val);
    }
    return v;
  }
}
int alphabeta(Node* n, int depth, int a, int b, bool maximizing) {
  //cout << "alphabeta" << endl;
  ++cnt;
  //cout << "depth: "<< depth << endl;
  if (depth == 1){
    //cout << "eval: " << n->eval() << endl;
    return n->eval();
  }
  if(maximizing){
    int v = INT_MIN;
    const vector<Node*>& nodes = n->get_children();
    for(int i = 0; i < nodes.size(); ++i){

        v = max(v,alphabeta(nodes[i],depth-1,a,b,false));
        a = max(v,a);
        if(b <= a){
          break;
        }
    }
    //cout << v << endl;
    return v;
  }
  else{
    int v = INT_MAX;
    const vector<Node*>& nodes = n->get_children();
    for(int i = 0; i < nodes.size(); ++i){

        v = min(v,alphabeta(nodes[i],depth-1,a,b,true));
        b = min(v,b);
        if(b <= a)
          break;
    }
    //cout << v << endl;
    return v;
  }

}

Node::Node(Board& bb, Loc m, Space s, bool p, bool mx)
: b(bb), value(0), move(m), side(s), pruning(p), max(mx)
{}
/*
 * Node functions
 */
Node::~Node() {
  for (int i=0; i<children.size(); ++i) {
    delete children[i];
  }
}

int Node::eval() {
  if (children.empty()) {
    value = evaluate_leaf(b, side,move,max);
    //cout << "value: " << value << endl;
  } else {
    int val = children[0]->eval();
    value += val;
    /*for (int i=1; i<children.size(); ++i) {
      int new_val = children[i]->eval();
      if (max && new_val > value)
        value += new_val;
      if (!max && new_val < value)
        value += new_val;
    }*/
  }
  return value;
}



/*
 * MinMaxTree functions
 */
MinMaxTree::MinMaxTree(Board& bb, Space s, int d, bool p, Loc loc) {
  root = new Node(bb, loc, side, p, true); // root node is a max node
  Space otherSide = s == WHITE ? BLACK : WHITE;
  b = bb;
  side = s;
  depth = d;
  pruning = p;



  queue<Node*> maxNodes;
  queue<Node*> minNodes;
  maxNodes.push(root);

  for (int i=0; i<depth; ++i) {
    if(root->b.listMoves(side).size() == 0) break;
    if (i%2 == 0) {
      while (!maxNodes.empty()) {
        Node* max = maxNodes.front();
        maxNodes.pop();

        for (Loc move : max->b.listMoves(side)) {
          Board b1 = max->b;
          //b1.movePiece(move.x, move.y, move.d, side);
          Node* min = new Node(max->b, move, side, pruning, false);
          max->children.push_back(min);
          minNodes.push(min);
          
        }
      }
    } else {
      while (!minNodes.empty()) {
        Node* min = minNodes.front();
        minNodes.pop();

        for (Loc move : min->b.listMoves(otherSide)) {
          Board b1 = min->b;
          //b1.movePiece(move.x, move.y, move.d, otherSide);
          Node* max = new Node(min->b, move, side, pruning, true);
          min->children.push_back(max);
          maxNodes.push(max);
          
        }
      }
    }

  }
}

MinMaxTree::~MinMaxTree() {
  delete root;
}

Pair* MinMaxTree::get_optimal_move() {
  if(!pruning){
    int val = minmax(root,depth,true);
    vector<Loc> moves;
    for (Node* mn : root->children) {
      if (mn->value == val)
        moves.push_back(mn->move);
    }
    return new Pair(moves[0],val);
  }
  else{
    int val = alphabeta (root,depth,INT_MIN,INT_MAX,true);
    cout << root->move.x << " " << root->move.y << " " << root->move.d << " " << val <<endl;
    return new Pair(root->move,val);
  }
}

/*
 * Utility function
 */
int evaluate_leaf(Board& b, Space side,Loc loc, bool maximizing) {
  Space v = b.victory();
  if (v == side) { // if you won
    return 16;
  }
  if (v != EMPTY) { // you didn't win, and someone has won, so it must be your opponent
    return -16;
  }
  Space otherSide = side == WHITE ? BLACK : WHITE;
  if(b.spaceAt(loc.x,loc.y) == otherSide){
    if(maximizing){
      return 1;
    }
    else{ 
      cout << "can capture" << endl;
      return -1;
    }
  }
  return 0;


  /*int yours = 0;
  int opponents = 0;
  for (int y=0; y<8; ++y) {
    for (int x=0; x<8; ++x) {
      if (b.spaceAt(x, y) == side) // if it's your piece
        return -1;
      else if (b.spaceAt(x, y) != EMPTY) // it's not your piece, but it's not empty, so it must be your opponent's
        return 1;
    }
  }
  return yours-opponents;*/
}
