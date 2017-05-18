#ifndef MIN_MAX_TREE_H
#define MIN_MAX_TREE_H
#include "Board.h"

int evaluate_leaf(Board& b, Space side);
bool prune(int a, int b, bool max);
int evaluate_root(Board& b, Space side, int depth, bool max, bool pruning, int parentval);
Loc get_optimal_move(Board& b, Space side, int depth, bool pruning);

#endif
