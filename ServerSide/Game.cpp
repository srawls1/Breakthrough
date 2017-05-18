#include "Game.h"
using namespace std;

//constructor
Game::Game() {
	ai = AI(8,BLACK);

}

//constructor
Game::Game(int difficulty): ai(difficulty, BLACK), turn(WHITE)
{
	states.push(Board());
}

Game::Game(int difficulty, bool s): ai(difficulty, BLACK), turn(WHITE), show(s)
{
	states.push(Board());
}

Game::Game(int difficulty, Space color, bool s): ai(difficulty, color), turn(WHITE), show(s)
{
	states.push(Board());
}

//copy constructor
Game::Game(const Game& g) : states(g.states), ai(g.ai), turn(g.turn) {}

//destructor
Game::~Game() {}

//make player move
bool Game::makeMove(int x, int y, dir d) {
	Board b = states.top();//get board

	bool invalidFlag = !(b.movePiece(x, y, d, turn));//make move

	states.push(b);//update board

	if (invalidFlag)//if bad move
		message = "ILLEGAL\n";
	else
	{
		changeTurn();//flip turn
		setMessage();//update output message
	}
	return !invalidFlag;//return move status
}

//ai moves
void Game::aiMove()
{
	Board b = states.top();//get board
	
	if (is_game_over())//if game is over
		return;

	ai.ai_move(b);//move
	states.push(b);//update board

	changeTurn();//rotate turn

	message  = ai.getMove();//update message

	if(show)//add board
		message += states.top().toString();

	if(is_game_over()){ //check if over
		gameOver();
	}
}

//check if game is over
bool Game::is_game_over(){
	Board last_state = states.top();//get board
	Space s = last_state.victory();//check status of board
	
	if(s != EMPTY){
		return true;
	}
	
	//check if draw
	if(last_state.listMoves(BLACK).size() < 1 && last_state.listMoves(WHITE).size() < 1)//never should be true
		return true;
		
	return false;

}

//undo previous move and ai's move
bool Game::undo() {
    if (undoMoves < MAX_UNDO_MOVES  &&  states.size() >= 3){
        states.pop(); //remove the last 2 top boards
		states.pop();
		undoMoves++;
		return true;
    }
	return false;
}

//determine if game is over
void Game::gameOver() {
	Board last_state = states.top();//get last baord
	Space s = last_state.victory();//check board state
	
	message += "; Game Over ";//find who won
	if(s == BLACK){
		message += "BLACK Wins!!\n";
	}
	if(s == WHITE){
		message += "WHITE Wins!!\n";
	}
	
	//check if no moves are left
	if(last_state.listMoves(BLACK).size() < 1 && last_state.listMoves(WHITE).size() < 1)//never should be true
		message += "DRAW!!\n";
}

//flip show, whether to display
void Game::display() {
	if(show)
		show = false;
	else
		show = true;
}

//return message
string Game::getMessage() {
	return message;
}

//clear game, not used
void Game::reset(){
    while (!states.empty()) {
        states.pop();
    }
}

//flip turn
void Game::changeTurn()
{
	if(turn == WHITE)
		turn = BLACK;
	else
		turn = WHITE;
}

//set message of comment to print
void Game::setMessage()
{
	if(show)//if to print board
		message = states.top().toString();
	else
		message = "";

	if(is_game_over()){ //if game is over
		gameOver();
	}
}

string Game::getAIMove()
{
	return ai.getMove();
}