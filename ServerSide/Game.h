#ifndef GAME_H
#define GAME_H
#include "Board.h"
#include "AI.h"
#include <iostream>
#include <stack>


class Game{
  stack<Board> states;//boards stored for undo
  AI ai;//ai player plays against
  Space turn;//whos current play
  const int MAX_UNDO_MOVES = 10;//max undos allowed
  int undoMoves = 0;//# undos used
  string message;//output of game to screen
  bool show = true;//if to display board

public:
  Game();//constructors
  Game(int difficulty);
  Game(int difficulty, bool s);
  Game(int difficulty, Space color, bool s);
  Game(const Game& g);//copy constructor
  ~Game();

  string getMessage();//get message to send to client
  string getAIMove();//get message to send to client
  void setMessage();//update message

  bool makeMove(int x, int y, dir d);//make move
  void aiMove();//ai makes move
  bool undo();//undo move
  void reset();//reset game
  void changeTurn();//flip turn
  void gameOver();//update message for game over
  bool is_game_over();//if game is over
  void display();//flip show
};

#endif
