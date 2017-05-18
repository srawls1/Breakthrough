#ifndef MIN_MAX_TREE_H
#define MIN_MAX_TREE_H
#include "Board.h"
#include <vector>

class MinMaxTree;
struct Pair
{
  Loc loc;
  int value;
  Pair(Loc l, int v) : loc(l), value(v) {}
};
//Nodes are contained in the MinMaxTree
class Node {
  Board b;
  Loc move;
  int value;
  Space side;
  bool pruning;
  bool max; // if true, max node, else, min node
  vector<Node*> children;
public:
  Node(Board& bb, Loc m, Space s, bool p, bool mx);//constructor
  ~Node();//deconstructor
  bool get_value(){return value;}//returns value
  int eval();//evaluates the node
  vector<Node*> get_children(){return children;} //gets the chidlren

  friend class MinMaxTree;
};
//our minmax algorithm
class MinMaxTree {
  Node* root; //root of the tree
  bool pruning;
  int depth;
  Space side;
  Board b;
public:
  MinMaxTree(Board& bb, Space s, int d, bool p,Loc loc); //constructor
  ~MinMaxTree(); //deconstructor
  Pair* get_optimal_move(); //gets best move
  Node* get_root(){return root;} //returns root

};
int minmax(Node* n, int depth, bool maximizing);
int alphabeta(Node* n, int depth, int a, int b, bool maximizing); //alphabeta pruning function
int evaluate_leaf(Board& b, Space side, Loc loc, bool maximizing); //evaluates a leaf

#endif
